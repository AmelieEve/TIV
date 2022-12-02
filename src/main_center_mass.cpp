//  #include <iostream>
//  using namespace std;

//  #include "opencv2/imgproc.hpp"
//  #include "opencv2/highgui.hpp"
//  using namespace cv;

//  #include "histogram.hpp"


//  Point center_of_mass(string name, Mat im, Rect r){
//  	Mat im_crop, thr, gray;

//      // crop image to general area
//  	im_crop = im(r);
//  	// imshow("cropped", im_crop);

//  	// convert image to grayscale
//  	cvtColor(im_crop, gray, COLOR_BGR2GRAY);
//  	// imshow("gray", gray);

//  	// convert grayscale to binary image
//  	threshold(gray, thr, 100, 255, THRESH_BINARY_INV);
//  	// imshow("thresh", thr);

//  	// find moments of the image
//  	Moments m = moments(thr,true);
//  	Point p(m.m10/m.m00, m.m01/m.m00);
//  	// coordinates of centroid
//  	cout<< Mat(p)<< endl;
//  	// show the image with a point mark at the centroid
//  	circle(im_crop, p, 5, Scalar(128,0,0), -1);
//  	imshow(name, im_crop);
//      Point ret(p.x + r.x, p.y + r.y);
//      return ret;
//  }

//  int main(void) {
//      //charge et affiche l'image
//      string imName = "../NicIcon/all-scans/00105.png";
//      // FIXME: Image 105 et 106: coins non reconnus
//      Mat im = imread(imName);
//      if(im.data == nullptr){
//          cerr << "Image not found: "<< imName << endl;
//          waitKey(0);
//          //system("pause");
//          exit(EXIT_FAILURE);
//      }

//      Point pl = center_of_mass("top right",   im, Rect(2120, 300, 250, 250));
//  	Point pr = center_of_mass("bottom left", im, Rect(100, 3100, 250, 250));


//  	int scale = 5;
//      Mat smol;
//  	resize(im, smol, Size(), 1.0/scale, 1.0/scale);
//  	// show the image with a point mark at the centroid
//  	circle(smol, pl/scale, 5, Scalar(128,0,0), -1);
//  	circle(smol, pr/scale, 5, Scalar(128,0,0), -1);
//  	imshow("Image with center", smol);

//      waitKey(0);
//  }