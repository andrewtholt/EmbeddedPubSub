#include <stdio.h>
#include <string.h>
#include "dump.h"

char p[] = "This is a test";

int main() {
    dump(p, strlen(p));
}

