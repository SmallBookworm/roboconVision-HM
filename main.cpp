#include <iostream>
#include <thread>
#include <csignal>
#include <sys/time.h>
#include "serial.hpp"
#include "Info.h"
#include "lineTest.h"
#include "rtlFinder.h"
#include "joystick.hh"

#define DOCKING_MODE 0x1
#define DROP_MODE 0x2

using namespace std;
using namespace cv;

int state = 0x0;

union Out wdata{};
//joystick state
unsigned char button = 0;
unsigned char axis[sizeof(wdata.meta.axis)];

MySerial ms = MySerial();
int fd;

void printMes(int signo) {
    //printf("Get a SIGALRM, signal NO:%d\n", signo);
    //joystick
    wdata.meta.button[0] |= button;
    for (int i = 0; i < sizeof(axis); ++i) {
        wdata.meta.axis[i] = axis[i];
    }
    //sum flag
    assignSum(&wdata);
    ms.nwrite(fd, wdata.data, sizeof(wdata.data));
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

    //joystick
    Joystick joystick;
    if (!joystick.isFound()) {
        printf("open failed.\n");
    }

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
    Info info;
    while (true) {
        //joystick control
        JoystickEvent event;
        if (joystick.sample(&event)) {
            if (event.isButton()) {
                //printf("Button %u is %s\n", event.number, event.value == 0 ? "up" : "down");
                if (event.value == 1)
                    button |= (1 << event.number);
                else
                    button &= ~(1 << event.number);
            }
            if (event.isAxis()) {
                int sum = 32767 * 2;
                printf("Axis %u is at position %d\n", event.number, event.value*255/sum);
                if (event.number < sizeof(wdata.meta.axis)) {
                    axis[event.number] = static_cast<unsigned char>(event.value * 255 / sum);
                }
            }
        }
        //read message
        unsigned char rdata;
        int n = ms.nread(fd, &rdata, 1);
        //sometime read nothing
        if (n <= 0)
            continue;
        cout << int(rdata) << endl;
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
                wdata.meta.dataArea[0] |= 0x02;
                memcpy(wdata.meta.dockDModule, &res[0], sizeof(res[0]));
                memcpy(wdata.meta.dockArgument, &res[1], sizeof(res[0]));
                memcpy(wdata.meta.dockRAngle, &res[2], sizeof(res[0]));
            }
        } else if ((state & DOCKING_MODE) != 0) {
            state ^= DOCKING_MODE;
            lineInfo.setStop(true);
        }
        //realtime find line
        double rtlCoordinate[4];
        int res = rtlInfo.get(rtlCoordinate);
        if (res >= 0) {
            wdata.meta.dataArea[0] |= 0x08;
            memcpy(wdata.meta.xAngle, &rtlCoordinate[0], sizeof(rtlCoordinate[0]));
            memcpy(wdata.meta.yAngle, &rtlCoordinate[1], sizeof(rtlCoordinate[0]));
            memcpy(wdata.meta.xDis, &rtlCoordinate[2], sizeof(rtlCoordinate[0]));
            memcpy(wdata.meta.yDis, &rtlCoordinate[3], sizeof(rtlCoordinate[0]));
        }
    }
    return 0;
}