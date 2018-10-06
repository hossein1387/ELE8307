#ifndef ECRAN2D_H
#define ECRAN2D_H

//#include "triangle3d.h"

#ifdef M3D_WIN32_GLUT_DISPLAY
#include "GL/glut.h"
#else 
#include "system.h"
#include "io.h"
#endif

/******************************************************************
	ECRAN2D_H

	

 ******************************************************************/

#define ECRAN2D_WIDTH			640
#define ECRAN2D_HEIGHT			480
#define ECRAN2D_ASPECTRATIO		1.333333333
#define ECRAN2D_CLEARCOLOR		0

// COLOR MODEL			COLOR_INDEX(3) & COLOR_INTENSITY(5)

#define COLOR_WHITE			0
#define COLOR_RED			1	//63
#define COLOR_GREEN			2
#define COLOR_BLUE			3	//127
#define COLOR_YELLOW		4	//159
#define COLOR_BROWN			5	//191
#define COLOR_CYAN			6	//223
#define COLOR_PURPLE		7	//255



//void ecran2d_drawTriangle( triangle3d_t *t );
void ecran2d_setPixel( int x, int y, int color );


#ifdef M3D_WIN32_GLUT_DISPLAY
static void ecran2d_setColorOpenGL( int color );
#else
//#define ecran2d_setPixel(x,y,color)   IOWR(VGA_0_BASE,(y*640)+x,color)     //grrrrr   
#endif

void ecran2d_clear( void );

#endif // ECRAN2D_H
