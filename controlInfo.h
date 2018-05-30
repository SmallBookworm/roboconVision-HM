//
// Created on 18-5-27.
//

#ifndef ROBOCONVISIONHM_CONTROLINFO_H
#define ROBOCONVISIONHM_CONTROLINFO_H

#include <mutex>
#include "ThreadInfo.h"

struct JStick {
    unsigned char button1;
    unsigned char button2;
    unsigned char axis[8];
};
union JSin {
    //129=-127
    unsigned char data[10]{0, 0, 0, 0, 129, 0, 0, 129, 0, 0};
    JStick js;
};

class ControlInfo : public ThreadInfo{
private:
//joystick state
    JSin in;
public:
    //true:success,false:fail
    bool get(JSin &res);

    void set(unsigned char *cin);
};


#endif //ROBOCONVISIONHM_CONTROLINFO_H
