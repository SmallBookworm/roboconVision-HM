#include <iostream>
#include <thread>
#include <csignal>
#include <sys/time.h>
#include "serial.hpp"
#include "Info.h"
#include "lineTest.h"
#include "ballTake.h"
#include "rtlFinder.h"
#include "joystick.hh"
#include "control.h"

#define DOCKING_MODE 0x1
#define TAKE_MODE 0x2

using namespace std;
using namespace cv;

int state = 0x0;

union Out wdata{};
//init wdata when main thread post data too fast.
bool initWdata = true;

void setJSValue(JSin jsin, bool &gear, bool &gearButton);

MySerial ms = MySerial();
int fd;

//reduce code in interrupt
void printMes(int signo) {
    //printf("Get a SIGALRM, signal NO:%d\n", signo);
    //sum flag
    assignSum(&wdata);
    int a = ms.nwrite(fd, wdata.data, sizeof(wdata.data));
    //restore
    wdata = {};
    initWdata = true;
}

int main() {
    fd = ms.open_port(1);
//    while (fd<0) {
//        fd = ms.open_port(1);
//    }
    ms.set_opt(fd, BAUDRATE, 8, 'N', 1);

    struct itimerval tick;
    signal(SIGALRM, printMes);
    memset(&tick, 0, sizeof(tick));
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 20000;
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 20000;
    if (setitimer(ITIMER_REAL, &tick, NULL) < 0)
        printf("Set time fail!");

    ControlInfo controlInfo;
    Control control;
    JSin jsin;
    bool gear = false;
    bool gearButton = false;
    thread thread10(control, ref(controlInfo));
    thread10.detach();
    //union Out s{};
    //cout << s.data << " length:" << sizeof(s.data) << endl;

    RtlFinder rtlFinder;
    RtlInfo rtlInfo;
    thread thread11(rtlFinder, ref(rtlInfo));
    thread11.detach();

    LineInfo lineInfo;
    LineInfo ballTakeInfo;
    Info info;
    //1:video0,2:video1,4:joystick
    unsigned char deviceState = 0;
    while (true) {
        bool tJS = (access("/dev/input/js0", F_OK) != -1);
        if (controlInfo.getThreadState()) {
            //joystick control
            bool jsNew = controlInfo.get(jsin);
            if (jsNew || initWdata) {
                setJSValue(jsin, gear, gearButton);
            }
        } else if (tJS) {
            JSin initjs;
            jsin = initjs;
            controlInfo.init();
            thread thread10(control, ref(controlInfo));
            thread10.detach();
        }
        //read message
        unsigned char rdata;
        int n = ms.nread(fd, &rdata, 1);
        //sometime read nothing
        if (n <= 0)
            continue;
        //cout << int(rdata) << endl;
        //cout << info.result.data << " length:" << sizeof(info.result.data) << endl;
        if (info.push(rdata) <= 0)continue;
        //wdata.meta.dataArea[0] = 0;
        //cout << "Docking mode" << (info.result.meta.flag1[0] & (1 << 1)) << endl;
        bool tVideo1 = (access("/dev/video1", F_OK) != -1);
        //Docking mode
        if ((info.result.meta.flag1[0] & (1 << 1)) != 0) {
            if (tVideo1)
                if ((state & DOCKING_MODE) == 0) {
                    state |= DOCKING_MODE;
                    lineInfo.init();
                    LineTest tracker;
                    thread thread1(tracker, ref(lineInfo));
                    thread1.detach();
                } else if (!lineInfo.getThreadState()) {
                    lineInfo.init();
                    LineTest tracker;
                    thread thread1(tracker, ref(lineInfo));
                    thread1.detach();
                }
            float res[3];
            int resF = lineInfo.get(res);
            if (resF > 0) {
                wdata.meta.dataArea[0] |= 0x01;
                memcpy(wdata.meta.dockDModule, &res[0], sizeof(res[0]));
                memcpy(wdata.meta.dockArgument, &res[1], sizeof(res[0]));
                memcpy(wdata.meta.dockRAngle, &res[2], sizeof(res[0]));
            }
        } else if ((state & DOCKING_MODE) != 0) {
            state ^= DOCKING_MODE;
            lineInfo.setStop(true);
        }
        //Take mode
        if ((info.result.meta.flag1[0] & (1 << 3)) != 0) {
            if (tVideo1)
                if ((state & TAKE_MODE) == 0) {
                    state |= TAKE_MODE;
                    ballTakeInfo.init();
                    BallTake tracker;
                    thread thread1(tracker, ref(ballTakeInfo));
                    thread1.detach();
                } else if (!ballTakeInfo.getThreadState()) {
                    ballTakeInfo.init();
                    BallTake tracker;
                    thread thread1(tracker, ref(ballTakeInfo));
                    thread1.detach();
                }
            float res[3];
            int resF = ballTakeInfo.get(res);
            if (resF > 0) {
                wdata.meta.dataArea[0] |= 0x02;
                memcpy(wdata.meta.dockDModule, &res[0], sizeof(res[0]));
                memcpy(wdata.meta.dockArgument, &res[1], sizeof(res[0]));
                memcpy(wdata.meta.dockRAngle, &res[2], sizeof(res[0]));
            }
        } else if ((state & TAKE_MODE) != 0) {
            state ^= TAKE_MODE;
            ballTakeInfo.setStop(true);
        }
        bool tVideo0 = (access("/dev/video0", F_OK) != -1);
        //realtime find line
        if (rtlInfo.getThreadState()) {
            double rtlCoordinate[4];
            char res = rtlInfo.get(rtlCoordinate);
            if (res > 0) {
                if ((res & 1) != 0) {
                    float line[] = {static_cast<float>(rtlCoordinate[2]), static_cast<float>(rtlCoordinate[0])};
                    wdata.meta.dataArea[0] |= 0x04;
                    memcpy(wdata.meta.xDis, &line[0], sizeof(line[0]));
                    memcpy(wdata.meta.xAngle, &line[1], sizeof(line[0]));
                }
                if ((res & 2) != 0) {
                    float line[] = {static_cast<float>(rtlCoordinate[3]), static_cast<float>(rtlCoordinate[1])};
                    wdata.meta.dataArea[0] |= 0x08;
                    memcpy(wdata.meta.yDis, &line[0], sizeof(line[0]));
                    memcpy(wdata.meta.yAngle, &line[1], sizeof(line[0]));
                }
            }
        } else if (tVideo0) {
            rtlInfo.init();
            thread thread11(rtlFinder, ref(rtlInfo));
            thread11.detach();
        }
        //device
        if (tJS)
            deviceState |= (1 << 2);
        else
            deviceState &= ~(1 << 2);
        if (tVideo1)
            deviceState |= (1 << 1);
        else
            deviceState &= ~(1 << 1);
        wdata.meta.device[0] = deviceState;

        if (initWdata)
            initWdata = false;
    }
    return 0;
}

void setJSValue(JSin jsin, bool &gear, bool &gearButton) {
    wdata.meta.button1[0] = jsin.js.button1;
    wdata.meta.button2[0] = jsin.js.button2;
    //control
    //LT
    char lt = jsin.js.axis[2];
    if (lt > -90)
        wdata.meta.button2[0] |= (1 << 3);
    else
        wdata.meta.button2[0] &= ~(1 << 3);
    //RT
    char rt = jsin.js.axis[5];
    if (rt > -90)
        wdata.meta.button2[0] |= (1 << 4);
    else
        wdata.meta.button2[0] &= ~(1 << 4);
    //left&right
    char lra = jsin.js.axis[6];
    if (abs(lra) > 50)
        wdata.meta.button2[0] |= (1 << 5);
    else
        wdata.meta.button2[0] &= ~(1 << 5);
    //up&down
    char uda = jsin.js.axis[7];
    if (abs(uda) > 50)
        wdata.meta.button2[0] |= (1 << 6);
    else
        wdata.meta.button2[0] &= ~(1 << 6);
    //spin angle
    char rax = jsin.js.axis[3];
    if (rax > 90) {
        wdata.meta.cSpinAngleASpeed[0] |= (1 << 0);
        wdata.meta.cSpinAngleASpeed[0] &= ~(1 << 1);
    } else if (rax < -90) {
        wdata.meta.cSpinAngleASpeed[0] &= ~(1 << 0);
        wdata.meta.cSpinAngleASpeed[0] |= (1 << 1);
    } else {
        wdata.meta.cSpinAngleASpeed[0] &= ~(1 << 0);
        wdata.meta.cSpinAngleASpeed[0] &= ~(1 << 1);
    }
    //speed
    //gear
    if ((jsin.js.button2 & (1 << 1)) != 0) {
        if (!gearButton)
            gear = !gear;
        gearButton = true;
    } else
        gearButton = false;
    //run or stop
    char lax = jsin.js.axis[0];
    char lay = jsin.js.axis[1];
    //x-y axis is circle
    double speedValue = sqrt(lax * lax + lay * lay) / 127;
    if (speedValue > 0.8) {
        //set gear
        if (gear) {
            wdata.meta.cSpinAngleASpeed[0] &= ~(1 << 2);
            wdata.meta.cSpinAngleASpeed[0] |= (1 << 3);
        } else {
            wdata.meta.cSpinAngleASpeed[0] |= (1 << 2);
            wdata.meta.cSpinAngleASpeed[0] &= ~(1 << 3);
        }
        //set angle
        if (lay < 0)
            wdata.meta.cSpinAngleASpeed[0] &= ~(1 << 4);
        else
            wdata.meta.cSpinAngleASpeed[0] |= (1 << 4);
        if (lax > 0)
            wdata.meta.cAngle[0] = atan(abs(lay / lax)) * 180 / M_PI;
        else {
            if (lax == 0)
                wdata.meta.cAngle[0] = 90;
            else
                wdata.meta.cAngle[0] = (M_PI - atan(abs(lay / lax))) * 180 / M_PI;

        }
    } else {
        wdata.meta.cSpinAngleASpeed[0] &= ~(1 << 2);
        wdata.meta.cSpinAngleASpeed[0] &= ~(1 << 3);
    }

}