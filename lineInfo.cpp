//
// Created by peng on 18-4-8.
//
#include "lineInfo.h"

using namespace std;

int LineInfo::get(float *res) {
    lock_guard<mutex> l(info_mutex);
    if (used) {
        return -1;
    } else {
        res[0] = dModule;
        res[1] = argument;
        res[2] = relativeAngle;
        used = true;
        return 1;
    }
}

void LineInfo::set(float *value) {
    lock_guard<mutex> l(info_mutex);
    dModule = value[0];
    argument = value[1];
    relativeAngle = value[2];
    used = false;
}

bool LineInfo::getStop() {
    lock_guard<mutex> l(stop_mutex);
    return stop;
}

void LineInfo::setStop(bool f) {
    lock_guard<mutex> l(stop_mutex);
    stop = f;
}

void LineInfo::init(short x, short y, short angle) {
    lock_guard<mutex> l(info_mutex);
    lock_guard<mutex> s(stop_mutex);
    lock_guard<mutex> k(ts_mutex);
    lock_guard<mutex> o(position_mutex);
    used = true;
    threadState = true;
    stop = false;
    this->x = x;
    this->y = y;
    this->angle = angle;
}

void LineInfo::getPositionInfo(short (&value)[3]) {
    value[0] = x;
    value[1] = y;
    value[2] = angle;
}