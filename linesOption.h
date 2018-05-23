//
// Created by peng on 18-4-8.
//

#ifndef LIGHTBAR_LINESOPTION_H
#define LIGHTBAR_LINESOPTION_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <fstream>

const int WIDTH = 640;
const int HEIGHT = 480;
const int SD = 234;//标准的标准
const int SPIX_LIGHT_HEIGHT = 299;//标准的标准
const float SLEFTD = 234;
const float SRIGHTD = 232;
const float SPIX_LEFT_HEIGHT = 299;
const float SPIX_RIGHT_HEIGHT = 302;
const int DELTA_HEIGHT = SPIX_LEFT_HEIGHT - SPIX_RIGHT_HEIGHT;
const int SREAL_HEIGHT = 96;	//mm
const int SREAL_WIDTH = 93;	//mm
const float SPIX_LIGHT_WIDTH = SPIX_LIGHT_HEIGHT *((float)SREAL_WIDTH / (float)SREAL_HEIGHT);
const float SINIT_ANGLE = 1.3;
const float SLEFTTOCENTER = -54.5;//$$$$$$$$$$$$$$$$$$
const float SRIGHTTOCENTER = 238;//$$$$$$$$$$$$$$$$$$
const int AVG = 2;
const float angleThreshold = 0.5;
const float xThreshold = 0.5;
const float yThreshold = 0.5;

class LinesOption {
public:
    cv::Point2f upPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f downPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f centerPoint(int i, std::vector<cv::Vec4i> &a);

    int pixheight(int i, std::vector<cv::Vec4i> &a);

    float realdist(int i, std::vector<cv::Vec4i> &a);

    float surposepixWidth(int i, std::vector<cv::Vec4i> &a);
};


#endif //LIGHTBAR_LINESOPTION_H
