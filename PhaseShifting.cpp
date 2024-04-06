#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <bitset>
#include <fstream>
#include <string>
using namespace cv;
using namespace std;
/*
const int width = 1280;
const int height = 720;
const double phase_shift = 60; // Degrees
const double wavelength = width / 128.0; // 10 pixels period, yielding 128 periods across the width
const int numGrayImages = 7;

int grayToBinary(int gray) {
    int binary = 0;
    for (; gray; gray >>= 1) {
        binary ^= gray;
    }
    return binary;
}
// Function to decode Gray-coded binary images to decimal values
cv::Mat decodeGrayImagesToDecimal(const std::vector<cv::Mat>& grayImages) {
    cv::Mat decimalImage(height, width, CV_32SC1, cv::Scalar(0));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int decimalValue = 0;
            for (int k = 0; k < numGrayImages; ++k) {
                // Assuming the gray code images are thresholded to 0 or 255
                bool bit = grayImages[k].at<uchar>(y, x) > 0;
                decimalValue |= (bit << (numGrayImages - 1 - k));
            }
            // Convert Gray code to binary
            decimalValue = grayToBinary(decimalValue);
            decimalImage.at<int>(y, x) = decimalValue;
        }
    }

    return decimalImage;
}
// Function to unwrap the phase map
cv::Mat unwrapPhaseMap(const cv::Mat& wrappedPhaseMap, const cv::Mat& fringeOrders) {
    cv::Mat unwrappedPhaseMap(height, width, CV_32FC1, cv::Scalar(0));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float wrappedPhase = wrappedPhaseMap.at<float>(y, x);
            int order = fringeOrders.at<int>(y, x);
            unwrappedPhaseMap.at<float>(y, x) = wrappedPhase + 2.0 * CV_PI * order;
        }
    }

    return unwrappedPhaseMap;
}
cv::Mat averagePhaseMaps(const cv::Mat& phaseMap1, const cv::Mat& phaseMap2) {
    assert(phaseMap1.size() == phaseMap2.size() && phaseMap1.type() == CV_32FC1 && phaseMap2.type() == CV_32FC1);

    cv::Mat averagedPhaseMap(phaseMap1.size(), CV_32FC1);

    for (int y = 0; y < phaseMap1.rows; ++y) {
        for (int x = 0; x < phaseMap1.cols; ++x) {
            float phase1 = phaseMap1.at<float>(y, x);
            float phase2 = phaseMap2.at<float>(y, x);

            // Convert phases to complex numbers (vectors on the unit circle)
            std::complex<float> complex1(std::cos(phase1), std::sin(phase1));
            std::complex<float> complex2(std::cos(phase2), std::sin(phase2));

            // Average the complex numbers
            std::complex<float> averagedComplex = 0.5f * (complex1 + complex2);

            // Convert back to phase
            averagedPhaseMap.at<float>(y, x) = std::atan2(averagedComplex.imag(), averagedComplex.real());
        }
    }

    return averagedPhaseMap;
}

void plotRow(const cv::Mat& unwrappedPhaseMap, int rowIndex, const std::string& windowName) {
    // Create an image to draw the plot
    cv::Mat plotImage = cv::Mat::zeros(400, unwrappedPhaseMap.cols, CV_8UC3);

    // Normalize the phase values to fit in the plot image
    double minVal, maxVal;
    cv::minMaxLoc(unwrappedPhaseMap.row(rowIndex), &minVal, &maxVal);
    cv::Mat normalizedRow;
    unwrappedPhaseMap.row(rowIndex).convertTo(normalizedRow, CV_64F, 1.0 / (maxVal - minVal), -minVal / (maxVal - minVal));

    // Draw the plot
    for (int i = 1; i < unwrappedPhaseMap.cols; ++i) {
        cv::line(plotImage,
                 cv::Point(i - 1, 400 - static_cast<int>(normalizedRow.at<double>(0, i - 1) * 400)),
                 cv::Point(i, 400 - static_cast<int>(normalizedRow.at<double>(0, i) * 400)),
                 cv::Scalar(255, 0, 0), 2);
    }

    // Display the plot
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    cv::imshow(windowName, plotImage);
}
cv::Mat computePhaseMap(const cv::Mat& I1, const cv::Mat& I2, const cv::Mat& I3) {
    cv::Mat phaseMap(height, width, CV_32FC1);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float i1 = I1.at<uchar>(y, x);
            float i2 = I2.at<uchar>(y, x);
            float i3 = I3.at<uchar>(y, x);

            phaseMap.at<float>(y, x) = std::atan2(sqrt(3) * (i1 - i3), 2 * i2 - i1 - i3);
        }
    }

    return phaseMap;
}
*/
//PARTTTTTT333333
/*
Mat readPhaseMap(const string& filename) {
    ifstream file(filename);
    vector<vector<double>> matrix;
    string line, value;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<double> row;
        while (getline(ss, value, ',')) {
            row.push_back(stod(value));
        }
        matrix.push_back(row);
    }

    Mat phaseMap(matrix.size(), matrix[0].size(), CV_64F);
    for (size_t i = 0; i < matrix.size(); i++)
        for (size_t j = 0; j < matrix[i].size(); j++)
            phaseMap.at<double>(i, j) = matrix[i][j];

    return phaseMap;
}

Mat calculateDisparity(const Mat& leftImage, const Mat& rightImage) {
    Mat disparityMap = Mat::zeros(leftImage.rows, leftImage.cols, CV_64F);

    for (int y = 0; y < leftImage.rows; ++y) {
        for (int x = 0; x < leftImage.cols; ++x) {
            double leftPixel = leftImage.at<double>(y, x);
            double rightPixel = rightImage.at<double>(y, x);

            // Check for exact match
            if (leftPixel != rightPixel) {
                // If there is any mismatch, the assumption of 'ideal case' is violated.
                // The pixel is then marked with a non-zero value (for visualization, let's say 1.0).
                disparityMap.at<double>(y, x) = 1.0;
            }
            // If the pixels match exactly, the value in the disparity map remains zero.
        }
    }
    return disparityMap;
}

void showDisparityMap(const Mat& disparityMap) {
    // Create an image with the same size as the disparity map, but with 8-bit depth
    Mat displayMap = Mat::zeros(disparityMap.size(), CV_8U);

    // Convert the 64F image to 8U image. Since we are not expecting any non-zero values,
    // this should result in a black image if the disparity map is indeed all zeros.
    disparityMap.convertTo(displayMap, CV_8U);

    // Show the disparity map
    imshow("Disparity Map", displayMap);
    waitKey(0);

}
*/
int main() {
    //PART1111111
    /*
    std::vector<cv::Mat> patterns(6);
    std::vector<cv::Mat> grayImages(numGrayImages);
    // Generate the six patterns
    for (int k = 0; k < 6; ++k) {
        patterns[k] = cv::Mat(height, width, CV_8UC1);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                double value = 0.5 * (1 + std::cos(2 * CV_PI * x / wavelength + k * CV_PI * phase_shift / 180.0));
                patterns[k].at<uchar>(y, x) = static_cast<uchar>(value * 255);
            }
        }
        //std::string windowTitle = "Sinusoidal Pattern " + std::to_string(k + 1);
        //cv::namedWindow(windowTitle, cv::WINDOW_AUTOSIZE);
        //cv::imshow(windowTitle, patterns[k]);
    }
    // Compute phase maps
    cv::Mat phaseMap1 = computePhaseMap(patterns[0], patterns[1], patterns[2]);
    cv::Mat phaseMap2 = computePhaseMap(patterns[3], patterns[4], patterns[5]);
    cv::Mat averagePhaseMap = averagePhaseMaps(phaseMap1,phaseMap2);
    // Display the phase maps
    //cv::imshow("Phase Map 1", phaseMap1);
    //cv::imshow("Phase Map 2", phaseMap2);
    //cv::imshow("Average Phase Map", averagePhaseMap);
    //cv::waitKey(0);
    //cv::Mat normalizedPhaseMap1, normalizedPhaseMap2, normalizedAveragePhaseMap;

    //cv::normalize(phaseMap1, normalizedPhaseMap1, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    //cv::normalize(phaseMap2, normalizedPhaseMap2, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    //cv::normalize(averagePhaseMap, normalizedAveragePhaseMap, 0, 255, cv::NORM_MINMAX, CV_8UC1);

    //cv::imwrite("normalizedPhaseMap1.png", normalizedPhaseMap1);
    //cv::imwrite("normalizedPhaseMap2.png", normalizedPhaseMap2);
    //cv::imwrite("normalizedAveragePhaseMap.png", normalizedAveragePhaseMap);
    for (int i = 0; i < numGrayImages; ++i) {
        std::string filePath = "gray_pattern_" + std::to_string(i) + ".png";
        grayImages[i] = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
        if (grayImages[i].empty()) {
            std::cerr << "Error reading file: " << filePath << std::endl;
            // Handle error, e.g., by exiting or throwing an exception
        }
    }
    // Decode Gray-coded images to obtain fringe orders as a decimal matrix
    cv::Mat fringeOrders = decodeGrayImagesToDecimal(grayImages);

    // Unwrap the phase map using the fringe orders
    cv::Mat unwrappedPhaseMap = unwrapPhaseMap(averagePhaseMap, fringeOrders);
    cv::imwrite("unwrappedPhaseMap.png", unwrappedPhaseMap);
    plotRow(unwrappedPhaseMap, unwrappedPhaseMap.rows / 2, "Unwrapped Phase Plot");
    cv::waitKey(0);
    */
    //PARTTTT333333
/*
    Mat leftMap = readPhaseMap("left_uwp_map.csv");
    Mat rightMap = readPhaseMap("right_uwp_map.csv");

    // Convert maps from double to 8-bit for median filtering
    leftMap.convertTo(leftMap, CV_8U);
    rightMap.convertTo(rightMap, CV_8U);

    // Apply median filter to mitigate impulse noise
    medianBlur(leftMap, leftMap, 5);
    medianBlur(rightMap, rightMap, 5);

    // Convert maps back to double precision
    leftMap.convertTo(leftMap, CV_64F);
    rightMap.convertTo(rightMap, CV_64F);

    // Apply Gaussian blur to mitigate Gaussian and uniform noise
    GaussianBlur(leftMap, leftMap, Size(5, 5), 1.5);
    GaussianBlur(rightMap, rightMap, Size(5, 5), 1.5);

    // Calculate disparity
    Mat disparityMap = calculateDisparity(leftMap, rightMap);

    // Display disparity map
    showDisparityMap(disparityMap);
*/
    return 0;
}
