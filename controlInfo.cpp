//
// Created on 18-5-27.
//

#include "controlInfo.h"

using namespace std;

JSin ControlInfo::get() {
    lock_guard<mutex> l(ci_mutex);
    return this->in;
}

void ControlInfo::set(unsigned char *cin) {
    lock_guard<mutex> l(ci_mutex);
    for (int i = 0; i < sizeof(this->in.data); ++i) {
        this->in.data[i] = cin[i];
    }
}