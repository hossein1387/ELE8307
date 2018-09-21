#include "ss_planet.h"


void ss_planet_init( ss_planet_t *planet, float mass, float radius, float orbit_a, float orbit_t, int color ) {

	int i;
	
	// Properties
	planet->mass = mass;
	planet->radius = radius;	
	planet->color = color;
	planet->orbit_a = orbit_a;
		
	// Physisc
	vecteur3d_init(&planet->center, orbit_a, 0, 0);
	vecteur3d_init(&planet->speed, 0, 2*PI*orbit_a/orbit_t, 0);
	
	// Display
	for( i=0; i<2; i++ ){
		vecteur3d_init(&planet->screenRadius[i],0,0,0);
		vecteur3d_init(&planet->screenCenter[i], 0,0,0);
		planet->visible[i] = 0;
	}
	planet->current_display_buff  = 0;
	planet->previous_display_buff = 1;

	vecteur3d_init(&planet->light_dir, 0,0,1);
	vecteur3d_init(&planet->light_dir, 0,1,0);
    
}

void ss_planet_updatePos( ss_planet_t *planet, float dt ) {
	
	planet->center.x += planet->speed.x*dt;
	planet->center.y += planet->speed.y*dt;	
}

void ss_planet_updateSpeed( ss_planet_t *planet, float dVx, float dVy ) {

	planet->speed.x += dVx;
	planet->speed.y += dVy;
}

/********************************************************************
    ss_planet_render()
  
    Transformations de l'espace vers la caméra des coordonnées 
    d'une planète.
    
  *******************************************************************/
void ss_planet_render( ss_planet_t *planet, camera3d_t *cam ) {

	vecteur3d_t v3d, screenPt;
	vecteur4d_t v4d;
	vecteur3d_t v3d_planet2sun, v3d_planet2sun_top;

	matrice4d_t	modelView;
	matrice4d_t	modelViewClip;

	matrice4d_t camRot;
	float planet_screen_radius;
	float planet_screen_dist;

	vecteur3d_t		w_v3d_planet_top, w_v3d_planet_dir, w_v3d_planet_center;	// world coords
	vecteur3d_t		s_v3d_planet_top, s_v3d_planet_dir, s_v3d_planet_center;	// cam coords
		
	// Transformer le centre en coord ecran	
	matrice4d_product_m4d( &cam->projection_matrix, &cam->view_matrix, &modelViewClip );
	matrice4d_product_v3d( &modelViewClip, &planet->center, &v4d );
	vecteur4d_getVecteur3d( &v4d, &v3d );
	ss_planet_clipSpaceToScreenSpace( &v3d, &screenPt );
	
	// Trouver la taille du rayon
	planet_screen_dist = vecteur3d_distance( &planet->center, &cam->center );
	if( planet_screen_dist != 0 )
		planet_screen_radius = planet->radius * cam->zoomx*200000 / planet_screen_dist;
	else
		planet_screen_radius = 0;

	// Vecteurs de direction lumineuse de la balle
	// Ball vertex init
	vecteur3d_init(&w_v3d_planet_center, planet->center.x,planet->center.y,0);
	vecteur3d_init(&w_v3d_planet_top, 0,0,1*planet->radius);
	vecteur3d_add(&w_v3d_planet_top, &w_v3d_planet_center);
	vecteur3d_init(&w_v3d_planet_dir, -planet->center.x,-planet->center.y,0);
	vecteur3d_normalize(&w_v3d_planet_dir);
	vecteur3d_mul(&w_v3d_planet_dir, planet->radius);
	vecteur3d_add(&w_v3d_planet_dir, &w_v3d_planet_center);
	// Ball vertex transform
	matrice4d_product_v3d( &cam->view_matrix, &w_v3d_planet_top, &v4d );
	vecteur4d_getVecteur3d( &v4d, &s_v3d_planet_top );
	matrice4d_product_v3d( &cam->view_matrix, &w_v3d_planet_dir, &v4d );
	vecteur4d_getVecteur3d( &v4d, &s_v3d_planet_dir );
	matrice4d_product_v3d( &cam->view_matrix, &w_v3d_planet_center, &v4d );
	vecteur4d_getVecteur3d( &v4d, &s_v3d_planet_center );

	vecteur3d_init(&v3d_planet2sun, -s_v3d_planet_dir.x+s_v3d_planet_center.x, s_v3d_planet_dir.y-s_v3d_planet_center.y, s_v3d_planet_dir.z-s_v3d_planet_center.z );
	vecteur3d_init(&v3d_planet2sun_top, -s_v3d_planet_top.x+s_v3d_planet_center.x, s_v3d_planet_top.y-s_v3d_planet_center.y, s_v3d_planet_top.z-s_v3d_planet_center.z );	
	vecteur3d_normalize(&v3d_planet2sun);
	vecteur3d_normalize(&v3d_planet2sun_top);

	planet->screenCenter[planet->current_display_buff] = screenPt;
	planet->screenRadius[planet->current_display_buff] = (int)planet_screen_radius;
	planet->light_dir = v3d_planet2sun;
	planet->light_top = v3d_planet2sun_top;

	if( v4d.z > -1 && planet_screen_dist > 1)
		planet->visible[planet->current_display_buff] = 1;
	else
		planet->visible[planet->current_display_buff] = 0;

}

/********************************************************************
    ss_planet_draw()
  
    Dessine un planete à l'écran
    
  *******************************************************************/
void ss_planet_draw( ss_planet_t *planet ) {

	int buff = planet->current_display_buff;
	vecteur3d_t light_dir, light_top;

	if( planet->orbit_a != 0 && planet->visible[buff] == 1) {
		ss_planet_drawBall3D( planet->screenCenter[buff].x, planet->screenCenter[buff].y, planet->screenRadius[buff], planet->color, &planet->light_dir, &planet->light_top );
		//_planete_drawball_opt ( planet->screenCenter[buff].x, planet->screenCenter[buff].y, planet->screenRadius[buff], planet->color );
	}
	else if ( planet->visible[buff] == 1 ) {
		vecteur3d_init( &light_dir, 0,0,-1);
		vecteur3d_init( &light_top, 1,0,0);
		ss_planet_drawBall3D( planet->screenCenter[buff].x, planet->screenCenter[buff].y, planet->screenRadius[buff] /20, planet->color, &light_dir, &light_top );
		//_planete_drawball_opt ( planet->screenCenter[buff].x, planet->screenCenter[buff].y, planet->screenRadius[buff], planet->color );
	}
}

/********************************************************************
    ss_planet_clear()
  
    Efface une planète à l'écran
  *******************************************************************/
void ss_planet_clear( ss_planet_t *planet ) {

	int buff = planet->previous_display_buff;
	if( planet->orbit_a != 0 && planet->visible[buff] == 1){
		int x0     = planet->screenCenter[buff].x;
		int y0     = planet->screenCenter[buff].y;
		int radius = planet->screenRadius[buff];
		int color  = 0;

		//ss_planet_drawBall( planet->screenCenter[buff].x, planet->screenCenter[buff].y, planet->screenRadius[buff], 0 );
		_planete_drawball_opt(x0 , y0, radius, color );

	}
	else if ( planet->visible[buff] == 1 ){
		//ss_planet_drawBall( planet->screenCenter[buff].x, planet->screenCenter[buff].y, planet->screenRadius[buff] / 20, 0 );
		_planete_drawball_opt( planet->screenCenter[buff].x, planet->screenCenter[buff].y, planet->screenRadius[buff] / 20, 0 );
	}
}

/********************************************************************
    ss_planet_swapDisplayBuffer()
  
    Un tampon pour les coordonnées d'affichage courant, et l'autre
    pour les coordonnées d'affichage précédents.
    
    Utilisé lorsque l'écran ne dispose que d'un seul tampon mémoire.
    
  *******************************************************************/
void ss_planet_swapDisplayBuffer( ss_planet_t *planet ) {

	if( planet->current_display_buff == 0 ) {
		planet->current_display_buff = 1;
		planet->previous_display_buff = 0;
	}
	else {
		planet->current_display_buff = 0;
		planet->previous_display_buff = 1;
	}	
}

/********************************************************************
	ss_planet_drawBall()
	
	Draws a 2D Filled Circle
    
    Color (0-255)

  *******************************************************************/
void ss_planet_drawBall( int x0, int y0, int radius, int color ) {

	int i,j;
	int x=0;
    int y=radius;
    int m= 5-4*radius;

    while( x<=y ) {
             
        for( i=-x+x0; i< x+x0; i++ ){
            if( i < 0 || i > 640 ) continue;
            ecran2d_setPixel(i,y+y0, color );
            ecran2d_setPixel(i,-y+y0, color );
        }
        for( i=-y+x0; i< y+x0; i++ ){
            if( i < 0 || i > 640 ) continue;
            ecran2d_setPixel(i,x+y0, color );
            ecran2d_setPixel(i,-x+y0, color );
        }        
        if( m>0 ) {
            y--;
            m= m-8*y;
        }
        x++;
        m = m+8*x+4;
    } 
}

/********************************************************************
	ss_planet_drawBall3D()
	
	Draws a 3D Ball where vz is the new direction in space of the
	z axis, and vy is the new top axis. 

	Color is an index from 0-7 (white,red....)

  *******************************************************************/
void ss_planet_drawBall3D( int x0, int y0, int radius, int color, vecteur3d_t *vz, vecteur3d_t *vy ) {

	int x=0;
    int y=radius;
    int m= 5-4*radius;
	int i,j;
	int intensity;
	vecteur3d_t	vx;
	matrice4d_t	modelT;  
	int c = color;
    
	if( x0 -radius >= ECRAN2D_WIDTH || x0+radius < 0 || y0+radius <0 || y0-radius >= ECRAN2D_WIDTH ) return;

	vecteur3d_crossProduct(vz,vy,&vx);		
	matrice4d_setRotationMatrixXYZ( &modelT, vy, &vx, vz);
       	
	while( x<=y ) {
               		
        for( i=-x+x0; i< x+x0; i++ ){
			if( i < 0 || i >= ECRAN2D_WIDTH ) continue;
            intensity = ss_planet_getBall3DIntensity(i-x0, y, radius,&modelT);						
			ecran2d_setPixel(i,y+y0, (c<<5)+intensity );   
            intensity = ss_planet_getBall3DIntensity(i-x0, -y, radius,&modelT);			
			ecran2d_setPixel(i,-y+y0, (c<<5)+intensity );
        }
        for( i=-y+x0; i< y+x0; i++ ){
			if( i < 0 || i >= ECRAN2D_WIDTH ) continue;
            intensity = ss_planet_getBall3DIntensity(i-x0, x, radius,&modelT);
            ecran2d_setPixel(i,x+y0, (c<<5)+intensity );
            intensity = ss_planet_getBall3DIntensity(i-x0, -x, radius,&modelT);
            ecran2d_setPixel(i,-x+y0, (c<<5)+intensity );
        }
        if( m>0 ) {
            y--;
            m= m-8*y;
        }
        x++;
        m = m+8*x+4;
    }

}

/********************************************************************
	ss_planet_getBall3DReferenceIntensity()
	
	Calculates the intensity of a point (x,y,z) on the surface of a
	sphere (of radius = radius) where light is comming from the Z axis.

  *******************************************************************/
inline int  ss_planet_getBall3DReferenceIntensity( float x, float y, float z, int radius ) {

	// Balle 3D : R2 = (x-x0)2 + (y-y0)2 + (z-z0)2
    int 	intensity;
	int		intensity_min = 0;
    float	r = sqrt(x*x + y*y);
    
    if( z <= 0 )
        intensity = 30*sin(acos(r/(float)radius));   
    else
        intensity = intensity_min;     

    return (int)intensity;
}

/********************************************************************
	ss_planet_getBall3DIntensity()
	
	Calculates the light intensity of a point x,y by determining
	the corresponding reference ball point (x,y,z) using the given
	rotation transform

  *******************************************************************/
int  ss_planet_getBall3DIntensity( int x, int y, int radius, matrice4d_t *transform ) {

	vecteur4d_t ballPt, ballPtRef;
	float z;
 
	//matrice4d_setIdentity(transform);        // Balle de reference (test)
	z = sqrt((float)radius*radius - (x*x + y*y));
	vecteur4d_init( &ballPt, x,y,z,1 );
	matrice4d_product_v4d(transform, &ballPt, &ballPtRef );
  
	return ss_planet_getBall3DReferenceIntensity(ballPtRef.x, ballPtRef.y, ballPtRef.z, radius);	
}

/********************************************************************
    ss_planet_clipSpaceToScreenSpace()
  
  *******************************************************************/
void ss_planet_clipSpaceToScreenSpace( vecteur3d_t *pt, vecteur3d_t *result ) {

	float i,j;
	i = pt->x*ECRAN2D_WIDTH/2 + ECRAN2D_WIDTH/2;
	j = -pt->y*ECRAN2D_HEIGHT/2 + ECRAN2D_HEIGHT/2;
	vecteur3d_init(result, (int)i,(int)j, 0);
}


