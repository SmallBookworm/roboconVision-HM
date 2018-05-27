//
// Created on 18-5-27.
//

#ifndef ROBOCONVISIONHM_CONTROLINFO_H
#define ROBOCONVISIONHM_CONTROLINFO_H

#include <mutex>
#include "Info.h"

struct JStick {
    unsigned char button1;
    unsigned char button2;
    unsigned char axis[8];
};
union JSin {
    unsigned char data[10]{};
    JStick js;
};

class ControlInfo {
private:
//joystick state
    JSin in;
    bool used= true;
    std::mutex ci_mutex;
public:
    //true:success,false:fail
    bool get(JSin &res);

    void set(unsigned char *cin);
};


#endif //ROBOCONVISIONHM_CONTROLINFO_H
