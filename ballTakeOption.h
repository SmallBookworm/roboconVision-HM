//
// Created on 18-5-26.
//

#ifndef ROBOCONVISIONHM_BALLTAKEOPTION_H
#define ROBOCONVISIONHM_BALLTAKEOPTION_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <fstream>

const int WIDTH = 640;
const int HEIGHT = 480;
const int SD = 800;
const int SLEFTD = 796;
const int SRIGHTD = 804;
const int SPIX_LIGHT_HEIGHT = 138.5;
const int SPIX_LEFT_HEIGHT = 140;
const int SPIX_RIGHT_HEIGHT = 137;
const int DELTA_HEIGHT = SPIX_LEFT_HEIGHT - SPIX_RIGHT_HEIGHT;
const int SREAL_HEIGHT = 150;	//mm
const int SREAL_WIDTH = 210;	//mm
const float SPIX_LIGHT_WIDTH = SPIX_LIGHT_HEIGHT *((float)SREAL_WIDTH / (float)SREAL_HEIGHT);//µÆÌõÍâ½Ó¾ØÐÎµÄ¿í¶È£¨ÏñËØ£
const float SINIT_ANGLE = -4.7;
const float SLEFTTOCENTER = -140;//$$$$$$$$$$$$$$$$$$
const float SRIGHTTOCENTER = 83;//$$$$$$$$$$$$$$$$$$
const int AVG = 2;
const float angleThreshold = 0.5;
const float xThreshold = 0.5;
const float yThreshold = 0.5;

class BallTakeOption {
public:
    cv::Point2f upPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f downPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f centerPoint(int i, std::vector<cv::Vec4i> &a);

    int pixheight(int i, std::vector<cv::Vec4i> &a);

    float realdist(int i, std::vector<cv::Vec4i> &a);

    float surposepixWidth(int i, std::vector<cv::Vec4i> &a);
};


#endif //ROBOCONVISIONHM_BALLTAKEOPTION_H
