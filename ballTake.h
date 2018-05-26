//
// Created on 18-5-26.
//

#ifndef ROBOCONVISIONHM_BALLTAKE_H
#define ROBOCONVISIONHM_BALLTAKE_H

#include "ballTakeOption.h"
#include "lineInfo.h"
#include <fcntl.h>
#include <linux/videodev2.h>
#include<sys/ioctl.h>

class BallTake {
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
    BallTakeOption all_line;
    float info_value[3]{0};
    int num = 0;
    std::vector<float> angleRecord;
    std::vector<float> xRecord;
    std::vector<float> yRecord;
public:
    int watch(cv::Mat res);

    //define operator()
    int operator()(LineInfo &info);
};


#endif //ROBOCONVISIONHM_BALLTAKE_H
