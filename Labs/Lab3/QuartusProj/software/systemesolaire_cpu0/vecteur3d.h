#ifndef VECTEUR3D_H 
#define VECTEUR3D_H

#include "math.h"

/******************************************************************
	VECTEUR3D_H

	

 ******************************************************************/

typedef struct {

	float x;
	float y;
	float z;

} vecteur3d_t;


void vecteur3d_init( vecteur3d_t *v, float x, float y, float z );
void vecteur3d_add( vecteur3d_t *v, vecteur3d_t *w );
void vecteur3d_sub( vecteur3d_t *v, vecteur3d_t *w );
void vecteur3d_mul( vecteur3d_t *v, float k );
void vecteur3d_negate( vecteur3d_t *v );
void vecteur3d_normalize( vecteur3d_t *v );

float vecteur3d_length( vecteur3d_t *v );
float vecteur3d_squaredLength( vecteur3d_t *v );
float vecteur3d_dotProduct( vecteur3d_t *v, vecteur3d_t *w );
//vecteur3d_t vecteur3d_crossProduct( vecteur3d_t *v, vecteur3d_t *w );
void vecteur3d_crossProduct( vecteur3d_t *v, vecteur3d_t *w , vecteur3d_t* result);
float vecteur3d_angle( vecteur3d_t *v, vecteur3d_t *w );

float vecteur3d_distance( vecteur3d_t *p1, vecteur3d_t *p2 );


void vecteur3d_print( vecteur3d_t *v );


#endif //VECTEUR3D_H
