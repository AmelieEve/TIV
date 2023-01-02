//
// Created by dorian on 09/12/22.
//
#include "imageRectifier.h"

using namespace std;
using namespace cv;

///////////////////////////////////////
// Détection des coins
// Bord supérieur droite
int min_x_up_r = 1900;
int max_y_up_r = 580;
// Bord inférieur gauche
int max_x_down_l = 400;
int min_y_down_l = 3000;
// Abscisse des points objectifs
int target_x_down_l = 250;
///////////////////////////////////////


bool rectify(string imName, Mat &out) {
    bool debug = false;

    //charge et affiche l'image
    //string imName = "../NicIcon/all-scans/00307.png";
    Mat im = imread(imName);
    if(im.data == nullptr){
        cerr << "Image not found: "<< imName << endl;
        waitKey(0);
        //system("pause");
        exit(EXIT_FAILURE);
    }


    Mat imd;
    cvtColor(im,imd, COLOR_BGR2GRAY);
    morphologyEx(imd,imd, MORPH_ERODE, getStructuringElement(MORPH_CROSS, Size(6,6)));
    dilate(imd, imd, getStructuringElement(MORPH_CROSS, Size(25,25)));
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



        if (x>min_x_up_r && y<max_y_up_r) {
            if (debug) std::cout << "UP RIGHT -> x=" << x << " y=" << y << " w=" << w << " h=" << h << std::endl;
            up_r = Point(x+w/2, y+h/2);
            if(found_ur) {
                if (debug) std::cerr << "WARNING: Two upper right corners detected!" << std::endl;
                return false;
            }
            found_ur = true;
        }
        if (x<max_x_down_l && y>min_y_down_l) {
            if (debug) std::cout << "DOWN LEFT -> x=" << x << " y=" << y << " w=" << w << " h=" << h << std::endl;
            down_l = Point(x+w/2, y+h/2);
            if(found_dl) {
                if (debug) std::cerr << "WARNING: Two lower left corners detected!" << std::endl;
                return false;
            }
            found_dl = true;
        }
    }

    if (!found_ur || !found_dl) {
        if (debug) std::cerr << "WARNING: At least one corner is missing" << std::endl;
        return false;
    }

    if (debug) std::cout << up_r << ";" << down_l << std::endl;

    // Translation pour aligner le point en bas à gauche sur la diagonale

    float tx=target_x_down_l-down_l.x;
    float ty=im.size[0]-(target_x_down_l * M_SQRT2)-down_l.y;
    float mt[] = {1.0,0.0,tx,0.0,1.0,ty};
    Mat translation(2, 3, CV_32F, mt);
    down_l.x += tx;
    down_l.y += ty;
    up_r.x += tx;
    up_r.y += ty;


    // Rotation pour que le point en haut à droite soit aussi sur la diagonale
    // https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors
    float x1 = up_r.x, y1 = up_r.y; // Vraie position
    float x2 = up_r.x, y2 = (im.size[1]-up_r.x) * M_SQRT2; // Position voulue
    float angle = atan2(x1*y2-y1*x2, x1*x2 + y1*y2);

    warpAffine(im,im, translation,im.size());
    warpAffine(im,im,getRotationMatrix2D(down_l,angle,1.0),im.size());
    out = im.clone();

    if (debug) {
        rectangle(im, up_r, down_l, Scalar(128, 0, 0));
        cv::namedWindow("base", WINDOW_NORMAL);
        namedWindow("eroded", WINDOW_NORMAL);
        imshow("base", im);
        imshow("eroded", imdb);
        waitKey(0);
    }

    return true;
}