//////////////////////////////////////////////////////////////////////////
// Option Images
// Projet, séance 1
// thème : premier pas en OpenCV
// contenu : charge, affiche, réduction, calcul et affichage d'histogramme
// version : 17.1128
//////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <iomanip>

using namespace std;

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

#include "imageRectifier.h"


string fileFromIds(int seriesId, int fileId, string basePath) {
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << seriesId;
    oss << std::setw(2) << fileId;
    return basePath + oss.str() + ".png";
}



int main(void) {
    /*for (int iSeries = 0; iSeries < 35; iSeries++)
        for (int iFile = 0; iFile < 22; iFile++)
            rectify(fileFromIds(iSeries, iFile, "../NicIcon/all-scans/"));*/
    Mat im;
    rectify("../NicIcon/all-scans/00020.png", im);
    namedWindow("base", WINDOW_NORMAL);
    imshow("base", im);
    waitKey(0);

}