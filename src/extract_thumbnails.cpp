//
// Created by amelie on 02/12/22.
//

#include "extract_thumbnails.hpp"
#include <iostream>
#include "opencv2/highgui.hpp"
#include <fstream>
using namespace std;
using namespace cv;

double angle( Point pt1, Point pt2, Point pt0 ) {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

vector<Point> sortCorners(vector<Point> square){
    int distCornersThreshold = 10; //set the distance threshold to differentiate two corners
    vector<Point> sortedCornersSquare;
    Point upperLeft = square[0];
    Point upperRight, lowerLeft, lowerRight;

    //verify that the upper left corner is correct to take it as a reference
    for(const Point& p : square){
        if (upperLeft.x - p.x > 10 && upperLeft.y - p.y > 10) {upperLeft = p;}
    }

    //deduct which are the other corners
    for(const Point& p : square){
        if (abs(p.y - upperLeft.y) > distCornersThreshold) {
            if(abs(p.x - upperLeft.x) > distCornersThreshold){
                lowerRight = p;
            } else {
                lowerLeft = p;
            }
        } else {
            upperRight = p;
        }
    }
    sortedCornersSquare.push_back(upperLeft);
    sortedCornersSquare.push_back(upperRight);
    sortedCornersSquare.push_back(lowerLeft);
    sortedCornersSquare.push_back(lowerRight);
    return sortedCornersSquare;
}

double sizeSquare(vector<Point> square){
    return (abs(square[3].x - square[0].x) * abs(square[3].y - square[0].y)); // use coordinates of upper left and lower right corners
}

Point center(vector<Point> square){
    return {(square[0].x + square[3].x)/2, (square[0].y + square[3].y)/2};
}

void detectSquares(const Mat& extractedLineImg, vector<vector<Point>>& squares){
    Mat pyr, timg, gray0(extractedLineImg.size(), CV_8U), gray;

    int thresh = 50, N = 11;

    // down-scale and upscale the image to filter out the noise
    pyrDown(extractedLineImg, pyr, Size(extractedLineImg.cols/2, extractedLineImg.rows/2));
    pyrUp(pyr, timg, extractedLineImg.size());
    vector<vector<Point> > contours;

    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ ) {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ ) {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 ) {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            } else {
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

            vector<Point> approx;

            // test each contour
            for(const auto & contour : contours)
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contour), approx, arcLength(Mat(contour), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx))) {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        }
    }
}

Mat debugSquares( vector<vector<Point>>& squares, Mat image ){
    for ( int i = 0; i< squares.size(); i++ ) {
        // draw contour
        drawContours(image, squares, i, Scalar(255,0,0), 1, 8, vector<Vec4i>(), 0, Point());

        // draw bounding rect
        Rect rect = boundingRect(Mat(squares[i]));
        rectangle(image, rect.tl(), rect.br(), Scalar(0,255,0), 2, 8, 0);

        // draw rotated rect
        RotatedRect minRect = minAreaRect(Mat(squares[i]));
        Point2f rect_points[4];
        minRect.points( rect_points );
        for ( int j = 0; j < 4; j++ ) {
            line( image, rect_points[j], rect_points[(j+1)%4], Scalar(0,0,255), 1, 8 ); // blue
        }
    }
    return image;
}

void filterSquares(vector<vector<Point>>& squares, vector<vector<Point>>& filteredSquares){
    // Set the size and distance thresholds
    double minSizeThreshold = 60000;
    double maxSizeThreshold = 65000; //the 5 squares have a size a bit smaller than that
    double distanceThreshold = 60;

    // Filter the squares based on the size and distance thresholds
    for (const vector<Point>& square : squares) {
        // Sort corners : upper left, upper right, lower left, lower right
        // Allows to use properly size and center functions in the following to compare squares
        vector<Point> sortedSquare = sortCorners(square);
        double s = sizeSquare(sortedSquare);
        if (minSizeThreshold < s && s < maxSizeThreshold) { //this way we don't take into account the big rectangle contour neither the small squares that could be drawn in symbols
            bool isDuplicate = false;
            for (const vector<Point>& otherSquare : filteredSquares) {
                double distance = norm(center(sortedSquare) - center(otherSquare));
                if (distance < distanceThreshold) {
                    isDuplicate = true;
                    break;
                }
            }
            if (!isDuplicate) {
                filteredSquares.push_back(sortedSquare);
            }
        }
    }
}

void extractThumbnails(const Mat& extractedLineImg, vector<vector<Point>>& filteredSquares, const string &filePrefix, const string& sizeSymbol){
    int i = filteredSquares.size();
    if(filteredSquares.size() > 5){
        cerr << "More than 5 thumbnails extracted" << endl;
    }

    //reextract information from file prefix for text file
    string infos = filePrefix;
    infos.erase(0,10);
    string row = infos.substr(infos.size()-2, 1);
    string pageNumber = infos.substr(infos.size()-5, 2);
    string scripterNumber = infos.substr(infos.size()-9, 3);
    string formNumber = scripterNumber+pageNumber;
    string label = infos.substr(0, infos.size()-10);

    for(vector<Point> square : filteredSquares){
        string column = to_string(i);
        imwrite(filePrefix + column + ".png",extractedLineImg(Range(square[0].y, square[3].y), Range(square[0].x, square[3].x)));
        ofstream textFile(filePrefix + column + ".txt");
        textFile << "# projet TIV 4INFO 2022-2023 - option MI\n"
            << "# groupe : EVENO Amélie - HUMEAU Dorian - ROZELAAR Marceline - SOREL Maxime\n"
            << "label " << label << "\n"
            << "form " << formNumber << "\n"
            << "scripter " << scripterNumber << "\n"
            << "page " << pageNumber << "\n"
            << "row " << row << "\n"
            << "column " << column << "\n"
            << "size" << sizeSymbol << "\n"
            << endl;
        i--;
    }
}

void mainExtractThumbnails(const Mat &extractedLineImg, const string& filePrefix, const string& sizeSymbol) {
    vector<vector<Point>> squares;
    detectSquares(extractedLineImg, squares);
    {
//        Mat debugSquaresImg = debugSquares(squares, extractedLineImg);
//        imshow("debug detected squares", debugSquaresImg);
//        cout << "Total number of detected squares = " + to_string(squares.size()) << endl;
    }

    vector<vector<Point>> filteredSquares;
    filterSquares(squares, filteredSquares);
    {
//        Mat debugFilteredSquaresImg = debugSquares(filteredSquares, extractedLineImg);
//        imshow("debug filtered squares", debugFilteredSquaresImg);
    }
    cout << "Number of filtered squares = " + to_string(filteredSquares.size()) << " \n";
    {
//        for(const auto & square : filteredSquares){
//            cout << square << " \n";
//        }
//        cout << endl;
    }


    extractThumbnails(extractedLineImg, filteredSquares, filePrefix, sizeSymbol);
    {
//        for(int i = 0; i<filteredSquares.size(); i++){
//            string title = " n° " + to_string(filteredSquares.size()-i);
//            imshow(title, filteredSquares[i]);
//        }
    }

}