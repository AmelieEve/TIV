//
// Created by amelie on 02/12/22.
//

#include "opencv2/imgproc.hpp"
using namespace cv;
using namespace std;

#ifndef PROJET_OPENCV_CMAKE_EXTRACT_THUMBNAILS_HPP
#define PROJET_OPENCV_CMAKE_EXTRACT_THUMBNAILS_HPP

/*
 * Compute angle described by three given points
 */
double angle( cv::Point pt1, cv::Point pt2, cv::Point pt0 );

/*
 * Sort the 4 points that describe a square in the following order : upper left, upper right, lower left, lower right
 * Based upon the hypothesis that the squares' sides are almost horizontal and vertical (no rotated squares)
 */
vector<Point> sortCorners(vector<Point> square);

/*
 * Compute size of a square or rectangle described by 4 given points
 */
double sizeSquare(vector<Point> square);

/*
 * Compute center coordinates of a given square
 */
Point center(vector<Point> square);

/*
 * Detect all squares
 */
void detectSquares(const Mat& extractedLineImg, vector<vector<Point>>& squares);

/*
 * Draw the detected squares on the original subimage of extracted line of symbols
 */
Mat debugSquares( vector<vector<Point> >& squares, Mat image);

/*
 * Filter squares to keep only the 5 needed containing the drawn symbols
 */
void filterSquares(vector<vector<Point>>& squares, vector<vector<Point>>& filteredSquares);

/*
 * Create images cropped from the original subimage using the 5 squares' coordinates
 */
void extractThumbnails(const Mat& extractedLineImg, vector<vector<Point>>& filteredSquares, const string &filePrefix);

/**
 * Main function for that part of the pre-processing
 * @param extractedLineImg the cropped rectangle image containing a line of squares with drawn symbols
 * @param thumbnails
 */
void mainExtractThumbnails(const Mat &extractedLineImg, const string& filePrefix);

#endif //PROJET_OPENCV_CMAKE_EXTRACT_THUMBNAILS_HPP
