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


class BallTakeOption {
public:
    const static int WIDTH = 640;
    const static int HEIGHT = 480;
    const static int SD = 800;
    const static int SLEFTD = 796;
    const static int SRIGHTD = 804;
    const static int SPIX_LIGHT_HEIGHT = 138.5;
    const static int SPIX_LEFT_HEIGHT = 140;
    const static int SPIX_RIGHT_HEIGHT = 137;
    const static int DELTA_HEIGHT = SPIX_LEFT_HEIGHT - SPIX_RIGHT_HEIGHT;
    const static int SREAL_HEIGHT = 150;	//mm
    const static int SREAL_WIDTH = 210;	//mm
    constexpr static float SPIX_LIGHT_WIDTH = SPIX_LIGHT_HEIGHT *((float)SREAL_WIDTH / (float)SREAL_HEIGHT);//µÆÌõÍâ½Ó¾ØÐÎµÄ¿í¶È£¨ÏñËØ£
    constexpr static float SINIT_ANGLE = -4.7;
    constexpr static float SLEFTTOCENTER = -140;//$$$$$$$$$$$$$$$$$$
    constexpr static float SRIGHTTOCENTER = 83;//$$$$$$$$$$$$$$$$$$
    const static int AVG = 2;
    constexpr static float angleThreshold = 0.5;
    constexpr static float xThreshold = 0.5;
    constexpr static float yThreshold = 0.5;
public:
    cv::Point2f upPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f downPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f centerPoint(int i, std::vector<cv::Vec4i> &a);

    int pixheight(int i, std::vector<cv::Vec4i> &a);

    float realdist(int i, std::vector<cv::Vec4i> &a);

    float surposepixWidth(int i, std::vector<cv::Vec4i> &a);
};


#endif //ROBOCONVISIONHM_BALLTAKEOPTION_H
