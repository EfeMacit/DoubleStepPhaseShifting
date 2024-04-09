#ifndef BILATERALFILTER_H
#define BILATERALFILTER_H

#include <vector>
#include <string>

class BilateralFilter {
public:
    BilateralFilter(const std::string& inputFilename, const std::string& outputFilename);
    void processFilter();
private:

    float gaussian(float x, float sigma);
    //void applyBilateralFilter(std::vector<std::vector<std::vector<float>>>& pointCloud,float spatialSigma,float rangeSigma);
    void applyBilateralFilter(std::vector<std::vector<std::vector<float>>>& pointCloud,int iteration,float spatialSigma,float rangeSigma);
    void writePLYFile(const std::string& filename, const std::vector<std::vector<std::vector<float>>>& organized_point_cloud, bool organized);
    std::vector<std::vector<std::vector<float>>> readPLYFileWithNormals(const std::string& filename, int width, int height);
    std::string inputFilename;
    std::string outputFilename;
    float spatialSigma = std::exp(-12);
    float rangeSigma = std::exp(-12);
    const int meshWidth = 2592;
    const int meshHeight = 1944;

};

#endif // BILATERALFILTER_H