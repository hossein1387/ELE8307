#include "vecteur3d.h"

/*********************************************************
	vecteur3d_init()

 *********************************************************/
void vecteur3d_init( vecteur3d_t *v, float x, float y, float z ) {

	v->x = x;
	v->y = y;
	v->z = z;
}

/*********************************************************
	vecteur3d_add()

 *********************************************************/
void vecteur3d_add( vecteur3d_t *v, vecteur3d_t *w ) {

	v->x = v->x + w->x;
	v->y = v->y + w->y;
	v->z = v->z + w->z;
}

/*********************************************************
	vecteur3d_sub()

 *********************************************************/
void vecteur3d_sub( vecteur3d_t *v, vecteur3d_t *w ) {

	v->x = v->x - w->x;
	v->y = v->y - w->y;
	v->z = v->z - w->z;
}

/*********************************************************
	vecteur3d_sub()

 *********************************************************/
void vecteur3d_mul( vecteur3d_t *v, float k ) {
	v->x *= k;
	v->y *= k;
	v->z *= k;
}


/*********************************************************
	vecteur3d_negate()

 *********************************************************/
void vecteur3d_negate( vecteur3d_t *v ) {

	v->x = -v->x;
	v->y = -v->y;
	v->z = -v->z;
}

/*********************************************************
	vecteur3d_normalize()

 *********************************************************/
void vecteur3d_normalize( vecteur3d_t *v ) {

	float len;
	len = vecteur3d_length(v);
	v->x /= len;
	v->y /= len;
	v->z /= len;
}

/*********************************************************
	vecteur3d_length()

 *********************************************************/
float vecteur3d_length( vecteur3d_t *v ) {

	return (float)sqrt( v->x*v->x + v->y*v->y + v->z*v->z );
}

/*********************************************************
	vecteur3d_squaredLength()

 *********************************************************/
float vecteur3d_squaredLength( vecteur3d_t *v ) {

	return ( v->x*v->x + v->y*v->y + v->z*v->z );
}


/*********************************************************
	vecteur3d_dotProduct()

 *********************************************************/
float vecteur3d_dotProduct( vecteur3d_t *v, vecteur3d_t *w ) {

	return ( v->x*w->x + v->y*w->y + v->z*w->z );
}

/*********************************************************
	vecteur3d_crossProduct()

 *********************************************************/
void vecteur3d_crossProduct( vecteur3d_t *v, vecteur3d_t *w , vecteur3d_t* result) {



	result->x = v->y*w->z - v->z*w->y;
	result->y = v->z*w->x - v->x*w->z;
	result->z = v->x*w->y - v->y*w->x;
}

/*********************************************************
	vecteur3d_angle()
	
 *********************************************************/
float vecteur3d_angle( vecteur3d_t *v, vecteur3d_t *w ) {
	// Angle E (0,PI)
	float cosA = vecteur3d_dotProduct(v,w) / (vecteur3d_length(v)*vecteur3d_length(w));
	return acos(cosA);

}

/*********************************************************
	vecteur3d_distance()
	
	distance entre 2 points
 *********************************************************/
float vecteur3d_distance( vecteur3d_t *p1, vecteur3d_t *p2 ) {

	float dx,dy,dz;
	dx = p2->x - p1->x;
	dz = p2->z - p1->z;
	dy = p2->y - p1->y;
	return sqrt( dx*dx + dy*dy + dz*dz );
}


/*********************************************************
	vecteur3d_print()

 *********************************************************/
void vecteur3d_print( vecteur3d_t *v ) {

	printf("(%2.2f, %2.2f, %2.2f)", v->x,v->y,v->z);
}
