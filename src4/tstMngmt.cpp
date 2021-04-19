#include "mngmt.h"

int main() {
    mngmt *m = mngmt::Instance();

    m->initTask(2);
    m->dump();
}

