//////////////////////////////////////////////////////////////////////////
// Option Images
// Projet, s�ance 1
// th�me : premier pas en OpenCV
// contenu : charge, affiche, r�duction, calcul et affichage d'histogramme
// version : 17.1128
//////////////////////////////////////////////////////////////////////////


#include <iostream>
using namespace std;

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

#include "histogram.hpp"


int main (void) {
    //charge et affiche l'image (� MODIFIER) :
    string imName = "../NicIcon/all-scans/00000.png";
    Mat img_rgb = imread(imName);
    Mat img_gray;
    cvtColor(img_rgb, img_gray, COLOR_BGR2GRAY);
    Mat shape_template = imread("../symboles/accident.png",0);

    Mat img_display;
    img_rgb.copyTo( img_display );

    Mat res;
    matchTemplate(img_gray,shape_template, res,TM_CCOEFF_NORMED);
    normalize( res, res, 0, 1, NORM_MINMAX, -1, Mat() );
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    minMaxLoc( res, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    matchLoc = maxLoc;

    rectangle( img_display, Point( matchLoc.x , matchLoc.y - shape_template.rows ), Point( matchLoc.x + 2100 , matchLoc.y + 1.75 * shape_template.rows ), Scalar::all(0), 2, 8, 0 );
    rectangle( res, matchLoc, Point( matchLoc.x + shape_template.cols , matchLoc.y + shape_template.rows ), Scalar::all(0), 2, 8, 0 );
    imshow( "image_window", img_display );

    Rect crop(matchLoc.x , matchLoc.y - shape_template.rows, 2100, 2.75 * shape_template.rows);
    Mat sub_image = img_display(crop);
    imshow( "triangle_sub_image", sub_image );

    int reduction = 2;
    Size tailleReduite(img_display.cols/reduction, img_display.rows/reduction);
    Mat imreduite = Mat(tailleReduite,CV_8UC3); //cree une image � 3 canaux de profondeur 8 bits chacuns
    resize(img_display,imreduite,tailleReduite);
    imshow("image reduite", imreduite);

    //termine le programme lorsqu'une touche est frappee
    waitKey(0);
    return EXIT_SUCCESS;
}
