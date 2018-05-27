//
// Created on 18-5-27.
//

#include "control.h"

int Control::operator()(ControlInfo &info) {
    //joystick
    Joystick joystick;
    bool jsCont = joystick.isFound();
    if (!jsCont) {
        printf("Joystick open failed.\n");
    }
    while (jsCont) {
        //joystick state
        JSin in;
        JoystickEvent event;
        if (joystick.sample(&event)) {
            if (event.isButton()) {
                //printf("Button %u is %s\n", event.number, event.value == 0 ? "up" : "down");
                if (event.value == 1) {
                    if (event.number < 8)
                        in.js.button1 |= (1 << event.number);
                    else
                        in.js.button2 |= (1 << (event.number - 8));
                } else {
                    if (event.number < 8)
                        in.js.button1 &= ~(1 << event.number);
                    else
                        in.js.button2 &= ~(1 << (event.number - 8));
                }
            }
            if (event.isAxis()) {
                int sum = 32767 * 2;
                //printf("Axis %u is at position %d\n", event.number, event.value*255/sum);
                if (event.number < sizeof(in.js.axis)) {
                    in.js.axis[event.number] = static_cast<unsigned char>(event.value * 255 / sum);
                }
            }
            info.set(in.data);
        }
    }
    return 0;
}