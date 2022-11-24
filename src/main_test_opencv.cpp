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
	string imName = "../Lenna.png";
	Mat im = imread(imName);
	if(im.data == nullptr){
		cerr << "Image not found: " << imName << endl;
		waitKey(0);
		//system("pause");
		exit(EXIT_FAILURE);
	}
	imshow("exemple1", im);

	//applique une reduction de taille d'un facteur 5
	//ici modifier pour ne reduire qu'a l'affichage 
	//comme demande dans l'enonce
	int reduction = 5;
	Size tailleReduite(im.cols/reduction, im.rows/reduction);
	Mat imreduite = Mat(tailleReduite,CV_8UC3); //cree une image � 3 canaux de profondeur 8 bits chacuns
	resize(im,imreduite,tailleReduite);
	imshow("image reduite", imreduite);

	computeHistogram("histogramme", im);


    Mat imhsv;
    cvtColor(im, imhsv, COLOR_RGB2HSV);
    Mat imh;
    Mat ims;
    Mat imv;
    extractChannel(imhsv, imh, 0);
    extractChannel(imhsv, ims, 1);
    extractChannel(imhsv, imv, 2);
    imshow("image hsv", imhsv);
    imshow("image h", imh);
    imshow("image s", ims);
    imshow("image v", imv);

    Mat imthresh;
    threshold(im, imthresh, 127, 255, THRESH_BINARY);
    imshow("threshold", imthresh);
    computeHistogram("histogramme", imthresh);

    Mat imroi = im(Rect(256, 256, 100, 100));
    imshow("roi", imroi);
    imwrite("lennas_eye.png", imroi);

	//termine le programme lorsqu'une touche est frappee
	waitKey(0);
	//system("pause");
	return EXIT_SUCCESS;
}
