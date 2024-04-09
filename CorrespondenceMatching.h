#ifndef CORRESPONDENCEMATCHING_H
#define CORRESPONDENCEMATCHING_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>

class CorrespondenceMatching {
public:
    void processImages(const std::string& leftFilename, const std::string& rightFilename);

private:
    cv::Mat readPhaseMap(const std::string& filename);
    cv::Mat calculateDisparity(const cv::Mat& leftImage, const cv::Mat& rightImage);
    //cv::Mat calculateDisparity(const cv::Mat& leftImage, const cv::Mat& rightImage,int blockSize,int maxDisparity);
    void showDisparityMap(const cv::Mat& disparityMap);
};

#endif // CORRESPONDENCEMATCHING_H
