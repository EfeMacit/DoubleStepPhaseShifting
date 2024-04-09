#include "CorrespondenceMatching.h"

cv::Mat CorrespondenceMatching::readPhaseMap(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::vector<double>> matrix;
    std::string line, value;

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::vector<double> row;
        while (getline(ss, value, ',')) {
            row.push_back(std::stod(value));
        }
        matrix.push_back(row);
    }

    cv::Mat phaseMap(matrix.size(), matrix[0].size(), CV_64F);
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            phaseMap.at<double>(i, j) = matrix[i][j];
        }
    }

    return phaseMap;
}
cv::Mat CorrespondenceMatching::calculateDisparity(const cv::Mat& leftImage, const cv::Mat& rightImage) {
    cv::Mat disparityMap = cv::Mat::zeros(leftImage.rows, leftImage.cols, CV_64F);

    for (int y = 0; y < leftImage.rows; ++y) {
        for (int x = 0; x < leftImage.cols; ++x) {
            double leftPixel =leftImage.at<double>(y, x);
            double rightPixel= rightImage.at<double>(y, x);
            //exact match
            if (leftPixel -rightPixel==0) {
                //pixel is then marked with a black for visualization.
                //best case all image is black
                disparityMap.at<double>(y, x) = 1.0;
            }
        }
    }
    return disparityMap;
}
//Better approach but in this case every pixel in images have an exact match therefore it seems unnecessary but works well
/*
cv::Mat CorrespondenceMatching::calculateDisparity(const cv::Mat& leftImage, const cv::Mat& rightImage, int blockSize, int maxDisparity) {
    CV_Assert(leftImage.type() == CV_64F && rightImage.type() == CV_64F);
    CV_Assert(leftImage.rows == rightImage.rows && leftImage.cols == rightImage.cols);

    int width = leftImage.cols;
    int height = leftImage.rows;
    cv::Mat disparityMap = cv::Mat::zeros(height, width, CV_64F);
    int halfBlockSize = blockSize / 2;
    for (int y = halfBlockSize; y < height - halfBlockSize; ++y) {
        for (int x = halfBlockSize; x < width - halfBlockSize; ++x) {
            double minSAD = std::numeric_limits<double>::max();
            int bestDisparity = 0;
            // Search for the best match within the disparity range
            for (int d = 0; d <= maxDisparity; ++d) {
                if (x - d < halfBlockSize) continue; //bound check
                double SAD = 0.0;
                //Sum of Absolute Differences
                for (int by = -halfBlockSize; by <= halfBlockSize; ++by) {
                    for (int bx = -halfBlockSize; bx <= halfBlockSize; ++bx) {
                        double leftPixel = leftImage.at<double>(y + by, x + bx);
                        double rightPixel = rightImage.at<double>(y + by, x + bx - d);
                        SAD += std::abs(leftPixel - rightPixel);
                    }
                }
                //update
                if (SAD < minSAD) {
                    minSAD = SAD;
                    bestDisparity = d;
                }
            }
            disparityMap.at<double>(y, x) = static_cast<double>(bestDisparity);
        }
    }
    return disparityMap;
}
 */
void CorrespondenceMatching::showDisparityMap(const cv::Mat& disparityMap) {
    cv::Mat displayMap;
    disparityMap.convertTo(displayMap, CV_8U);
    imshow("Disparity Map", displayMap);
    cv::waitKey(0);
}
void CorrespondenceMatching::processImages(const std::string& leftFilename, const std::string& rightFilename) {
    cv::Mat leftMap = readPhaseMap(leftFilename);
    cv::Mat rightMap = readPhaseMap(rightFilename);
    //converting 8-bit for medianblur
    leftMap.convertTo(leftMap, CV_8U);
    rightMap.convertTo(rightMap, CV_8U);
    //medianBlur for impulse noise and uniform noise
    medianBlur(leftMap, leftMap, 5);
    medianBlur(rightMap, rightMap, 5);
    //converting 64-bit for gaussianblur
    leftMap.convertTo(leftMap, CV_64F);
    rightMap.convertTo(rightMap, CV_64F);
    //for gaussian noise
    GaussianBlur(leftMap, leftMap, cv::Size(5, 5), 1.5);
    GaussianBlur(rightMap, rightMap, cv::Size(5, 5), 1.5);

    cv::Mat disparityMap = calculateDisparity(leftMap, rightMap);
    showDisparityMap(disparityMap);
}
