#include "ss_orbit.h"

#define LINECLIP_CODE_RIGHT		8
#define	LINECLIP_CODE_TOP		4
#define LINECLIP_CODE_LEFT		2
#define	LINECLIP_CODE_BOTTOM	1

/********************************************************************
	ss_orbit_init()
 *******************************************************************/
void ss_orbit_init( ss_orbit_t *orbit, float orbit_a ) {

	int i;
	float angle;

	// Planet Orbit points
	for( i=0; i< SS_ORBIT_NB_DIV; i++ ) {
		angle = 2*PI*(i/(float)SS_ORBIT_NB_DIV);
		vecteur3d_init( &orbit->orbit_pts[i], orbit_a*cos( angle ), orbit_a*sin( angle ), 0 );
	}

	orbit->nb_screen_lines[0] = 0;
	orbit->nb_screen_lines[1] = 0;	
	orbit->current_display_buff = 0;
	orbit->previous_display_buff = 1;	
}
	
/********************************************************************

	Renders the orbit pts to screen lines in the current display 
	buffer.

 *******************************************************************/
void ss_orbit_render( ss_orbit_t *orbit, camera3d_t	*camera ) {

	int				i;
	vecteur3d_t		s_orbit_pts[SS_ORBIT_NB_DIV];
	vecteur4d_t		s4_orbit_pts[SS_ORBIT_NB_DIV];
	vecteur3d_t		v3d;
	vecteur4d_t		v4d;
	matrice4d_t		modelViewClip;

	ss_orbit_line_t	orbitLine;

	// Transform orbit points from space to screen
	for( i=0; i<SS_ORBIT_NB_DIV; i++ ) {

		matrice4d_product_m4d( &camera->projection_matrix, &camera->view_matrix, &modelViewClip );
		matrice4d_product_v3d( &modelViewClip, &orbit->orbit_pts[i], &v4d );
		s4_orbit_pts[i] = v4d;
		vecteur4d_getVecteur3d( &v4d, &v3d );
		ss_orbit_clipSpaceToScreenSpace( &v3d, &s_orbit_pts[i] );	
	}		

	// Clip lines
	for( i=0; i<SS_ORBIT_NB_DIV-1; i++ ) {
		if( s4_orbit_pts[i].z > -1 && s4_orbit_pts[i+1].z > -1) {
			ss_orbit_line_init( &orbitLine, s_orbit_pts[i].x, s_orbit_pts[i].y, s_orbit_pts[i+1].x, s_orbit_pts[i+1].y );
			if(ss_orbit_line_clip(&orbitLine) == 1 ) {
				ss_orbit_addLine(orbit, &orbitLine);
			}
		}
		
	}
	if( s4_orbit_pts[i].z > -1 && s4_orbit_pts[0].z > -1) {
		ss_orbit_line_init( &orbitLine, s_orbit_pts[i].x, s_orbit_pts[i].y, s_orbit_pts[0].x, s_orbit_pts[0].y );
		if(ss_orbit_line_clip(&orbitLine) == 1 ) {
			ss_orbit_addLine(orbit, &orbitLine);
		}
	}

}

/********************************************************************
	ss_orbit_addLine()
 *******************************************************************/
void ss_orbit_addLine( ss_orbit_t *orbit, ss_orbit_line_t *line ) {

	int buff = orbit->current_display_buff;

	orbit->screen_lines[buff][orbit->nb_screen_lines[buff]] = *line;
	orbit->nb_screen_lines[buff]++;
}

/********************************************************************
	ss_orbit_draw()
 *******************************************************************/
void ss_orbit_draw( ss_orbit_t *orbit ) {

	int i;
	int buff = orbit->current_display_buff;
	
	for( i=0; i< orbit->nb_screen_lines[buff]; i++ ) {
		ss_orbit_line_draw( &orbit->screen_lines[buff][i], SS_ORBIT_COLOR );
	}
}

/********************************************************************
	ss_orbit_clear()
 *******************************************************************/
void ss_orbit_clear( ss_orbit_t *orbit ) {

	int i;
	int buff = orbit->previous_display_buff;
	
	for( i=0; i< orbit->nb_screen_lines[buff]; i++ ) {
		ss_orbit_line_draw( &orbit->screen_lines[buff][i], 0 );
	}
}

/********************************************************************
	ss_orbit_swapDisplayBuffer()
 *******************************************************************/
void ss_orbit_swapDisplayBuffer( ss_orbit_t *orbit ) {

	if( orbit->current_display_buff == 0 ) {
		orbit->current_display_buff = 1;
		orbit->previous_display_buff = 0;
	}
	else {
		orbit->current_display_buff = 0;
		orbit->previous_display_buff = 1;
	}
	orbit->nb_screen_lines[orbit->current_display_buff] = 0;
}

/********************************************************************
	ss_orbit_line_draw()
 *******************************************************************/
void ss_orbit_line_init( ss_orbit_line_t *line, int x0, int y0, int x1, int y1 ) {

	line->x0 = x0;
	line->x1 = x1;
	line->y0 = y0;
	line->y1 = y1;
}



int abs( int a ) { if( a>=0 ) return a; else return -a; }

void swap( int *a, int *b) {
    int t;
    t = *a;
    *a = *b;
    *b = t;
}

/********************************************************************
	ss_orbit_line_draw()
	
	- 8bit color	(color_index<<5)+intensity
  *******************************************************************/
void ss_orbit_line_draw( ss_orbit_line_t *line, int color ) {

	int word1, word2, ret;
	int x0,y0,x1,y1;
    int Dx,Dy, steep;
    int ystep, xstep,TwoDy, TwoDyTwoDx, E, xDraw, yDraw, x,y;

    if(color==255)
    {
    	x0 = line->x0;
    	x1 = line->x1;
    	y0 = line->y0;
    	y1 = line->y1;
    	word1 =  ((x1 & 0x000003FF)<<20) + ((y0 & 0x000003FF)<<10) + (x0 & 0x000003FF);
    	word2 =  ((color & 0x000003FF)<<10) + (y1 & 0x000003FF);
    	ret   =  ALT_CI_DRAWLINE_0(1, word1, word2);
    } else {
        x0 = line->x0;
        x1 = line->x1;
        y0 = line->y0;
        y1 = line->y1;

        Dx = x1 - x0;
        Dy = y1 - y0;
        steep = (abs(Dy) >= abs(Dx));
        if (steep) {
            swap(&x0, &y0);
            swap(&x1, &y1);
            // recompute Dx, Dy after swap
            Dx = x1 - x0;
            Dy = y1 - y0;
        }
        xstep = 1;
        if (Dx < 0) {
            xstep = -1;
            Dx = -Dx;
        }
        ystep = 1;
        if (Dy < 0) {
            ystep = -1;
            Dy = -Dy;
        }
        TwoDy = 2*Dy;
        TwoDyTwoDx = TwoDy - 2*Dx;  // 2*Dy - 2*Dx
        E = TwoDy - Dx;             //2*Dy - Dx
        y = y0;

        for ( x = x0; x != x1; x += xstep) {
            if (steep) {
                xDraw = y;
                yDraw = x;
            }
            else {
                xDraw = x;
                yDraw = y;
            }
            ecran2d_setPixel( xDraw,yDraw, color);

            if (E > 0) {
                E += TwoDyTwoDx; //E += 2*Dy - 2*Dx;
                y = y + ystep;
            }
            else {
                E += TwoDy; //E += 2*Dy;
            }
        }
    }
}


/*************************************************************
	ss_orbit_line_clip

	returns '1' if in screen (need to be drawn)
			'0' if rejected
  ************************************************************/
int ss_orbit_line_clip( ss_orbit_line_t *line ) {

	int	code0, code1, outCode;
	int	accept = 0;
	int done = 0;
	float x,y;

	int xmin=0, ymin=0, xmax=ECRAN2D_WIDTH, ymax=ECRAN2D_HEIGHT;

	int x0, y0, x1, y1;
	x0 = line->x0;
	y0 = line->y0;
	x1 = line->x1;
	y1 = line->y1;	

	code0 = ss_orbit_line_computeClipCode(x0,y0);
	code1 = ss_orbit_line_computeClipCode(x1,y1);

	do {
		if( (code0|code1) == 0 ) {
			accept = 1;
			done = 1;
		}
		else if( (code0 & code1) ) {
			done = 1;
		}
		else {
			if( code0 ) 
				outCode = code0;
			else
				outCode = code1;

			if( outCode & LINECLIP_CODE_TOP ) {
				x = x0 + (x1-x0) * (ymax-y0)/(y1-y0);
				y = ymax;
			}
			else if( outCode & LINECLIP_CODE_BOTTOM ) {
				x = x0 + (x1-x0) * (ymin-y0)/(y1-y0);
				y = ymin;
			}
			else if( outCode & LINECLIP_CODE_RIGHT ) {
				y = y0 + (y1-y0) * (xmax-x0)/(x1-x0);
				x = xmax;
			}
			else  {
				y = y0 + (y1-y0) * (xmin-x0)/(x1-x0);
				x = xmin;
			}

			if( outCode == code0 ) {
				x0 = x;
				y0 = y;
				code0 = ss_orbit_line_computeClipCode(x0,y0);
			}
			else {
				x1 = x;
				y1 = y;
				code1 = ss_orbit_line_computeClipCode(x1,y1);
			}

		}
	}
	while ( done != 1 );

	if( accept ) {
		//ss_solar_system_drawLine( x0,y0,x1,y1, color );
		line->x0 = x0;
		line->y0 = y0;
		line->x1 = x1;
		line->y1 = y1;	
		return 1;
	}
	return 0;
}


int	ss_orbit_line_computeClipCode( float x, float y ) {

	int code = 0;
	if ( y > ECRAN2D_HEIGHT )		code |= LINECLIP_CODE_TOP;
	else if ( y < 0 )				code |= LINECLIP_CODE_BOTTOM;

	if ( x > ECRAN2D_WIDTH )		code |= LINECLIP_CODE_RIGHT;
	else if ( x < 0 )				code |= LINECLIP_CODE_LEFT;
	return code;
}


/********************************************************************
	ss_solar_system_clipSpaceToScreenSpace()
	
	Converts clip-space points to screen coordinates.

  *******************************************************************/
void ss_orbit_clipSpaceToScreenSpace( vecteur3d_t *pt, vecteur3d_t *result ) {

	float i,j;
	i = pt->x*ECRAN2D_WIDTH/2 + ECRAN2D_WIDTH/2;
	j = -pt->y*ECRAN2D_HEIGHT/2 + ECRAN2D_HEIGHT/2;
	vecteur3d_init(result, (int)i,(int)j, 0);
} 

