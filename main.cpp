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
JSin jsin;

MySerial ms = MySerial();
int fd;

//reduce code in interrupt
void printMes(int signo) {
    //printf("Get a SIGALRM, signal NO:%d\n", signo);
    //joystick
    wdata.meta.button1[0] = jsin.js.button1;
    wdata.meta.button2[0] = jsin.js.button2;
    for (int i = 0; i < sizeof(jsin.js.axis); ++i) {
        wdata.meta.axis[i] = jsin.js.axis[i];
    }
    //sum flag
    assignSum(&wdata);
    if (wdata.meta.head[1] != 0xbb) {
        cout << "errh" << endl;
    }
    int a = ms.nwrite(fd, wdata.data, sizeof(wdata.data));
    if (a != 48)
        cout << a << endl;
    //restore
    wdata = {};
}

int main() {
    fd = ms.open_port(1);
    ms.set_opt(fd, BAUDRATE, 8, 'N', 1);

    struct itimerval tick;
    signal(SIGALRM, printMes);
    memset(&tick, 0, sizeof(tick));
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 50000;
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 50000;
    if (setitimer(ITIMER_REAL, &tick, NULL) < 0)
        printf("Set time fail!");

    ControlInfo controlInfo;
    Control control;
    thread thread10(control, ref(controlInfo));
    thread10.detach();
    //union Out s{};
    //cout << s.data << " length:" << sizeof(s.data) << endl;

    RtlFinder rtlFinder;
    RtlInfo rtlInfo;
    bool test = true;
    if (!test) {
        thread thread11(rtlFinder, ref(rtlInfo));
        thread11.detach();
    }

    LineInfo lineInfo;
    LineInfo ballTakeInfo;
    Info info;
    while (true) {
        //joystick control
        jsin.js = controlInfo.get().js;
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
        //Docking mode
        if ((info.result.meta.flag1[0] & (1 << 1)) != 0) {
            if ((state & DOCKING_MODE) == 0) {
                state |= DOCKING_MODE;
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
            if ((state & TAKE_MODE) == 0) {
                state |= TAKE_MODE;
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
        //realtime find line
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
    }
    return 0;
}