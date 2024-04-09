#include "BilateralFilter.h"
#include "CorrespondenceMatching.h"
#include "DoubleThreeStepPhaseShifting.h"
int main() {
/*
    DoubleThreeStepPhaseShifting phaseShifting;
    phaseShifting.processPhaseShift();
*/

    BilateralFilter bilateralFilter("3D-bilateral/OriginalMesh.ply", "FilteredMesh.ply");
    bilateralFilter.processFilter();

/*
    CorrespondenceMatching matcher;
    matcher.processImages("correspondence-matching/left_uwp_map.csv", "correspondence-matching/right_uwp_map.csv");
*/
    return 0;

}
