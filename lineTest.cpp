//
// Created by peng on 18-4-8.
//

#include "lineTest.h"

using namespace std;
using namespace cv;

/*
寻找光线色差，分离通道
*/
bool LineTest::is_nan(double dVal) {
    if (dVal == dVal) {
        return false;
    }
    return true;
}

void LineTest::GetDiffImage(Mat src1, Mat dst) {

    for (int i = 0; i < lineOption.HEIGHT; i++) {
        uchar *pData1 = src1.ptr<uchar>(i);
        uchar *pData3 = dst.ptr<uchar>(i);
        for (int j = 0; j < lineOption.WIDTH; j++) {

            pData3[j] = pData1[j];

        }
    }
}

bool LineTest::comp(const Vec4i &a, const Vec4i &b) {
    return a[0] < b[0];

}

float LineTest::RadianCalculate(float rleft, float rright) {
    float radian = asinf((float) (rleft - rright) / (float) lineOption.SREAL_WIDTH);
    return radian;
}

float LineTest::rad(float ang) {
    return (float) ang * M_PI / (float) 180;
}

float LineTest::angleProcessor(float newNum) {
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

float LineTest::xProcessor(float newNum) {
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

float LineTest::yProcessor(float newNum) {
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

float LineTest::AngleCalculate(float rleft, float rright) {
    float radian = asinf((float) (rleft - rright) / (float) lineOption.SREAL_WIDTH);
    float angle = (float) radian * 180 / (float) M_PI;
    return angle;
}

vector<Vec4i> LineTest::findCorner(Mat dst) {
    vector<vector<Point> > contours;
    vector<Vec4i> lines;

    cv::findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    Mat imageContours = Mat::zeros(dst.size(), CV_8UC1);
    for (int i = 0; i < contours.size(); i++) {
        Point2f P[4];

        if (contours[i].size() > cornerSize) //change according to fps
        {
            Vec4i leftLine;
            Vec4i rightLine;
            int averageRightDownY = 0;
            int averageRightDownNumY = 0;

            int averageLeftUpY = 0;
            int averageLeftUpNumY = 0;

            int averageLeftDownY = 0;
            int averageLeftDownNumY = 0;

            int averageRightUpY = 0;
            int averageRightUpNumY = 0;


            int rightDown = 0;
            int rightDownD = 0;
            int leftUp = lineOption.WIDTH * lineOption.HEIGHT;
            int leftUpD = 0;

            int leftDown = 0;
            int leftDownD = 0;
            int rightUp = 0;
            int rightUpD = 0;
            //find 4 max or min points
            for (int j = 0; j < contours[i].size(); j++) {
                if (contours[i][j].x * contours[i][j].y > rightDown) {
                    rightDownD = j;
                    rightDown = contours[i][j].x * contours[i][j].y;
                }
                if (contours[i][j].x * contours[i][j].y < leftUp) {
                    leftUpD = j;
                    leftUp = contours[i][j].x * contours[i][j].y;
                }
                if (contours[i][j].x * (lineOption.HEIGHT - contours[i][j].y) > rightUp) {
                    rightUpD = j;
                    rightUp = contours[i][j].x * (lineOption.HEIGHT - contours[i][j].y);
                }
                if ((lineOption.WIDTH - contours[i][j].x) * contours[i][j].y > leftDown) {
                    leftDownD = j;
                    leftDown = (lineOption.WIDTH - contours[i][j].x) * contours[i][j].y;
                }

            }
            //maek sure corner by average points around the found 4 points
            for (int j = 0; j < contours[i].size(); j++) {
                if (contours[i][j].y > (contours[i][rightDownD].y - lineOption.AVG) &&
                    contours[i][j].y < (contours[i][rightDownD].y + lineOption.AVG)) {
                    averageRightDownY += contours[i][j].y;
                    averageRightDownNumY++;
                }
                if (contours[i][j].y > (contours[i][leftUpD].y - lineOption.AVG) &&
                    contours[i][j].y < (contours[i][leftUpD].y + lineOption.AVG)) {
                    averageLeftUpY += contours[i][j].y;
                    averageLeftUpNumY++;
                }
                if (contours[i][j].y > (contours[i][leftDownD].y - lineOption.AVG) &&
                    contours[i][j].y < (contours[i][leftDownD].y + lineOption.AVG)) {
                    averageLeftDownY += contours[i][j].y;
                    averageLeftDownNumY++;
                }
                if (contours[i][j].y > (contours[i][rightUpD].y - lineOption.AVG) &&
                    contours[i][j].y < (contours[i][rightUpD].y + lineOption.AVG)) {
                    averageRightUpY += contours[i][j].y;
                    averageRightUpNumY++;
                }
            }
            averageRightDownY = averageRightDownY / averageRightDownNumY;
            averageLeftUpY = averageLeftUpY / averageLeftUpNumY;
            averageLeftDownY = averageLeftDownY / averageLeftDownNumY;
            averageRightUpY = averageRightUpY / averageRightUpNumY;
            RotatedRect rect = minAreaRect(contours[i]);
            rect.points(P);

            float tempHigh1 = 0, tempLow1 = 0;
            for (int j = 0; j <= 3; j++)//经验值
            {
                if (P[j].y > lineOption.HEIGHT - 5 || P[j].y < 5 || P[j].x > lineOption.WIDTH - 5 || P[j].x < 5) {
                    vector<Vec4i> nulllines;
                    return nulllines;
                }
                //line(src, P[j], P[(j + 1) % 4], Scalar(255), 2);
                if (P[j].y > rect.center.y) {
                    if (tempHigh1 == 0) {
                        tempHigh1 = P[j].x;
                    }
                    if (tempHigh1 > P[j].x) {
                        leftLine[0] = P[j].x + 1;
                        leftLine[1] = averageLeftDownY;
                        rightLine[0] = tempHigh1 + 1;
                        rightLine[1] = averageRightDownY;
                    }
                    if (tempHigh1 < P[j].x) {
                        rightLine[0] = P[j].x + 1;
                        rightLine[1] = averageLeftDownY;
                        leftLine[0] = tempHigh1 - 1;
                        leftLine[1] = averageRightDownY;
                    }
                }


                if (P[j].y < rect.center.y) {
                    if (tempLow1 == 0) {
                        tempLow1 = P[j].x;
                    }
                    if (tempLow1 > P[j].x) {
                        leftLine[2] = P[j].x + 1;
                        leftLine[3] = averageLeftUpY;
                        rightLine[2] = tempLow1 - 1;
                        rightLine[3] = averageRightUpY;
                    }
                    if (tempLow1 < P[j].x) {
                        rightLine[2] = P[j].x - 1;
                        rightLine[3] = averageRightUpY;
                        leftLine[2] = tempLow1 + 1;
                        leftLine[3] = averageLeftUpY;
                    }
                }
            }
            int testLH = leftLine[1] - leftLine[3];
            int testRH = rightLine[1] - rightLine[3];
            int testW = rightLine[0] - leftLine[0];
            if (testLH > minHeight && testRH > minHeight && (testLH > 5 * testW) && (testRH > 5 * testW)) {
                lines.push_back(leftLine);
                lines.push_back(rightLine);
            }
        }
    }
    //x from small to big
    sort(lines.begin(), lines.end(), comp);
    return lines;
}

vector<float> LineTest::analyse(Mat paint, vector<Vec4i> lines) {
    LinesOption left_line;
    LinesOption right_line;
    vector<float> all_data;
    float pix_left_height = left_line.pixheight(0, lines);
    float pix_right_height = right_line.pixheight(3, lines);
    float real_left_D = left_line.realdist(0, lines);//左灯条左边的真实距离
    float real_right_D = right_line.realdist(3, lines);//右灯条右边的真实距离
    //cout << "sld" <<real_left_D<<endl;
    //cout << "srd" <<real_right_D << endl;
    //求角度
    float pic_angle = AngleCalculate(real_left_D, real_right_D);
    float angle = pic_angle - lineOption.SINIT_ANGLE;
    float radian = rad(angle);
    float leftToCenter = left_line.centerPoint(0, lines).x - lineOption.WIDTH / 2;//pix
    float rightToCenter = right_line.centerPoint(3, lines).x - lineOption.WIDTH / 2;//pix
    int r = 2;
    if (pix_left_height - pix_right_height > lineOption.DELTA_HEIGHT) {
        float real_delta_x =
                (float) lineOption.SREAL_HEIGHT * ((float) leftToCenter / (float) left_line.pixheight(0, lines) -
                                                   (float) lineOption.SLEFTTOCENTER /
                                                   (float) lineOption.SPIX_LEFT_HEIGHT);//#########改过
        float real_delta_d = real_left_D - lineOption.SLEFTD;
        float a[2] = {real_delta_x, real_delta_d};
        Mat av = Mat(2, 1, CV_32FC1, a);
        //s_left_x,s_left_dis
        float b[2] = {lineOption.SREAL_HEIGHT * lineOption.SLEFTTOCENTER / (float) lineOption.SPIX_LEFT_HEIGHT,
                      lineOption.SLEFTD};
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
        float x = data1[0];
        float y = data1[1];
        all_data.push_back(angle);
        all_data.push_back(x);
        all_data.push_back(y);
    }
    if (pix_left_height - pix_right_height <= lineOption.DELTA_HEIGHT) {
        float real_delta_x =
                (float) lineOption.SREAL_HEIGHT * ((float) rightToCenter / (float) right_line.pixheight(3, lines) -
                                                   (float) lineOption.SRIGHTTOCENTER /
                                                   (float) lineOption.SPIX_RIGHT_HEIGHT);
        float real_delta_d = real_right_D - lineOption.SRIGHTD;
        float a[2] = {real_delta_x, real_delta_d};
        Mat av = Mat(2, 1, CV_32FC1, a);
        float b[2] = {lineOption.SREAL_HEIGHT * lineOption.SRIGHTTOCENTER / (float) lineOption.SPIX_RIGHT_HEIGHT,
                      lineOption.SRIGHTD};
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
        float x = data1[0];
        float y = data1[1];
        all_data.push_back(angle);
        all_data.push_back(x);
        all_data.push_back(y);
    }
    /*调参数用*/
    //    all_data.push_back(pix_left_height);
    //    all_data.push_back(pix_right_height);
    //    all_data.push_back(leftToCenter);
    //    all_data.push_back(rightToCenter);
    //namedWindow("final", 0);
    //imshow("final", paint);
    //waitKey(10);//&&&&&&&&&&&&&&&&&&
    return all_data;
}


void LineTest::drawDetectLines(Mat &image, const vector<Vec4i> &lines, Scalar &color) {
    // 将检测到的直线在图上画出来
    vector<Vec4i>::const_iterator it = lines.begin();
    while (it != lines.end()) {
        Point pt1((*it)[0], (*it)[1]);
        Point pt2((*it)[2], (*it)[3]);
        line(image, pt1, pt2, color, 2); //  线条宽度设置为2
        ++it;
    }
}

int LineTest::watch(cv::Mat src) {
    Mat pBinary, record, dst;
    dst = Mat::zeros(Size(lineOption.WIDTH, lineOption.HEIGHT), CV_8UC1);
    vector<Mat> mv;
    vector<Vec4i> lines;
    Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
    Mat elementE = getStructuringElement(MORPH_RECT, Size(9, 9));
    Mat elementC = getStructuringElement(MORPH_RECT, Size(3, 3));
    vector<vector<float>> dateRecord;

    split(src, mv);
    GetDiffImage(mv[1], dst);

    //先膨胀，后腐蚀（联通区域）
    threshold(dst, dst, thresh, 255, THRESH_BINARY);

    //GaussianBlur(dst, dst, Size(3, 3), 0, 0);
    //imshow("eee",dst);
    //得到角点
    lines = findCorner(dst);
    vector<float> data;
    vector<float> data_filter1;
    vector<float> data_final;
    if (lines.size() == 4) {
        Scalar sca = Scalar(0, 0, 255);
        drawDetectLines(src, lines, sca);
        data = analyse(src, lines);
        if (abs(data[0]) < lineOption.angleThreshold) {
            data[0] = 0;
        }
        if (abs(data[1]) < lineOption.xThreshold) {
            data[1] = 0;
        }
        if (abs(data[2]) < lineOption.yThreshold) {
            data[2] = 0;
        }
        data_filter1.push_back(angleProcessor(data[0]));
        data_filter1.push_back(xProcessor(data[1]));
        data_filter1.push_back(yProcessor(data[2]));
        float vectRadian = atan2f(data_filter1[2], data_filter1[1]);
        float vectAngle = 180 * vectRadian / (float) M_PI;
        float vectLength = sqrtf(powf(data_filter1[1], 2) + powf(data_filter1[2], 2));
        if (abs(vectLength) < 10) {
            vectLength = 0;
        }
        if (vectLength == 0 && data_filter1[0] == 0) {
            vectAngle = 0;
        }

        data_final.push_back(data_filter1[0]);
        data_final.push_back(vectAngle);
        data_final.push_back(vectLength);

//        cout << "angle: " << data_final[0] << endl;
//        cout << "vectAngle: " << data_final[1] << endl;
//        cout << "vectLength: " << data_final[2] << endl;
        info_value[0] = data_final[2];
        info_value[1] = data_final[1];
        info_value[2] = data_final[0];
    } else if (lines.size() < 4) {
//        cout << "invalid " << lines.size() << endl;
    } else if (lines.size() > 4) {
        //imshow("eeee",src);
        //waitKey(0);
//        cout << "invalid " << lines.size() << endl;
    }
    //waitKey(0);
    return static_cast<int>(lines.size());
}

int LineTest::operator()(LineInfo &info) {
    cout << "dock" << endl;
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
        ctrl1.value = 1500;
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

    info.getPositionInfo(positionInfo);
    //make sure zone
    if (positionInfo[2] == -1) {
        cout << "TZ1" << endl;
        cornerSize = 120;
        thresh = 57;
        minHeight = 80;
        lineOption.WIDTH = 640;
        lineOption.HEIGHT = 480;
        lineOption.SD = 566;//标准的标准
        lineOption.SPIX_HEIGHT = 143;//标准的标准
        lineOption.SLEFTD = 566;
        lineOption.SRIGHTD = 566;
        lineOption.SPIX_LEFT_HEIGHT = 143;
        lineOption.SPIX_RIGHT_HEIGHT = 143;
        lineOption.DELTA_HEIGHT = lineOption.SPIX_LEFT_HEIGHT - lineOption.SPIX_RIGHT_HEIGHT;
        lineOption.SREAL_HEIGHT = 113;    //mm
        lineOption.SREAL_WIDTH = 68;    //mm
        lineOption.SPIX_LIGHT_WIDTH =
                lineOption.SPIX_HEIGHT * ((float) lineOption.SREAL_WIDTH / (float) lineOption.SREAL_HEIGHT);
        lineOption.SINIT_ANGLE = 0;
        lineOption.SLEFTTOCENTER = -67;//$$$$$$$$$$$$$$$$$$
        lineOption.SRIGHTTOCENTER = 18;//$$$$$$$$$$$$$$$$$$
        lineOption.AVG = 2;
        lineOption.angleThreshold = 1;
        lineOption.xThreshold = 7;
        lineOption.yThreshold = 2;
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
        if (size == 4 && !isnan(info_value[0]) && !isnan(info_value[1]) && !isnan(info_value[2])) {
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
