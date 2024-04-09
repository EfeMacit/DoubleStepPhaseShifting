#include "DoubleThreeStepPhaseShifting.h"

DoubleThreeStepPhaseShifting::DoubleThreeStepPhaseShifting()
{

}

void DoubleThreeStepPhaseShifting::generatePatterns()
{
    patterns.resize(6);
    for (int k = 0; k < 6; ++k) {
        patterns[k] = Mat(height, width, CV_8UC1);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                //convert [-1,1] to [0,1] for proper image
                double value = 0.5*(1 + cos(2 * CV_PI * x / wavelength +k * CV_PI * phase_shift/180.0));
                patterns[k].at<uchar>(y, x) = static_cast<uchar>(value *255);
            }
        }
    }
}

Mat DoubleThreeStepPhaseShifting::computePhaseMap(const Mat& I1, const Mat& I2, const Mat& I3)
{
    Mat phaseMap(height, width, CV_32FC1);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float i1 = I1.at<uchar>(y, x);
            float i2 = I2.at<uchar>(y, x);
            float i3 = I3.at<uchar>(y, x);
            phaseMap.at<float>(y, x) = atan2(sqrt(3) * (i1 - i3), 2 * i2 - i1 - i3);//Equation 7
        }
    }
    return phaseMap;
}

void DoubleThreeStepPhaseShifting::averagePhaseMaps()
{
    //check but can be removed
    assert(phaseMap1.size()==phaseMap2.size() && phaseMap1.type() == CV_32FC1 && phaseMap2.type() == CV_32FC1);

    averagePhaseMap = Mat(phaseMap1.size(), CV_32FC1);

    for (int y = 0; y < phaseMap1.rows; ++y) {
        for (int x = 0; x < phaseMap1.cols; ++x) {
            //reading values from phasemap1 and phasemap2
            float phase1 = phaseMap1.at<float>(y, x);
            float phase2 = phaseMap2.at<float>(y, x);
            // Converting to complex numbers to avaoid incorrect values
            complex<float> complex1(cos(phase1), sin(phase1));
            complex<float> complex2(cos(phase2), sin(phase2));
            // Averaging complexnumbers by adding them then dividing them for magnitude
            complex<float> averagedComplex = 0.5f * (complex1 + complex2);
            // Converting back
            averagePhaseMap.at<float>(y, x) = atan2(averagedComplex.imag(), averagedComplex.real());
        }
    }
}
void DoubleThreeStepPhaseShifting::loadGrayImages(std::vector<cv::Mat>& grayImages, int numGrayImages, const std::string& basePath)
{
    grayImages.resize(numGrayImages);  //vector for grayImages

    for (int i = 0; i < numGrayImages; ++i) {
        std::string filePath = basePath + std::to_string(i) + ".png";//filepath
        // Load the image in grayscale
        grayImages[i] = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
        if (grayImages[i].empty()) {
            std::cerr << "Error reading file: " << filePath << std::endl;
        }
    }
}
int DoubleThreeStepPhaseShifting::grayToBinary(int gray)
{
    int binary = 0;
    for (; gray; gray >>= 1) {
        binary ^= gray;
    }
    return binary;
}
void DoubleThreeStepPhaseShifting::decodeGrayImages()
{
    fringeOrders = Mat(height, width, CV_32SC1, Scalar(0));

    for (int y = 0; y < height; ++y) {
        for (int x = 0;x < width; ++x) {
            int decimalValue = 0;
            for (int k = 0; k<numGrayImages; ++k) {
                // In gray
                bool bit = grayImages[k].at<uchar>(y, x) > 0;
                decimalValue |= (bit << (numGrayImages - 1 - k));
            }
            //Conversion to binary
            decimalValue = grayToBinary(decimalValue);
            fringeOrders.at<int>(y, x) = decimalValue;
        }
    }
}

void DoubleThreeStepPhaseShifting::unwrapPhaseMap()
{
    unwrappedPhaseMap = Mat(height, width, CV_32FC1, Scalar(0));

    for (int y = 0;y < height; ++y) {
        for (int x = 0; x<width; ++x) {
            float wrappedPhase = averagePhaseMap.at<float>(y, x);
            int order = fringeOrders.at<int>(y, x);
            unwrappedPhaseMap.at<float>(y, x) = wrappedPhase+2.0 * CV_PI * order;//2piK
        }
    }
}

void DoubleThreeStepPhaseShifting::plotRow(int rowIndex, const string& windowName)
{
    Mat plotImage = Mat::zeros(400, unwrappedPhaseMap.cols, CV_8UC3);
    double minVal, maxVal;
    minMaxLoc(unwrappedPhaseMap.row(rowIndex), &minVal, &maxVal);
    Mat normalizedRow;
    unwrappedPhaseMap.row(rowIndex).convertTo(normalizedRow, CV_64F, 1.0 / (maxVal - minVal), -minVal / (maxVal - minVal));

    for (int i = 1; i < unwrappedPhaseMap.cols; ++i) {
        line(plotImage,
             Point(i - 1, 400 - static_cast<int>(normalizedRow.at<double>(0, i - 1) * 400)),
             Point(i, 400 - static_cast<int>(normalizedRow.at<double>(0, i) * 400)),
             Scalar(255, 0, 0), 2);
    }

    namedWindow(windowName, WINDOW_AUTOSIZE);
    imshow(windowName, plotImage);
}

void DoubleThreeStepPhaseShifting::processPhaseShift()
{
    //1.Generating patterns
    generatePatterns();
    //2.Compute phase maps and displaying them
    phaseMap1 = computePhaseMap(patterns[0], patterns[1], patterns[2]);
    phaseMap2 = computePhaseMap(patterns[3], patterns[4], patterns[5]);
    cv::imshow("Phase Map 1", phaseMap1);
    cv::imshow("Phase Map 2", phaseMap2);
    //3.Averaging phase maps
    averagePhaseMaps();
    cv::imshow("Average Phase Map", averagePhaseMap);
    //4.Loading Gray-coded images
    std::string filePath = "double-three-step/gray_pattern_";
    loadGrayImages(grayImages,7,filePath);
    //5.Decimal Matrix
    decodeGrayImages();
    //Unwrapping
    unwrapPhaseMap();
    //Plot
    plotRow(height / 2, "Unwrapped Phase Plot Row");
    cv::waitKey(0);
}


