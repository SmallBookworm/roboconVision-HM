//
// Created on 18-5-27.
//

#ifndef ROBOCONVISIONHM_CONTROL_H
#define ROBOCONVISIONHM_CONTROL_H

#include "controlInfo.h"
#include "joystick.hh"

class Control {
public:
    int operator()(ControlInfo &info);
};


#endif //ROBOCONVISIONHM_CONTROL_H
