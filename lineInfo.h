//
// Created by peng on 18-4-8.
//

#ifndef LIGHTBAR_LINEINFO_H
#define LIGHTBAR_LINEINFO_H

#include <mutex>
#include "ThreadInfo.h"

class LineInfo :public ThreadInfo{
private:
    float dModule;
    float argument;
    float relativeAngle;
private:
    bool stop= false;
    std::mutex stop_mutex;

public:
    void init();

    void set(float value[3]);

//-1 used,1 success
    int get(float *res);

    void setStop(bool f);

    bool getStop();
};


#endif //LIGHTBAR_LINEINFO_H
