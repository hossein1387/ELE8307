#ifndef SS_ORBIT_H
#define SS_ORBIT_H

#include "math.h"

#include "camera3d.h"
#include "vecteur3d.h"
#include "vecteur4d.h"
#include "ecran2d.h"
#include "matrice4d.h"
#include "math_constants.h"


#define SS_ORBIT_NB_DIV		64
#define SS_ORBIT_COLOR		15

typedef struct {

	int x0,y0,x1,y1;

} ss_orbit_line_t;			// Orbit Screen line segments


typedef struct {

	// Space points
	vecteur3d_t			orbit_pts[SS_ORBIT_NB_DIV];		
	
	// Screen Display
	ss_orbit_line_t		screen_lines[2][SS_ORBIT_NB_DIV];	
	int					nb_screen_lines[2];
	int					current_display_buff;
	int					previous_display_buff;

}	ss_orbit_t;

void ss_orbit_init( ss_orbit_t *orbit, float orbit_a );

void ss_orbit_render( ss_orbit_t *orbit, camera3d_t	*cam );
void ss_orbit_draw( ss_orbit_t *orbit );
void ss_orbit_clear( ss_orbit_t *orbit );
void ss_orbit_swapDisplayBuffer( ss_orbit_t *orbit );

void ss_orbit_addLine( ss_orbit_t *orbit, ss_orbit_line_t *line );

void ss_orbit_line_init( ss_orbit_line_t *line, int x0, int y0, int x1, int y1 );
void ss_orbit_line_draw( ss_orbit_line_t *line, int color );
int  ss_orbit_line_clip( ss_orbit_line_t *line );
int	 ss_orbit_line_computeClipCode( float x, float y );


void ss_orbit_clipSpaceToScreenSpace( vecteur3d_t *pt, vecteur3d_t *result );


#endif
