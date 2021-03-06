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
    //normal ball
    int WIDTH = 640;
    int HEIGHT = 480;
    int SD = 534;
    float SLEFTD = 534;
    float SRIGHTD = 534;
    int SPIX_HEIGHT = 161;//灯高度（像素）
    int SPIX_LEFT_HEIGHT = 161;//左灯高度（像素）
    float SPIX_RIGHT_HEIGHT = 161;//右灯高度（像素）
    int DELTA_HEIGHT = SPIX_LEFT_HEIGHT - SPIX_RIGHT_HEIGHT;
    int SREAL_HEIGHT = 120;    //mm
    int SREAL_WIDTH = 60;    //mm
    float SPIX_LIGHT_WIDTH = SPIX_HEIGHT * ((float) SREAL_WIDTH / (float) SREAL_HEIGHT);//灯条外接矩形的宽度（像素）
    float SINIT_ANGLE = -0.4;
    float SLEFTTOCENTER = -11;//$$$$$$$$$$$$$$$$$$
    float SRIGHTTOCENTER = 68;//$$$$$$$$$$$$$$$$$$
    int AVG = 2;
    float angleThreshold = 1;
    float xThreshold = 5;
    float yThreshold = 5;
public:
    cv::Point2f upPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f downPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f centerPoint(int i, std::vector<cv::Vec4i> &a);

    int pixheight(int i, std::vector<cv::Vec4i> &a);

    float realdist(int i, std::vector<cv::Vec4i> &a);

    float surposepixWidth(int i, std::vector<cv::Vec4i> &a);
};


#endif //ROBOCONVISIONHM_BALLTAKEOPTION_H