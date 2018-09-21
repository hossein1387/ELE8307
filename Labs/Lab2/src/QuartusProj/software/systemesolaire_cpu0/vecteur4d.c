#include "vecteur4d.h"

/*********************************************************
	vecteur4d_init()

 *********************************************************/
void vecteur4d_init( vecteur4d_t *v, float x, float y, float z, float w ) {

	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

/*********************************************************
	vecteur4d_init_v3d()

 *********************************************************/
void vecteur4d_init_v3d( vecteur4d_t *v, vecteur3d_t *u ) {

	v->x = u->x;
	v->y = u->y;
	v->z = u->z;
	v->w = 1;
}

/*********************************************************
	vecteur4d_getVecteur3d()

 *********************************************************/
void vecteur4d_getVecteur3d( vecteur4d_t *v , vecteur3d_t *result ) {

	/*result->x = v->x / v->w;
	result->y = v->y / v->w;
	result->z = v->z / v->w;
	*/
	
	if( v->w < 0 ) {
		result->x = v->x / -v->w;
		result->y = v->y / -v->w;
		result->z = v->z / v->w;
	}
	else { 
		result->x = v->x / v->w;
		result->y = v->y / v->w;
		result->z = v->z / v->w;
	}
	
}

void vecteur4d_print( vecteur4d_t *v ) {
    
    printf("(%2.2f, %2.2f, %2.2f, %2.2f)", v->x,v->y,v->z,v->z);
    
}

