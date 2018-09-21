#ifndef MATRICE4D_H
#define MATRICE4D_H

#include "vecteur3d.h"
#include "vecteur4d.h"
//#include "triangle3d.h"

/******************************************************************
	MATRICE4D_H

	

 ******************************************************************/


typedef struct {

	float m[4][4];			// m[ligne][colone]

} matrice4d_t;

void matrice4d_init( matrice4d_t *m );

void matrice4d_product_m4d( matrice4d_t *m, matrice4d_t *n, matrice4d_t *result );
void matrice4d_product_v4d( matrice4d_t *m, vecteur4d_t *v, vecteur4d_t *result );
void matrice4d_product_v3d( matrice4d_t *m, vecteur3d_t *v, vecteur4d_t *result );

void matrice4d_invert( matrice4d_t *m );
void matrice4d_transpose( matrice4d_t *m, matrice4d_t *result );

void matrice4d_setIdentity( matrice4d_t *m );
void matrice4d_setTranslationMatrix( matrice4d_t *m, vecteur3d_t *v );
void matrice4d_setRotationMatrixX( matrice4d_t *m, float angle );
void matrice4d_setRotationMatrixY( matrice4d_t *m, float angle );
void matrice4d_setRotationMatrixZ( matrice4d_t *m, float angle );
void matrice4d_setRotationMatrixN( matrice4d_t *m, vecteur3d_t *normal, float angle ); 

void matrice4d_setRotationMatrixXYZ( matrice4d_t *m, vecteur3d_t *X, vecteur3d_t *Y, vecteur3d_t *Z );
void matrice4d_setProjectionMatrix( matrice4d_t *m, float near_dist, float far_dist, float zoomx, float zoomy ); 

void matrice4d_print( matrice4d_t *m );


#endif // MATRICE4D_H