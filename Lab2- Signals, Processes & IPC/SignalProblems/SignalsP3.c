
#include <stdio.h>
void ignore_SIGSEGV() {
    signal (SIGSEGV, SIG_IGN);
}