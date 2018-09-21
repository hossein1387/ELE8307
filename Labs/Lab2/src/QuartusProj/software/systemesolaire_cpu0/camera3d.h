#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "vecteur3d.h"
#include "matrice4d.h"
#include "ecran2d.h"

/******************************************************************
	CAMERA3D_H

  
	Important : Les matrices de 'vue' est produites a partir de 
				l'orientation et de la position	de la camera.
				Lorsque ces valeurs sont modifiees manuellement, 
				il faut appeller 'camera3d_setViewTransform' pour 
				mettre a jour la matrices.

				Il en est de meme pour la matrice 'projection' 
				produite a partir des valeurs 'zoom' et 'near/far'.
				(camera3d_setProjectionTransform)
	

 ******************************************************************/

#define		CAMERA3D_NEAR_DIST_DEFAULT	1.0
#define		CAMERA3D_FAR_DIST_DEFAULT	1000000.0
#define		CAMERA3D_ZOOM_DEFAULT		4.0

typedef struct {

	vecteur3d_t		center;
	vecteur3d_t		dir;
	vecteur3d_t		top;
	float			near_dist ,far_dist;
	float			zoomx, zoomy;

	matrice4d_t		view_matrix;
	matrice4d_t		projection_matrix;

} camera3d_t;


void camera3d_init( camera3d_t *cam );

void camera3d_setPosition( camera3d_t *cam, vecteur3d_t *pos );
void camera3d_setOrientation( camera3d_t *cam, vecteur3d_t *dir, vecteur3d_t *top );

void camera3d_getSideVector( camera3d_t *cam, vecteur3d_t *result );
void camera3d_getViewTransform( camera3d_t *cam, matrice4d_t *result );
void camera3d_getProjectionTransform( camera3d_t *cam, matrice4d_t *result );

void camera3d_rotatePitch( camera3d_t *cam, float angle );
void camera3d_rotateHeading( camera3d_t *cam, float angle );
void camera3d_rotateBank( camera3d_t *cam, float angle );

void camera3d_translateAlongDir( camera3d_t *cam, float distance );
void camera3d_translateAlongTop( camera3d_t *cam, float distance );
void camera3d_translateAlongSide( camera3d_t *cam, float distance );


void camera3d_setViewTransform( camera3d_t *cam );
void camera3d_setProjectionTransform( camera3d_t *cam );




#endif  // CAMERA3D_H