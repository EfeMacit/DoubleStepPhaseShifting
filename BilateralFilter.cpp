#include "BilateralFilter.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <limits>

BilateralFilter::BilateralFilter(const std::string& inputFilename, const std::string& outputFilename)
        : inputFilename(inputFilename), outputFilename(outputFilename) {}



float BilateralFilter::gaussian(float x, float sigma) {
    static const float sqrt2pi = std::sqrt(2 * M_PI);
    return std::exp(-0.5f * x * x / (sigma * sigma)) / (sigma * sqrt2pi);
}
void BilateralFilter::applyBilateralFilter(std::vector<std::vector<std::vector<float>>>& pointCloud, int iterations,float spatialSigma,float rangeSigma) {
    int width = pointCloud.size();
    int height = pointCloud[0].size();

    for (int iter = 0; iter < iterations; ++iter) {
        auto originalPointCloud = pointCloud;
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                if (std::isnan(originalPointCloud[x][y][0])) continue;//can be removed
                // Calculate adaptive sigma values based on local neighborhood

                std::vector<float> filteredPoint(6, 0.0f);//x,y,z,nx,ny,nz
                float weightSum = 0.0f;
                //spatialsigma σc
                //rangesigma σs
                //Iterating 8
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        if (i == 0 && j == 0) continue;//center
                        int nx = x + i, ny = y + j;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height && !std::isnan(originalPointCloud[nx][ny][0])) {
                            // Compute spatial weight
                            float spatialWeight = gaussian(std::sqrt(float(i * i + j * j)), spatialSigma);
                            //range weight
                            float normalDiff = std::sqrt(
                                    (originalPointCloud[nx][ny][3] - originalPointCloud[x][y][3]) * (originalPointCloud[nx][ny][3] - originalPointCloud[x][y][3]) +
                                    (originalPointCloud[nx][ny][4] - originalPointCloud[x][y][4]) * (originalPointCloud[nx][ny][4] - originalPointCloud[x][y][4]) +
                                    (originalPointCloud[nx][ny][5] - originalPointCloud[x][y][5]) * (originalPointCloud[nx][ny][5] - originalPointCloud[x][y][5]));
                            float rangeWeight = gaussian(normalDiff, rangeSigma);
                            //Adding weights
                            float weight = spatialWeight * rangeWeight;
                            weightSum += weight;
                            //Updating
                            for (int k = 0; k < 6; ++k) {
                                filteredPoint[k] += originalPointCloud[nx][ny][k] * weight;
                            }
                        }
                    }
                }
                //updating point cloud
                if (weightSum > 0) {
                    for (int k = 0; k < 6; ++k) {
                        pointCloud[x][y][k] = filteredPoint[k] / weightSum;
                    }
                }
            }
        }
    }
}
//this version is without iterations
/*
void BilateralFilter::applyBilateralFilter(std::vector<std::vector<std::vector<float>>>& pointCloud, float spatialSigma, float rangeSigma) {
    int width = pointCloud.size();
    int height = pointCloud[0].size();

    auto originalPointCloud = pointCloud;//copy

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (std::isnan(originalPointCloud[x][y][0])) continue;//can be removed

            std::vector<float> filteredPoint(6, 0.0f);//x,y,z,nx,ny,nz
            float weightSum = 0.0f;
            //spatialsigma σc
            //rangesigma σs
            //8 neighbors
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (i == 0 && j == 0) continue;//center

                    int nx = x + i, ny = y + j;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height && !std::isnan(originalPointCloud[nx][ny][0])) {
                        // Compute spatial weight
                        float spatialWeight = gaussian(std::sqrt(float(i * i + j * j)), spatialSigma);

                        // Compute range weight (based on normal difference)
                        float normalDiff = std::sqrt(
                                (originalPointCloud[nx][ny][3] - originalPointCloud[x][y][3]) * (originalPointCloud[nx][ny][3] - originalPointCloud[x][y][3]) +
                                (originalPointCloud[nx][ny][4] - originalPointCloud[x][y][4]) * (originalPointCloud[nx][ny][4] - originalPointCloud[x][y][4]) +
                                (originalPointCloud[nx][ny][5] - originalPointCloud[x][y][5]) * (originalPointCloud[nx][ny][5] - originalPointCloud[x][y][5]));
                        float rangeWeight = gaussian(normalDiff, rangeSigma);

                        // Total weight
                        float weight = spatialWeight * rangeWeight;
                        weightSum += weight;

                        // Update filtered point values
                        for (int k = 0; k < 6; ++k) {
                            filteredPoint[k] += originalPointCloud[nx][ny][k] * weight;
                        }
                    }
                }
            }
            //updating point cloud
            if (weightSum > 0) {
                for (int k = 0; k < 6; ++k) {
                    pointCloud[x][y][k] = filteredPoint[k] / weightSum;
                }
            }
        }
    }
}
*/

void BilateralFilter::writePLYFile(const std::string& filename, const std::vector<std::vector<std::vector<float>>>& organized_point_cloud, bool organized = false) {
    std::ofstream plyFile(filename);

    if (!plyFile) {
        std::cout << "Unable to open file";
        return;
    }

    // Calculate total vertices
    size_t totalVertices = 0;
    if (!organized){
        for (const auto& row : organized_point_cloud) {
            for (const auto& pt : row) {
                if (!std::isnan(pt[0])) { // assuming if x is NaN, the entire point is invalid
                    totalVertices++;
                }
            }
        }
    }
    else {
        totalVertices = organized_point_cloud.size() * organized_point_cloud[0].size();
    }


    plyFile << "ply\n";
    plyFile << "format ascii 1.0\n";
    plyFile << "element vertex " << totalVertices << "\n";
    plyFile << "property float x\n";
    plyFile << "property float y\n";
    plyFile << "property float z\n";
    plyFile << "property float nx\n";
    plyFile << "property float ny\n";
    plyFile << "property float nz\n";
    plyFile << "element face " << 0 << "\n";
    plyFile << "property list uchar int vertex_indices\n";
    plyFile << "end_header\n";

    for (const auto& row : organized_point_cloud) {
        for (const auto& pt : row) {
            if (organized || !std::isnan(pt[0])) { // assuming if x is NaN, the entire point is invalid
                plyFile << pt[0] << " " << pt[1] << " " << pt[2] << " "
                        << pt[3] << " " << pt[4] << " " << pt[5] << "\n";
            }
        }
    }
}

std::vector<std::vector<std::vector<float>>> BilateralFilter::readPLYFileWithNormals(const std::string& filename,int width,int height) {
    std::ifstream inFile(filename);
    std::vector<std::vector<std::vector<float>>> organized_point_cloud(width, std::vector<std::vector<float>>(height, std::vector<float>(6, std::numeric_limits<float>::quiet_NaN())));  // Initialize with NaN values

    std::string line;


    if (!inFile) {
        std::cerr << "Unable to open file\n";
        return organized_point_cloud;  // return empty opc
    }

    // Read and skip header lines until we reach end_header
    while (std::getline(inFile, line)) {
        if (line == "end_header\r") //"end_header case does not break"
        {
            break;
        }
    }

    int nan = 0;
    int no_nan = 0;
    int current_row = 0, current_col = 0;

    // Read data lines
    while (std::getline(inFile, line) && current_row < width) {
        std::istringstream lineStream(line);
        float x, y, z, nx, ny, nz;

        // If parsing succeeds and none of the values are NaN
        if ((lineStream >> x >> y >> z >> nx >> ny >> nz)) {
            organized_point_cloud[current_row][current_col] = { x, y, z, nx, ny, nz };
            no_nan++;
        }
        else {
            nan++;
        }

        current_col++;
        if (current_col >= height) {
            current_col = 0;
            current_row++;
        }
    }

    std::cout << "Nan: " << nan << " Not Nan: " << no_nan << std::endl;
    return organized_point_cloud;
}
void BilateralFilter::processFilter() {
    auto pointCloud = readPLYFileWithNormals(inputFilename,meshWidth,meshHeight);
    //applyBilateralFilter(pointCloud,spatialSigma,rangeSigma);
    applyBilateralFilter(pointCloud,10,spatialSigma,rangeSigma);
    writePLYFile(outputFilename, pointCloud);
    std::cout << "Bilateral filtering completed. Filtered point cloud saved to " << outputFilename << std::endl;
}