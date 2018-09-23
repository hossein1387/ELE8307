/*
 * Image.h
 *
 *  Created on: Jul 15, 2013
 *      Author: jpdavid
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "NN.h"
#include "stdlib.h"
#include "VGA.h"
#ifndef __GNUC__
    #include "system.h"
    #include "io.h"
#endif

class Image {
public:
    Image();
    Image(int new_length, int new_height);
    void init(int new_length, int new_height);
    void randomize();
    unsigned char * source_pixel(int x, int y);
    Image * apply_NN(NN * network, int size);
    void printToScreen(int x, int y, VGA *pVGA);
    void print();
    ~Image();

    int height;
    int length;
    unsigned char *source_array;
};

#endif /* IMAGE_H_ */
