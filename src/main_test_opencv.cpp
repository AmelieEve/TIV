//////////////////////////////////////////////////////////////////////////
// Option Images
// Projet, séance 1
// thème : premier pas en OpenCV
// contenu : charge, affiche, réduction, calcul et affichage d'histogramme
// version : 17.1128
//////////////////////////////////////////////////////////////////////////


#include <iostream>
using namespace std;

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

#include "histogram.hpp"

//////////////////////////////////////////////////////////////////////////
// Détection des coins
// Bord supérieur droite
int min_x_up_r = 1900;
int max_y_up_r = 580;
// Bord inférieur gauche
int max_x_down_l = 400;
int min_y_down_l = 3000;
//////////////////////////////////////////////////////////////////////////

/*
int main(void) {
    //charge et affiche l'image
    string imName = "../NicIcon/all-scans/00106.png";
    // FIXME: Image 105 et 106: coins non reconnus
    Mat im = imread(imName);
    if(im.data == nullptr){
        cerr << "Image not found: "<< imName << endl;
        waitKey(0);
        //system("pause");
        exit(EXIT_FAILURE);
    }


    Mat imd;
    cvtColor(im,im, COLOR_BGR2GRAY);
    morphologyEx(im,imd, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(2,2)));
    //dilate(imd, imd, getStructuringElement(MORPH_CROSS, Size(25,25)));
    namedWindow("eroded",WINDOW_NORMAL);
    Mat imdb;

    threshold( imd, imdb, 128,255,THRESH_BINARY_INV );


    Mat labels;
    Mat stats;
    Mat centroids;
    cv::connectedComponentsWithStats(imdb, labels, stats, centroids);

    Point up_r;
    Point down_l;
    bool found_ur = false;
    bool found_dl = false;

    for(int i=0; i<stats.rows; i++)
    {
        int x = stats.at<int>(Point(0, i));
        int y = stats.at<int>(Point(1, i));
        int w = stats.at<int>(Point(2, i));
        int h = stats.at<int>(Point(3, i));

        std::cout << "x=" << x << " y=" << y << " w=" << w << " h=" << h << std::endl;

        if (x>min_x_up_r && y<max_y_up_r) {
            up_r = Point(x+w/2, y+h/2);
            if(found_ur)
                std::cerr << "WARNING: Two upper right corners detected!" << std::endl;
            found_ur = true;
        }
        if (x<max_x_down_l && y>min_y_down_l) {
            down_l = Point(x+w/2, y+h/2);
            if(found_dl)
                std::cerr << "WARNING: Two lower left corners detected!" << std::endl;
            found_dl = true;
        }
    }

    if (!found_ur || !found_dl)
        std::cerr << "WARNING: At least one corner is missing" << std::endl;

    std::cout << up_r << ";" << down_l << std::endl;

    rectangle(im, up_r, down_l, Scalar(128,0,0));
    cv::namedWindow("base",WINDOW_NORMAL);
    imshow("base", im);
    imshow("eroded", imdb);

    waitKey(0);
}*/
