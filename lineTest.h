//
// Created by peng on 18-4-8.
//

#ifndef LIGHTBAR_LINETEST_H
#define LIGHTBAR_LINETEST_H

#include "linesOption.h"
#include "lineInfo.h"
#include <fcntl.h>
#include <linux/videodev2.h>
#include<sys/ioctl.h>
#include <zconf.h>

class LineTest {
private:
    bool is_nan(double dVal);

    void GetDiffImage(cv::Mat src1, cv::Mat dst);

    static bool comp(const cv::Vec4i &a, const cv::Vec4i &b);

    float RadianCalculate(float rleft, float rright);

    float rad(float ang);

    float angleProcessor(float newNum);//今日新加 5.1

    float xProcessor(float newNum);//5.1

    float yProcessor(float newNum);//5.1

    float AngleCalculate(float rleft, float rright);

    std::vector<cv::Vec4i> findCorner(cv::Mat dst);

    std::vector<float> analyse(cv::Mat paint,
                               std::vector<cv::Vec4i> lines);

    void drawDetectLines(cv::Mat &image, const std::vector<cv::Vec4i> &lines, cv::Scalar &color);

public:
    short positionInfo[3];
    LinesOption lineOption;
    float info_value[3]{0};
    int num = 0;
    std::vector<float> angleRecord;
    std::vector<float> xRecord;
    std::vector<float> yRecord;
    int cornerSize=200;
    int thresh=65;
    int maxHeight=80;
public:
    int watch(cv::Mat res);

    //define operator()
    int operator()(LineInfo &info);
};


#endif //LIGHTBAR_LINETEST_H
