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
    int SREAL_HEIGHT = 89;    //灯实际高度mm
    int SREAL_WIDTH = 68;    //mm
    int SD = 511;//标准的标准
    float SPIX_HEIGHT = 159;//标准的标准
    float SLEFTD = 511;
    float SRIGHTD = 511;
    float SPIX_LEFT_HEIGHT = 159;
    float SPIX_RIGHT_HEIGHT = 159;
    int DELTA_HEIGHT = SPIX_LEFT_HEIGHT - SPIX_RIGHT_HEIGHT;
    float SPIX_LIGHT_WIDTH = SPIX_HEIGHT * ((float) SREAL_WIDTH / (float) SREAL_HEIGHT);
    float SINIT_ANGLE = 0;
    float SLEFTTOCENTER = -31;//$$$$$$$$$$$$$$$$$$
    float SRIGHTTOCENTER = 64;//$$$$$$$$$$$$$$$$$$
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
