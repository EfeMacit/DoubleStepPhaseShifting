#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <complex>

using namespace cv;
using namespace std;

class DoubleThreeStepPhaseShifting {
public:
    DoubleThreeStepPhaseShifting();
    void processPhaseShift();
private:
    void generatePatterns();
    Mat computePhaseMap(const Mat& I1, const Mat& I2, const Mat& I3);
    void averagePhaseMaps();
    void loadGrayImages(std::vector<cv::Mat>& grayImages, int numGrayImages, const std::string& basePath);
    int grayToBinary(int gray);
    void decodeGrayImages();
    void unwrapPhaseMap();
    void plotRow(int rowIndex, const string& windowName);
    int width = 1280, height = 720;
    double phase_shift = 60, wavelength = width/128.0;
    int numGrayImages=7;
    vector<Mat> patterns;
    vector<Mat> grayImages;
    Mat phaseMap1, phaseMap2, averagePhaseMap, fringeOrders, unwrappedPhaseMap;
};