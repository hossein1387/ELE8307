/********************************************************************
 * main.cpp
 *
 * Routine principale pour le projet ELE8307 A13.
 *
 * v1.2 - Application avec une machine pre-entrainee pour la
 *          reconnaissance de caracteres en braille* (avec une image
 *          50x100 pixels contenant 9 characteres 16x16).
 *
 *      - Ajout d'un #define pour activer/desactiver les printf dans
 *          la routine Image::apply_NN();
 *
 * Author : (V)-F)-I)
 *
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#ifndef __GNUC__
    #include <sys/alt_alarm.h>
#endif

#include "NN.h"
// #include "CNN.h"
// #include "MaxPool.h"
#include "Image.h"
#include "VGA.h"

VGA * pVGA;
char display[VGA_WIDTH*VGA_HEIGHT];
int* ELE8307_VGA_0_BASE = NULL;
/*********************************************************************
 * main
 *********************************************************************/
int main(int argc, char **argv)
{
    printf("Entering ELE8307 A17 Project main() \r\n");

    pVGA = new VGA(ELE8307_VGA_0_BASE, display);
    pVGA->send_to_display();
    pVGA->clr();
    
    /* Initialisation et application a une image de la machine neuronale (poids aleatoires) */

    printf("> Exemple 1: NN Aleatoire \r\n");
    NN network(3);
    int matrix_size = 8;
    network.layer[0].random_init(matrix_size*matrix_size, 200);
    network.layer[1].random_init(200, 50);
    network.layer[2].random_init(50, 1);
    network.make_ternary();

    Image my_image(640,200);
    my_image.printToScreen(0,0,pVGA);
    printf("Start processing ...");
#ifndef __GNUC__
    int time1 = alt_nticks();
#endif
    Image * result_image = my_image.apply_NN(&network, matrix_size);
#ifndef __GNUC__
    int time2 = alt_nticks();
#endif
    result_image->print();
    result_image->printToScreen(0,240,pVGA);

#ifndef __GNUC__
    printf("done in %d ms\r\n",(time2-time1));
#endif

    return 0;
}


