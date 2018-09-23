#include "VGA.h"
#ifndef __GNUC__
    #include "io.h"
#endif
//-------------------------------------------------------------------------
void VGA::clr() {
    int x;
    for (x=0;x<640*480/4;x++) {
    #ifndef __GNUC__
       IOWR(memory_address, x, 0x0);
    #else
       //printf("writing %0d to %0d\n", 0, x);
    #endif

    }
}
