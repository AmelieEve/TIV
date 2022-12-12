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
    Mat shape_template = imread("../symboles/car.png",0);

    Mat img_display;
    img_rgb.copyTo( img_display );

    Mat res;
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc, lastMatch;

    /*** Un symbole, une image ***/
/*    try{
        matchTemplate(img_gray,shape_template, res,TM_CCOEFF_NORMED);
        normalize( res, res, 0, 1, NORM_MINMAX, -1, Mat() );
        minMaxLoc( res, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
        matchLoc = maxLoc;

        Rect crop(matchLoc.x , matchLoc.y - 120, 2100, 332);
        Mat sub_image = img_display(crop);
        rectangle( img_display, Point( matchLoc.x , matchLoc.y - 120 ), Point( matchLoc.x + 2100 , matchLoc.y + 212 ), Scalar::all(0), 2, 8, 0 );
//        imshow( "image_window", img_display );
        imshow( "triangle_sub_image", sub_image );
    }catch(cv::Exception &exception){

    }*/


    /*** Une image, tous les symboles ***/
    vector<cv::String> sym;
    glob("../symboles/*.png", sym, false);
    vector<Mat> symboles;
    size_t count = sym.size();
    for (size_t i=0; i<count; i++) {
        std::cout << "symbole testé : " << sym[i] << std::endl;
        symboles.push_back(imread(sym[i],IMREAD_GRAYSCALE));
        try {
            matchTemplate(img_gray, symboles[i], res, TM_CCOEFF_NORMED);
            normalize(res, res, 0, 1, NORM_MINMAX, -1, Mat());
            minMaxLoc(res, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
            matchLoc = maxLoc;

            Rect crop(matchLoc.x - 30, matchLoc.y - 120, 2100, 332);
            Mat sub_image = img_display(crop);
            imshow(  "test " + sym[i], sub_image);
        }catch(cv::Exception &exception){
            std::cerr << "symbole N°" << i << " not found" << std::endl;
        }
    }



    /*** un symbole, toutes les images test***/
/*  vector<cv::String> fn;
    glob("../NicIcon/all-scans/*.png", fn, false);
    vector<Mat> images;
    vector<Mat> images_gray;
    vector<Mat> sub_images;
    size_t count = fn.size(); //number of png files in images folder
    for (size_t i=0; i<10; i++) {
        std::string name = "test " + std::to_string(i);
//        std::cout << "nom fichier : " << fn[i] << std::endl;

        images.push_back(imread(fn[i]));
        cvtColor(img_rgb, img_gray, COLOR_BGR2GRAY);
        images_gray.push_back(img_gray);

        try{
//            res;
            matchTemplate(images_gray[i],shape_template, res,TM_CCOEFF_NORMED);
            normalize( res, res, 0, 1, NORM_MINMAX, -1, Mat() );
            minMaxLoc( res, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
            matchLoc = maxLoc;
//            lastMatch = matchLoc;

//            if(matchLoc != lastMatch){
                Rect crop(matchLoc.x - 30 , matchLoc.y - 120, 2100, 332);
                sub_images.push_back(images[i](crop));
//            rectangle( img_display, Point( matchLoc.x , matchLoc.y - 120 ), Point( matchLoc.x + 2100 , matchLoc.y + 212 ), Scalar::all(0), 2, 8, 0 );
//            imshow( name_sub, sub_image );
//            }


        }catch(cv::Exception &exception){
            std::cerr << "test" <<std::endl;
        }
    }

    for(int i =0; i< sub_images.size();i++){
        std::string name_sub = "sub " + std::to_string(i);
        imshow( name_sub, sub_images[i] );
    }*/



    int reduction = 2;
    Size tailleReduite(img_display.cols/reduction, img_display.rows/reduction);
    Mat imreduite = Mat(tailleReduite,CV_8UC3); //cree une image � 3 canaux de profondeur 8 bits chacuns
    resize(img_display,imreduite,tailleReduite);
//    imshow("image reduite", imreduite);

    //termine le programme lorsqu'une touche est frappee
    waitKey(0);
    return EXIT_SUCCESS;
}
