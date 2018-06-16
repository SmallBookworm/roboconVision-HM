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

class LinesOption {
public:
    //tz2
    int WIDTH = 640;
    int HEIGHT = 480;
    int SREAL_HEIGHT = 106;    //灯实际高度mm
    int SREAL_WIDTH = 93;    //mm
    int SD = 425;//标准的标准
    float SPIX_HEIGHT = 179;//标准的标准
    float SLEFTD = 425;
    float SRIGHTD = 427.4;
    float SPIX_LEFT_HEIGHT = 179;
    float SPIX_RIGHT_HEIGHT = 178;
    int DELTA_HEIGHT = SPIX_LEFT_HEIGHT - SPIX_RIGHT_HEIGHT;
    float SPIX_LIGHT_WIDTH = SPIX_HEIGHT * ((float) SREAL_WIDTH / (float) SREAL_HEIGHT);
    float SINIT_ANGLE = -1.2;
    float SLEFTTOCENTER = -87;//$$$$$$$$$$$$$$$$$$
    float SRIGHTTOCENTER = 71;//$$$$$$$$$$$$$$$$$$
    int AVG = 2;
    float angleThreshold = 1;
    float xThreshold = 7;
    float yThreshold = 2;
public:
    cv::Point2f upPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f downPoint(int i, std::vector<cv::Vec4i> &a);

    cv::Point2f centerPoint(int i, std::vector<cv::Vec4i> &a);

    int pixheight(int i, std::vector<cv::Vec4i> &a);

    float realdist(int i, std::vector<cv::Vec4i> &a);

    float surposepixWidth(int i, std::vector<cv::Vec4i> &a);
};


#endif //LIGHTBAR_LINESOPTION_H
