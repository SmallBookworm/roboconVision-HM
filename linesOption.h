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
const int SD = 330;
const int SPIX_LIGHT_HEIGHT = 265.5;//灯条外接矩形的高度（像素）
const int SREAL_HEIGHT = 120;    //mm
const int SREAL_WIDTH = 120;    //mm
const float SPIX_LIGHT_WIDTH = SPIX_LIGHT_HEIGHT * ((float) SREAL_WIDTH / (float) SREAL_HEIGHT);//灯条外接矩形的宽度（像素）
const float SINIT_ANGLE = 0.00;
const float SLEFTTOCENTER = -133;//$$$$$$$$$$$$$$$$$$
const float SRIGHTTOCENTER = 126.5;//$$$$$$$$$$$$$$$$$$

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
