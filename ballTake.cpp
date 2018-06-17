//
// Created on 18-5-26.
//

#include "ballTake.h"

using namespace std;
using namespace cv;

/*
寻找光线色差，分离通道
*/
bool BallTake::is_nan(double dVal) {
    if (dVal == dVal) {
        return false;
    }
    return true;
}

void BallTake::GetDiffImage(Mat src1, Mat dst) {

    for (int i = 0; i < takeOption.HEIGHT; i++) {
        uchar *pData1 = src1.ptr<uchar>(i);
        uchar *pData3 = dst.ptr<uchar>(i);
        for (int j = 0; j < takeOption.WIDTH; j++) {

            pData3[j] = pData1[j];

        }
    }
}

bool BallTake::comp(const Vec4i &a, const Vec4i &b) {
    return a[0] < b[0];

}

float BallTake::RadianCalculate(float rleft, float rright) {
    float radian = asinf((float) (rleft - rright) / (float) takeOption.SREAL_WIDTH);
    return radian;
}

float BallTake::rad(float ang) {
    return (float) ang * M_PI / (float) 180;
}

float BallTake::angleProcessor(float newNum) {
    float ca = 0;
    int zeroNum = 0;
    int otherNum = 0;
    if (angleRecord.size() < 10) {
        angleRecord.push_back(newNum);
    } else {
        num = num % 10;
        angleRecord[num] = newNum;
        num++;
    }
    for (int o = 0; o < angleRecord.size(); o++) {
        if (angleRecord[o] == 0) {
            zeroNum++;
        } else {
            otherNum++;
        }
    }
    if (zeroNum > otherNum) {
        return 0;
    } else {
        for (int o = 0; o < angleRecord.size(); o++) {
            ca = angleRecord[o] + ca;
        }
        ca = ca / angleRecord.size();
        return ca;
    }
}

float BallTake::xProcessor(float newNum) {
    float ca = 0;
    int zeroNum = 0;
    int otherNum = 0;
    if (xRecord.size() < 10) {
        xRecord.push_back(newNum);
    } else {
        num = num % 10;
        xRecord[num] = newNum;
        num++;
    }
    for (int o = 0; o < xRecord.size(); o++) {
        if (xRecord[o] == 0) {
            zeroNum++;
        } else {
            otherNum++;
        }
    }
    if (zeroNum > otherNum) {
        return 0;
    } else {
        for (int o = 0; o < xRecord.size(); o++) {
            ca = xRecord[o] + ca;
        }
        ca = ca / xRecord.size();
        return ca;
    }
}

float BallTake::yProcessor(float newNum) {
    float ca = 0;
    int zeroNum = 0;
    int otherNum = 0;
    if (yRecord.size() < 10) {
        yRecord.push_back(newNum);
    } else {
        num = num % 10;
        yRecord[num] = newNum;
        num++;
    }
    for (int o = 0; o < yRecord.size(); o++) {
        if (yRecord[o] == 0) {
            zeroNum++;
        } else {
            otherNum++;
        }
    }
    if (zeroNum > otherNum) {
        return 0;
    } else {
        for (int o = 0; o < yRecord.size(); o++) {
            ca = yRecord[o] + ca;
        }
        ca = ca / yRecord.size();
        return ca;
    }
}

float BallTake::AngleCalculate(float rleft, float rright) {
    float radian = asinf((float) (rleft - rright) / (float) takeOption.SREAL_WIDTH);
    float angle = (float) radian * 180 / (float) M_PI;
    return angle;
}

vector<Vec4i> BallTake::findCorner(Mat dst) {
    vector<vector<Point>> contours;
    vector<Vec4i> lines;
    int greaterC = 0;
    int greaterCD = 0;
    cv::findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    if (contours.size() == 0) {
        return lines;
    }
    Mat imageContours = Mat::zeros(dst.size(), CV_8UC1); //最小外接矩形画布
    for (int i = 0; i < contours.size(); i++) {
        //Point2f P[4];
        if (contours[i].capacity() > greaterC) {
            greaterCD = i;
            greaterC = contours[i].capacity();
        }
    }
    Vec4i leftLine;
    Vec4i rightLine;

    int rightDown = 0;
    int rightDownD = 0;
    int leftUp = takeOption.WIDTH * takeOption.HEIGHT;
    int leftUpD = 0;

    int leftDown = 0;
    int leftDownD = 0;
    int rightUp = 0;
    int rightUpD = 0;

    for (int j = 0; j < contours[greaterCD].size(); j++) {
        if (contours[greaterCD][j].x * contours[greaterCD][j].y > rightDown) {
            rightDownD = j;
            rightDown = contours[greaterCD][j].x * contours[greaterCD][j].y;
        }
        if (contours[greaterCD][j].x * contours[greaterCD][j].y < leftUp) {
            leftUpD = j;
            leftUp = contours[greaterCD][j].x * contours[greaterCD][j].y;
        }
        if (contours[greaterCD][j].x * (takeOption.HEIGHT - contours[greaterCD][j].y) > rightUp) {
            rightUpD = j;
            rightUp = contours[greaterCD][j].x * (takeOption.HEIGHT - contours[greaterCD][j].y);
        }
        if ((takeOption.WIDTH - contours[greaterCD][j].x) * contours[greaterCD][j].y > leftDown) {
            leftDownD = j;
            leftDown = (takeOption.WIDTH - contours[greaterCD][j].x) * contours[greaterCD][j].y;
        }

    }

    leftLine[0] = contours[greaterCD][leftDownD].x;
    leftLine[1] = contours[greaterCD][leftDownD].y;
    leftLine[2] = contours[greaterCD][leftUpD].x;
    leftLine[3] = contours[greaterCD][leftUpD].y;
    rightLine[0] = contours[greaterCD][rightDownD].x;
    rightLine[1] = contours[greaterCD][rightDownD].y;
    rightLine[2] = contours[greaterCD][rightUpD].x;
    rightLine[3] = contours[greaterCD][rightUpD].y;

    int testLH = leftLine[1] - leftLine[3];
    int testRH = rightLine[1] - rightLine[3];
    int testW = rightLine[0] - leftLine[0];
    if (testLH > 80 && testRH > 80 && (testLH > 1.5 * testW) && (testRH > 1.5 * testW)) {
        lines.push_back(leftLine);
        lines.push_back(rightLine);
    }
    sort(lines.begin(), lines.end(), comp);
    //for (int i = 0; i < lines.size(); i++) {
    //	for (int j = 0; j < 4; j++) {
    //		lines[i][j] = simple(lines[i][j], i * 4 + j);
    //	}
    //}

    return lines;
}


vector<float> BallTake::analyse(Mat paint, vector<Vec4i> lines) {
    BallTakeOption left_line;
    BallTakeOption right_line;
    float angle;
    float x;
    float y;
    vector<float> all_data;
    float leftToCenter = left_line.centerPoint(0, lines).x - takeOption.WIDTH / 2;//pix
    float rightToCenter = right_line.centerPoint(1, lines).x - takeOption.WIDTH / 2;//pix
    float pix_left_height = left_line.pixheight(0, lines);
    float pix_right_height = right_line.pixheight(1, lines);
    //判断超出的情况
    if ((left_line.upPoint(0, lines).x == 0 && left_line.downPoint(0, lines).x < 7) ||
        (left_line.upPoint(0, lines).x < 7 && left_line.downPoint(0, lines).x == 0))//******有参数,标志物向左出界
    {
        angle = 0;
        float guessX = -takeOption.SREAL_HEIGHT *
                       (((takeOption.WIDTH / 2 +
                          (pix_right_height * takeOption.SREAL_WIDTH) / (float) (2 * takeOption.SREAL_HEIGHT))
                         - right_line.centerPoint(1, lines).x)
                        / (float) pix_right_height);
        float guessY = 0;
        all_data.push_back(angle);
        all_data.push_back(guessX);
        all_data.push_back(guessY);
    }
    if ((right_line.upPoint(0, lines).x == takeOption.WIDTH &&
         right_line.downPoint(0, lines).x > (takeOption.WIDTH - 7)) ||
        (right_line.upPoint(0, lines).x > (takeOption.WIDTH - 7) &&
         right_line.downPoint(0, lines).x == takeOption.WIDTH)) {
        angle = 0;
        float guessX = -takeOption.SREAL_HEIGHT *
                       (((takeOption.WIDTH / 2 -
                          (pix_left_height * takeOption.SREAL_WIDTH) / (float) (2 * takeOption.SREAL_HEIGHT))
                         - left_line.centerPoint(0, lines).x)
                        / (float) pix_left_height);
        float guessY = 0;
        all_data.push_back(angle);
        all_data.push_back(guessX);
        all_data.push_back(guessY);
    } else//正常情况
    {
        float real_left_D = left_line.realdist(0, lines);//左灯条左边的真实距离
        float real_right_D = right_line.realdist(1, lines);//右灯条右边的真实距离
        //求角度
        float pic_angle = AngleCalculate(real_left_D, real_right_D);
        angle = pic_angle - takeOption.SINIT_ANGLE;
        float radian = rad(angle);
        int r = 2;
        if (pix_left_height - pix_right_height > takeOption.DELTA_HEIGHT) {
            float real_delta_x =
                    (float) takeOption.SREAL_HEIGHT *
                    ((float) leftToCenter / (float) left_line.pixheight(0, lines) -
                     (float) takeOption.SLEFTTOCENTER /
                     (float) takeOption.SPIX_LEFT_HEIGHT);//#########改过
            float real_delta_d = real_left_D - takeOption.SLEFTD;
            float a[2] = {real_delta_x, real_delta_d};
            Mat av = Mat(2, 1, CV_32FC1, a);
            //s_left_x,s_left_dis
            float b[2] = {
                    takeOption.SREAL_HEIGHT * takeOption.SLEFTTOCENTER /
                    (float) takeOption.SPIX_LEFT_HEIGHT,
                    takeOption.SLEFTD};
            Mat bv = Mat(2, 1, CV_32FC1, b);
            float rotate[4] = {cosf(-radian), -sinf(-radian), sinf(-radian), cosf(-radian)};
            Mat rotatev = Mat(2, 2, CV_32FC1, rotate);
            Mat cv = rotatev * bv;
            Mat dv = bv + av - cv;
            float lc[2] = {0, 240};//*****************************
            Mat locateToCamera1 = Mat(2, 1, CV_32FC1, lc);
            Mat locateToCamera2 = rotatev * locateToCamera1;
            Mat locateToLocate = locateToCamera1 + dv - locateToCamera2;
            float *data1 = locateToLocate.ptr<float>(0);
            x = data1[0];
            y = data1[1];
            all_data.push_back(angle);
            all_data.push_back(x);
            all_data.push_back(y);
        }
        if (pix_left_height - pix_right_height <= takeOption.DELTA_HEIGHT) {
            float real_delta_x =
                    (float) takeOption.SREAL_HEIGHT *
                    ((float) rightToCenter / (float) right_line.pixheight(1, lines) -
                     (float) takeOption.SRIGHTTOCENTER /
                     (float) takeOption.SPIX_RIGHT_HEIGHT);
            float real_delta_d = real_right_D - takeOption.SRIGHTD;
            float a[2] = {real_delta_x, real_delta_d};
            Mat av = Mat(2, 1, CV_32FC1, a);
            float b[2] = {takeOption.SREAL_HEIGHT * takeOption.SRIGHTTOCENTER /
                          (float) takeOption.SPIX_RIGHT_HEIGHT, takeOption.SRIGHTD};
            Mat bv = Mat(2, 1, CV_32FC1, b);
            float rotate[4] = {cosf(-radian), -sinf(-radian), sinf(-radian), cosf(-radian)};
            Mat rotatev = Mat(2, 2, CV_32FC1, rotate);
            Mat cv = rotatev * bv;
            Mat dv = bv + av - cv;
            float lc[2] = {0, 240};//*****************************
            Mat locateToCamera1 = Mat(2, 1, CV_32FC1, lc);
            Mat locateToCamera2 = rotatev * locateToCamera1;
            Mat locateToLocate = locateToCamera1 + dv - locateToCamera2;
            float *data1 = locateToLocate.ptr<float>(0);
            x = data1[0];
            y = data1[1];
            all_data.push_back(angle);
            all_data.push_back(x);
            all_data.push_back(y);
        }
    }
    return all_data;
}


void BallTake::drawDetectLines(Mat &image, const vector<Vec4i> &lines, Scalar &color) {
    // 将检测到的直线在图上画出来
    vector<Vec4i>::const_iterator it = lines.begin();
    while (it != lines.end()) {
        Point pt1((*it)[0], (*it)[1]);
        Point pt2((*it)[2], (*it)[3]);
        line(image, pt1, pt2, color, 2); //  线条宽度设置为2
        ++it;
    }
}

int BallTake::watch(cv::Mat src) {
    Mat pBinary, record, dst;
    dst = Mat::zeros(Size(takeOption.WIDTH, takeOption.HEIGHT), CV_8UC1);
    vector<Mat> mv;
    vector<Vec4i> lines;
    vector<vector<float>> dateRecord;

    split(src, mv);
    GetDiffImage(mv[1], dst);

    //先膨胀，后腐蚀（联通区域）
    threshold(dst, dst, 40, 255, THRESH_BINARY);

    //GaussianBlur(dst, dst, Size(3, 3), 0, 0);
    //imshow("eee",dst);
    //得到角点
    lines = findCorner(dst);
    vector<float> data;
    vector<float> data_filter1;
    vector<float> data_final;
    if (lines.size() == 2) {
        Scalar sca = Scalar(0, 0, 255);
        drawDetectLines(src, lines, sca);
        data = analyse(src, lines);
        if (abs(data[0]) < takeOption.angleThreshold) {
            data[0] = 0;
        }
        if (abs(data[1]) < takeOption.xThreshold) {
            data[1] = 0;
        }
        if (abs(data[2]) < takeOption.yThreshold) {
            data[2] = 0;
        }
        data_filter1.push_back(angleProcessor(data[0]));
        data_filter1.push_back(xProcessor(data[1]));
        data_filter1.push_back(yProcessor(data[2]));
        float vectRadian = atan2f(data_filter1[2], data_filter1[1]);
        float vectAngle = 180 * vectRadian / (float) M_PI;
        float vectLength = sqrtf(powf(data_filter1[1], 2) + powf(data_filter1[2], 2));
        if (vectLength < 10) {
            vectLength = 0;
        }
        data_final.push_back(data_filter1[0]);
        data_final.push_back(vectAngle);
        data_final.push_back(vectLength);

        cout << "angle: " << data_final[0] << endl;
        cout << "vectAngle: " << data_final[1] << endl;
        cout << "vectLength: " << data_final[2] << endl;
        info_value[0] = data_final[2];
        info_value[1] = data_final[1];
        info_value[2] = data_final[0];
    } else if (lines.size() < 2) {
        cout << "invalid " << lines.size() << endl;
    } else if (lines.size() > 2) {
        //imshow("eeee",src);
        //waitKey(0);
        cout << "invalid " << lines.size() << endl;
    }
    //waitKey(0);
    return static_cast<int>(lines.size());
}

int BallTake::operator()(LineInfo &info) {
    cout << "take" << endl;
    //system("v4l2-ctl --set-ctrl=exposure_auto=1 -d /dev/video1");

    VideoCapture capture("/dev/video0");
    //capture.open("/home/peng/下载/realse/1.avi");

    int fd = open("/dev/video0", O_RDWR);
    if (fd >= 0) {
        struct v4l2_control ctrl;
        ctrl.id = V4L2_CID_EXPOSURE_AUTO;
        ctrl.value = V4L2_EXPOSURE_MANUAL;
        int ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl);

        struct v4l2_control ctrl1;
        ctrl1.id = V4L2_CID_EXPOSURE_ABSOLUTE;
        ctrl1.value = 10000;
        ret = ioctl(fd, VIDIOC_S_CTRL, &ctrl1);
        if (ret < 0) {
            printf("Get exposure failed (%d)\n", ret);
        } else
            printf("\nGet Exposure :[%d]\n", ctrl1.value);
    }
    //close when we will not used
    close(fd);

    Mat srcImage;
    if (!capture.isOpened()) {
        std::cout << "fail to open video!" << std::endl;
        info.setThreadState(false);
        return -1;
    }

    short positionInfo[3];
    info.getPositionInfo(positionInfo);
    if (positionInfo[2] == 1) {
        //golden ball
        takeOption.WIDTH = 640;
        takeOption.HEIGHT = 480;
        takeOption.SD = 384;
        takeOption.SLEFTD = 384;
        takeOption.SRIGHTD = 384;
        takeOption.SPIX_HEIGHT = 116;//灯高度（像素）
        takeOption.SPIX_LEFT_HEIGHT = 116;//左灯高度（像素）
        takeOption.SPIX_RIGHT_HEIGHT = 116;//右灯高度（像素）
        takeOption.DELTA_HEIGHT = takeOption.SPIX_LEFT_HEIGHT - takeOption.SPIX_RIGHT_HEIGHT;
        takeOption.SREAL_HEIGHT = 120;    //mm
        takeOption.SREAL_WIDTH = 60;    //mm
        takeOption.SPIX_LIGHT_WIDTH = takeOption.SPIX_HEIGHT *
                                      ((float) takeOption.SREAL_WIDTH / (float) takeOption.SREAL_HEIGHT);//灯条外接矩形的宽度（像素）
        takeOption.SINIT_ANGLE = -0.4;
        takeOption.SLEFTTOCENTER = -7.9;//$$$$$$$$$$$$$$$$$$
        takeOption.SRIGHTTOCENTER = 48.9;//$$$$$$$$$$$$$$$$$$
        takeOption.AVG = 2;
        takeOption.angleThreshold = 1;
        takeOption.xThreshold = 5;
        takeOption.yThreshold = 5;
    }
    bool status = info.getStop();
    while (!status) {
        if (!capture.isOpened())
            break;
        capture >> srcImage;
        if (srcImage.empty())
            break;
        int size = watch(srcImage);
        //test
        //cout << size << endl;
        if (size == 2 && !isnan(info_value[0]) && !isnan(info_value[1]) && !isnan(info_value[2])) {
            info.set(info_value);
        }
        //test
//        imshow("show", srcImage);
//        if (waitKey(1) == 27) {
//            break;
//        }
        status = info.getStop();
    };
    capture.release();
    info.setThreadState(false);
    return 0;
}