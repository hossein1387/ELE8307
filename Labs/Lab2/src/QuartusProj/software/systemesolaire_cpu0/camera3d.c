#include "camera3d.h"

/*********************************************************
	camera3d_init()

 *********************************************************/
void camera3d_init( camera3d_t *cam ) {

	vecteur3d_init(&cam->center, 0,0,1000000000 );
	vecteur3d_init(&cam->dir,    0,0,-1 );
	vecteur3d_init(&cam->top,	 0,1,0 );

/*	vecteur3d_init(&cam->center, 0,-100,0 );
	vecteur3d_init(&cam->dir,    0,1,0 );
	vecteur3d_init(&cam->top,	 0,0,1 );
*/
	vecteur3d_init(&cam->center, 1000,0,0 );
	vecteur3d_init(&cam->dir,    1,0,0 );
	vecteur3d_init(&cam->top,	 0,0,1 );

	cam->near_dist =	CAMERA3D_NEAR_DIST_DEFAULT;
	cam->far_dist =		CAMERA3D_FAR_DIST_DEFAULT;
	cam->zoomx =		CAMERA3D_ZOOM_DEFAULT;
	cam->zoomy =		CAMERA3D_ZOOM_DEFAULT * ECRAN2D_WIDTH/ECRAN2D_HEIGHT;

	camera3d_setViewTransform( cam );
	camera3d_setProjectionTransform( cam );
}

/*********************************************************
	camera3d_setPosition()

	Fixe la position de la camera et met a jour la matrice
	'vue'.

 *********************************************************/
void camera3d_setPosition( camera3d_t *cam, vecteur3d_t *pos ) {

	cam->center = *pos;
	camera3d_setViewTransform( cam );
}

/*********************************************************
	camera3d_setOrientation()

	Fixe l'orientation de la camera et met a jour la 
	matrice 'projection'

 *********************************************************/
void camera3d_setOrientation( camera3d_t *cam, vecteur3d_t *dir, vecteur3d_t *top ) {

	cam->dir = *dir;
	cam->top = *top;
	camera3d_setViewTransform( cam );
}

/*********************************************************
	camera3d_getSideVector()

 *********************************************************/
void camera3d_getSideVector( camera3d_t *cam, vecteur3d_t *result ) {

	vecteur3d_crossProduct(&cam->top, &cam->dir, result);	
}

/*********************************************************
	camera3d_getViewTransform()

	Calcule la matrice de transformation 'vue' de la
	camera a partir de l'orientation et de la position.
 *********************************************************/
void camera3d_getViewTransform( camera3d_t *cam, matrice4d_t *result ) {

	matrice4d_t tm, rm;
	vecteur3d_t side;

	vecteur3d_t camTranslation;
	camTranslation = cam->center;
	vecteur3d_negate(&camTranslation );
	matrice4d_setTranslationMatrix(&tm, &camTranslation);

	camera3d_getSideVector(cam, &side);
	matrice4d_setRotationMatrixXYZ(&rm, &side, &cam->top, &cam->dir );
	matrice4d_product_m4d(&rm, &tm, result);
}

/*********************************************************
	camera3d_getProjectionTransform()

  	Calcule la matrice de transformation 'projection'
	de la camera a partir des parametres 'zoom', 'far'
	et 'near'.
 *********************************************************/
void camera3d_getProjectionTransform( camera3d_t *cam, matrice4d_t *result ) {

	matrice4d_setProjectionMatrix( result, cam->near_dist, cam->far_dist, cam->zoomx, cam->zoomy );
}

/*********************************************************
	camera3d_rotatePitch()

 *********************************************************/
void camera3d_rotatePitch( camera3d_t *cam, float angle ) {

	matrice4d_t	rm;
	vecteur3d_t side;
	vecteur4d_t v4d;

	camera3d_getSideVector(cam, &side);
	matrice4d_setRotationMatrixN( &rm, &side, angle );
	matrice4d_product_v3d( &rm, &cam->dir, &v4d );
	vecteur4d_getVecteur3d( &v4d, &cam->dir);
	matrice4d_product_v3d( &rm, &cam->top, &v4d );
	vecteur4d_getVecteur3d( &v4d, &cam->top);

	camera3d_setViewTransform( cam );
}

/*********************************************************
	camera3d_rotateHeading()

 *********************************************************/
void camera3d_rotateHeading( camera3d_t *cam, float angle ) {

	matrice4d_t	rm;
	vecteur4d_t v4d;

	matrice4d_setRotationMatrixN( &rm, &cam->top, angle );
	matrice4d_product_v3d( &rm, &cam->dir, &v4d );
	vecteur4d_getVecteur3d( &v4d, &cam->dir);

	camera3d_setViewTransform( cam );
}

/*********************************************************
	camera3d_rotateBank()

 *********************************************************/
void camera3d_rotateBank( camera3d_t *cam, float angle ) {

	matrice4d_t	rm;
	vecteur4d_t v4d;

	matrice4d_setRotationMatrixN( &rm, &cam->dir, angle );
	matrice4d_product_v3d( &rm, &cam->top, &v4d );
	vecteur4d_getVecteur3d( &v4d, &cam->top);

	camera3d_setViewTransform( cam );
}

/*********************************************************
	camera3d_translateAlongDir()

 *********************************************************/
void camera3d_translateAlongDir( camera3d_t *cam, float distance ) {

	cam->center.x = cam->center.x + distance*cam->dir.x;
	cam->center.y = cam->center.y + distance*cam->dir.y;
	cam->center.z = cam->center.z + distance*cam->dir.z;

	camera3d_setViewTransform( cam );
}


/*********************************************************
	camera3d_translateAlongTop()

 *********************************************************/
void camera3d_translateAlongTop( camera3d_t *cam, float distance ) {

	cam->center.x = cam->center.x + distance*cam->top.x;
	cam->center.y = cam->center.y + distance*cam->top.y;
	cam->center.z = cam->center.z + distance*cam->top.z;

	camera3d_setViewTransform( cam );
}

/*********************************************************
	camera3d_translateAlongSide()

 *********************************************************/
void camera3d_translateAlongSide( camera3d_t *cam, float distance ) {

	vecteur3d_t side;
	camera3d_getSideVector(cam, &side);
	cam->center.x = cam->center.x + distance*side.x;
	cam->center.y = cam->center.y + distance*side.y;
	cam->center.z = cam->center.z + distance*side.z;

	camera3d_setViewTransform( cam );
}


/*********************************************************
	camera3d_setViewTransform()

	Calcule la matrice de transformation vue de cette 
	camera et l'applique a cette derniere.
 *********************************************************/
void camera3d_setViewTransform( camera3d_t *cam ) { 

	camera3d_getViewTransform( cam, &cam->view_matrix );
}

/*********************************************************
	camera3d_setProjectionTransform()

  	Calcule la matrice de transformation projection de 
	cette camera et l'applique a cette derniere.
 *********************************************************/
void camera3d_setProjectionTransform( camera3d_t *cam ) { 

	camera3d_getProjectionTransform( cam, &cam->projection_matrix );
}
