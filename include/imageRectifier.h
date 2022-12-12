//
// Created by dorian on 09/12/22.
//

#ifndef PROJET_OPENCV_CMAKE_IMAGERECTIFIER_H
#define PROJET_OPENCV_CMAKE_IMAGERECTIFIER_H

#include <iostream>
#include <iomanip>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

bool rectify(std::string imName, cv::Mat &out);

#endif //PROJET_OPENCV_CMAKE_IMAGERECTIFIER_H
