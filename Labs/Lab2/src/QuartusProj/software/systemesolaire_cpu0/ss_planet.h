#ifndef SS_PLANET_H
#define SS_PLANET_H

#include "ss_solar_system_constants.h"

#include "camera3d.h"
#include "vecteur3d.h"
#include "vecteur4d.h"
#include "ecran2d.h"
#include "matrice4d.h"
#include "math_constants.h"


#define GRAVITATIONAL_CONSTANT		6.67E-11	// Nm2/kg2


typedef struct {

	// Properties
	float			radius;	
	float			mass;
	int				color;
	float			orbit_a;

	// Physics
	vecteur3d_t 	center;
	vecteur3d_t		speed;
	
	// Display 
	vecteur3d_t		screenCenter[2];
	int				screenRadius[2];
	int				visible[2];
	int				current_display_buff, previous_display_buff;

	vecteur3d_t		light_dir, light_top;
    
} ss_planet_t;


void ss_planet_init( ss_planet_t *planet, float mass, float radius, float orbit_a, float orbit_t, int color );
void ss_planet_updatePos( ss_planet_t *planet, float dt );
void ss_planet_updateSpeed( ss_planet_t *planet, float dVx, float dVy );

void ss_planet_render( ss_planet_t *planet, camera3d_t *cam );

void ss_planet_draw( ss_planet_t *planet );
void ss_planet_clear( ss_planet_t *planet );
void ss_planet_swapDisplayBuffer( ss_planet_t *planet );

void ss_planet_drawBall( int x0, int y0, int radius, int color );
void ss_planet_drawBall3D( int x0, int y0, int radius, int color, vecteur3d_t *vz, vecteur3d_t *vy );
inline int  ss_planet_getBall3DReferenceIntensity( float x, float y, float z, int radius );
int  ss_planet_getBall3DIntensity( int x, int y, int radius, matrice4d_t *transform );

void ss_planet_clipSpaceToScreenSpace( vecteur3d_t *pt, vecteur3d_t *result );

extern void _planete_drawball_opt(int x0, int y0, int radius, int color);
#endif //SS_PLANETE_H

