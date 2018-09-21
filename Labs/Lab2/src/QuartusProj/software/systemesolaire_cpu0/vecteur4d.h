#ifndef VECTEUR4D_H 
#define VECTEUR4D_H

#include "vecteur3d.h"

/******************************************************************
	VECTEUR4D_H

	

 ******************************************************************/


typedef struct {

	float x;
	float y;
	float z;
	float w;

} vecteur4d_t;

void vecteur4d_init( vecteur4d_t *v, float x, float y, float z, float w );
void vecteur4d_init_v3d( vecteur4d_t *v, vecteur3d_t *u );
//vecteur3d_t vecteur4d_getVecteur3d( vecteur4d_t *v );
void vecteur4d_getVecteur3d( vecteur4d_t *v , vecteur3d_t *result );

void vecteur4d_print( vecteur4d_t *v );


#endif VECTEUR4D_H