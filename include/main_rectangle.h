//
// Created by dorian on 02/01/23.
//

#ifndef PROJET_OPENCV_CMAKE_MAIN_RECTANGLE_H
#define PROJET_OPENCV_CMAKE_MAIN_RECTANGLE_H

#include <iostream>
#include <iomanip>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "extract_thumbnails.hpp"
#include "imageRectifier.h"

using namespace std;
using namespace cv;

std::string fixedDigitInt(int input, int digits);
std::string detectSize(Mat &subImage);
int guessRow(int y);
bool parse_page(const std::string &imName, int scripterNb, int pageNb);

#endif //PROJET_OPENCV_CMAKE_MAIN_RECTANGLE_H
