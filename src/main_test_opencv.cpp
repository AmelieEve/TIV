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
#include "main_rectangle.h"


string fileFromIds(int seriesId, int fileId, string basePath) {
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << seriesId;
    oss << std::setw(2) << fileId;
    return basePath + oss.str() + ".png";
}



int main(int argc, char** argv) {
    int nbPagesGood = 0;
    int nbPagesTotal = 0;
    int deb = 0;
    int fin = 35;
    if (argc>=3) {
        deb = ::atoi(argv[1]);
        fin = ::atoi(argv[2]);
    }
    for (int iSeries = deb; iSeries < fin; iSeries++)
        for (int iFile = 0; iFile < 22; iFile++) {
            nbPagesTotal++;
            string file = fileFromIds(iSeries, iFile, "../NicIcon/all-scans/");
            if (parse_page(file, iSeries, iFile))
                nbPagesGood++;
        }

    cout << "DONE! " << nbPagesGood << "/" << nbPagesTotal << " pages correctly parsed" << endl;

}