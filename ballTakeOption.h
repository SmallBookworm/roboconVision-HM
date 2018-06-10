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
    int WIDTH = 640;
    int HEIGHT = 480;
    int SD = 242;
    int SLEFTD = 242;
    int SRIGHTD = 242;
    int SPIX_HEIGHT = 180;//灯高度（像素）
    int SPIX_LEFT_HEIGHT = 181;//左灯高度（像素）
    int SPIX_RIGHT_HEIGHT = 180;//右灯高度（像素）
    int DELTA_HEIGHT = SPIX_LEFT_HEIGHT - SPIX_RIGHT_HEIGHT;
    int SREAL_HEIGHT = 60;    //mm
    int SREAL_WIDTH = 70;    //mm
    float SPIX_LIGHT_WIDTH = SPIX_HEIGHT * ((float) SREAL_WIDTH / (float) SREAL_HEIGHT);//灯条外接矩形的宽度（像素）
    float SINIT_ANGLE = -1.2;
    float SLEFTTOCENTER = -75;//$$$$$$$$$$$$$$$$$$
    float SRIGHTTOCENTER = 132;//$$$$$$$$$$$$$$$$$$
    int AVG = 2;
    float angleThreshold = 0.5;
    float xThreshold = 0.5;
    float yThreshold = 0.5;
public:
    cv::Point2f upPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f downPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f centerPoint(int i, std::vector<cv::Vec4i> &a);

    int pixheight(int i, std::vector<cv::Vec4i> &a);

    float realdist(int i, std::vector<cv::Vec4i> &a);

    float surposepixWidth(int i, std::vector<cv::Vec4i> &a);
};


#endif //ROBOCONVISIONHM_BALLTAKEOPTION_H
