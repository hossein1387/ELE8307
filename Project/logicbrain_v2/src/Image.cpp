/*
 * Image.cpp
 *
 *  Created on: Jul 15, 2013
 *      Author: jpdavid
 */

#include "Image.h"

#include "stdio.h"
#ifndef __GNUC__
    #include "io.h"
    #include "system.h"
#endif

Image::Image() {
    // TODO Auto-generated constructor stub
    source_array = 0;
    length = 0;
    height = 0;
}

Image::Image(int new_length, int new_height) {
    // TODO Auto-generated constructor stub
    source_array = 0;
    init(new_length, new_height);
}

void Image::init(int new_length, int new_height) {
    // TODO Auto-generated constructor stub
    length = new_length;
    height = new_height;
    if (source_array != 0) delete[] source_array;
    source_array = new unsigned char [height*length];
    randomize();
}

void Image::randomize() {
    for (int y=0; y<height; y++) {
        for (int x=0; x<length; x++) {
            *source_pixel(x,y) = rand() &0xFF;
        }
    }
}

unsigned char * Image::source_pixel(int x, int y) {
    // TODO Auto-generated constructor stub
    return source_array + (y*length+x);
}

/*******************************************************
 * Application d'un reseau de neuronnes a cette image.
 *
 * Applique toutes les sous-images size x size de cette
 * image a l'entree de ce reseau de neuronnes.
 *
 * - Fonction de calcul de reference.
 *
 *******************************************************/
Image * Image::apply_NN(NN * network, int size) {
    float source[size*size];
    Image * result = new Image(length-size+1,height-size+1);

    for (int y=0; y<=height-size; y++) {
        for (int x=0; x<=length-size; x++) {

            /* Appliquer le reseau sur un sous-bloc de l'image */
            for (int j=0; j<size; j++) {
                for (int i=0; i<size; i++) {
                    source[j*size + i] = (*source_pixel(x+i,y+j))/255.0;
                }
            }
            network->propagate(source);

            /* Stocker les bons/meilleurs matchs */
            unsigned char pixel;
//-----------------------------------------------------------------------------
//          GET INDEX OF MAX VALUE
//
//          float scale = 255.0/(network->layer[network->n_layer-1].n_neuron-1);
//          pixel = network->getMaxOutputIndex()*scale;

//-----------------------------------------------------------------------------
//          GET MAX VALUE
//
            float scale = 255.0/2;
            pixel = (1+network->getMaxOutputValue())*scale;

            *(result->source_pixel(x,y)) = pixel;
        }
    }
    return result;
}

/**********************************************************
 * Affiche l'image a l'ecran a la position x,y.
 *
 **********************************************************/
void Image::printToScreen(int x, int y, VGA *pVGA) {
    for(int i=0; i<length; i++) {
        for(int j=0; j<height; j++) {
            pVGA->Set_Pixel_Color(x+i,y+j,*source_pixel(i,j));
        }
    }
}

/**********************************************************
 * Affiche l'image a l'ecran a la position x,y.
 *
 **********************************************************/
void Image::print() {
    printf("\n");
    for(int i=0; i<length; i++) {
        for(int j=0; j<height; j++) {
            int value = (int) *source_pixel(i,j);
            printf("%i,",value);
        }
        printf("\n");
    }
}

Image::~Image() {
    if (source_array != 0) delete[] source_array;
}

