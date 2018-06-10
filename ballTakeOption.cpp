//
// Created on 18-5-26.
//

#include "ballTakeOption.h"

using namespace std;
using namespace cv;

Point2f BallTakeOption::upPoint(int i, vector<Vec4i> &a) {
    Point2f p;
    p.x = a[i][2];
    p.y = a[i][3];
    return p;
}

Point2f BallTakeOption::downPoint(int i, vector<Vec4i> &a) {
    Point2f p;
    p.x = a[i][0];
    p.y = a[i][1];
    return p;
}

Point2f BallTakeOption::centerPoint(int i, vector<Vec4i> &a) {
    Point2f p;
    p.x = (a[i][0] + a[i][2]) / 2;
    p.y = (a[i][1] + a[i][3]) / 2;
    return p;
}

int BallTakeOption::pixheight(int i, vector<Vec4i> &a) {
    return a[i][1] - a[i][3];
}

float BallTakeOption::realdist(int i, vector<Vec4i> &a) {
    return (float) SD * SPIX_HEIGHT / (float) (a[i][1] - a[i][3]);//a[i][1] - a[i][3]:pixheight
}

float BallTakeOption::surposepixWidth(int i, vector<Vec4i> &a) {
    return (a[i][1] - a[i][3]) * ((float) SPIX_LIGHT_WIDTH / (float) SPIX_HEIGHT); //a[i][1] - a[i][3]:pixheight
}