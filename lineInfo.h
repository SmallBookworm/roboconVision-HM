//
// Created by peng on 18-4-8.
//

#ifndef LIGHTBAR_LINEINFO_H
#define LIGHTBAR_LINEINFO_H

#include <mutex>
#include "ThreadInfo.h"

class LineInfo : public ThreadInfo {
private:
    float dModule;
    float argument;
    float relativeAngle;
private:
    bool stop = false;
    std::mutex stop_mutex;
private:
    //real x,y,z,angle(-1:tz1||normal,1:tz2||golden)
    short x, y, angle;
    std::mutex position_mutex;
public:
    void init(short x, short y, short angle);

    void set(float value[3]);

//-1 used,1 success
    int get(float *res);

    void setStop(bool f);

    bool getStop();

    void getPositionInfo(short (&value)[3]);
};


#endif //LIGHTBAR_LINEINFO_H
