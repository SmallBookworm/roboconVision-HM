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
    int WIDTH = 640;
    int HEIGHT = 480;
    int SD = 364;//标准的标准
    float SPIX_LIGHT_HEIGHT = 188.5;//标准的标准
    float SLEFTD = 363.037;
    float SRIGHTD = 364.968;
    float SPIX_LEFT_HEIGHT = 189;
    float SPIX_RIGHT_HEIGHT = 188;
    int DELTA_HEIGHT = SPIX_LEFT_HEIGHT - SPIX_RIGHT_HEIGHT;
    int SREAL_HEIGHT = 96;    //mm
    int SREAL_WIDTH = 93;     //mm
    float SPIX_LIGHT_WIDTH = SPIX_LIGHT_HEIGHT * ((float) SREAL_WIDTH / (float) SREAL_HEIGHT);
    float SINIT_ANGLE = -1.0;
    float SLEFTTOCENTER = -76.5;//$$$$$$$$$$$$$$$$$$
    float SRIGHTTOCENTER = 107;//$$$$$$$$$$$$$$$$$$
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


#endif //LIGHTBAR_LINESOPTION_H
