//
// Created on 18-5-27.
//

#include "controlInfo.h"

using namespace std;

bool ControlInfo::get(JSin &res) {
    lock_guard<mutex> l(ci_mutex);
    if (used)
        return false;
    else {
        res = this->in;
        used = true;
        return true;
    }
}

void ControlInfo::set(unsigned char *cin) {
    lock_guard<mutex> l(ci_mutex);
    for (int i = 0; i < sizeof(this->in.data); ++i) {
        this->in.data[i] = cin[i];
    }
    this->used = false;
}