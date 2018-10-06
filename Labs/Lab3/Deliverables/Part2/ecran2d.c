#include "ecran2d.h"

#ifdef M3D_WIN32_GLUT_DISPLAY
/*********************************************************
	ecran2d_setPixel()

 *********************************************************/
void ecran2d_setPixel( int x, int y, int color ) {
	
	ecran2d_setColorOpenGL( color );
	glBegin(GL_POINTS);
	glVertex2f(x, y);	
	glEnd();
}


/*********************************************************
	ecran2d_drawTriangle()

	Dessiner un triangle en tracant des lignes verticales
	de gauche a droite du triangle.

 *********************************************************/
void ecran2d_drawTriangle( triangle3d_t *tri ) {

	float a1,a2,a3,at, y1,y2;
	float dx1,dx2,dx3;
	float s1,s2,s3;
	int i,j;

	ecran2d_setColorOpenGL( tri->color );
	glBegin(GL_POINTS);

	triangle3d_sortPointsByX( tri );

	dx1 = (tri->pts[2].x - tri->pts[0].x);
	dx2 = (tri->pts[1].x - tri->pts[0].x);
	dx3 = (tri->pts[2].x - tri->pts[1].x);

	if( dx1 != 0 ) 
		a1 = (tri->pts[2].y - tri->pts[0].y) / dx1;	// pt[0]-pt[2]	; long side (in x)
	if( dx2 != 0 ) 
		a2 = (tri->pts[1].y - tri->pts[0].y) / dx2;	// pt[0]-pt[1]
	if( dx3 != 0 ) 
		a3 = (tri->pts[2].y - tri->pts[1].y) / dx3;	// pt[1]-pt[2]

	if( dx2 != 0 ) {

		y1 = tri->pts[0].y;
		y2 = y1;
		
		for( i= tri->pts[0].x ; i< tri->pts[1].x ; i++ ){	
			if( y1 < y2 ) {
				for( j=y1; j<y2; j++ )
					glVertex2f((float)i, (float)j);	
			}
			else {
				for( j=y2; j<y1; j++ )
					glVertex2f((float)i, (float)j);	
			}
			y1 += a1;
			y2 += a2;
		}		
		for( i= tri->pts[1].x ; i< tri->pts[2].x ; i++ ){			
			if( y1 < y2 ) {
				for( j=y1; j<y2; j++ )
					glVertex2f((float)i, (float)j);	
			}
			else {
				for( j=y2; j<y1; j++ )
					glVertex2f((float)i, (float)j);	
			}	
			y1 += a1;
			y2 += a3;				
		}			
	}
	else if ( dx2 == 0 ) {

		if( tri->pts[0].y < tri->pts[1].y ) {
			y1 = tri->pts[0].y;
			y2 = tri->pts[1].y;
			for( i= tri->pts[0].x ; i< tri->pts[2].x ; i++ ){			
				for( j=y1; j<y2; j++ )
					glVertex2f((float)i, (float)j);	
				y1 += a1;
				y2 += a3;
			}	
		}
		else {
			y1 = tri->pts[1].y;
			y2 = tri->pts[0].y;
			for( i= tri->pts[0].x ; i< tri->pts[2].x ; i++ ){			
				for( j=y1; j<y2; j++ )
					glVertex2f((float)i, (float)j);					
				y1 += a3;
				y2 += a1;
			}	
		}
	}

	glEnd();
}

/*********************************************************
	ecran2d_setColorOpenGL()

 *********************************************************/
static void ecran2d_setColorOpenGL( int color ) {

	float intensite;
	intensite = (color%32)/(31.0f);
	
	switch( color/32 ) {
		case COLOR_WHITE : 
			glColor3f(intensite, intensite, intensite);
			break;
		case COLOR_RED : 
			glColor3f(intensite, 0.0, 0.0);
			break;
		case COLOR_GREEN : 
			glColor3f(0.0, intensite, 0.0);
			break;
		case COLOR_BLUE : 
			glColor3f(0.0, 0.0, intensite);
			break;
		case COLOR_YELLOW : 
			glColor3f(intensite, intensite, 0.0);
			break;
		case COLOR_BROWN : 
			glColor3f(intensite*0.5, intensite*0.25, intensite*0.0625);
			break;
		case COLOR_CYAN : 
			glColor3f(0.0, intensite, intensite);
			break;
		case COLOR_PURPLE : 
			glColor3f(intensite, 0.0, intensite);
			break;	
		default :
			glColor3f(1.0,1.0,1.0);
			break;
	}
}

#else   // NIOS II 

void ecran2d_setPixel( int x, int y, int color ) {
	int addr;
	x =  ((y & 0x0000FFFF)<<16) + (x & 0x0000FFFF);
	addr = ALT_CI_SETPIXEL_0(x, color);
    //IOWR(0, addr,color);
}


void ecran2d_clear() {
    
    int i,j;
    for( i=0; i<ECRAN2D_WIDTH; i++ ){
        for( j=0; j<ECRAN2D_HEIGHT; j++ ){
            ecran2d_setPixel(i,j,0);      
        }
    } 
}

/*inline void ecran2d_setPixel( int x, int y, int color ) {
    //IOWR(VGA_0_BASE,(y*640)+x,color);
    //IOWR(VGA_0_BASE,(y<<9)+(y<<7)+x,color);
    ALT_CI_SETPIXEL_INST((y<<16) + x, color);
}*/

/*********************************************************
	ecran2d_drawTriangle()

	Dessiner un triangle en tracant des lignes verticales
	de gauche a droite du triangle.

 *********************************************************/
/*void ecran2d_drawTriangle( triangle3d_t *tri ) {

	float a1,a2,a3,at, y1,y2;
	float dx1,dx2,dx3;
	float s1,s2,s3;
	int i,j;

	//ecran2d_setColorOpenGL( tri->color );
	//glBegin(GL_POINTS);

	triangle3d_sortPointsByX( tri );

	dx1 = (tri->pts[2].x - tri->pts[0].x);
	dx2 = (tri->pts[1].x - tri->pts[0].x);
	dx3 = (tri->pts[2].x - tri->pts[1].x);

	if( dx1 != 0 ) 
		a1 = (tri->pts[2].y - tri->pts[0].y) / dx1;	// pt[0]-pt[2]	; long side (in x)
	if( dx2 != 0 ) 
		a2 = (tri->pts[1].y - tri->pts[0].y) / dx2;	// pt[0]-pt[1]
	if( dx3 != 0 ) 
		a3 = (tri->pts[2].y - tri->pts[1].y) / dx3;	// pt[1]-pt[2]

	if( dx2 != 0 ) {

		y1 = tri->pts[0].y;
		y2 = y1;
		
		for( i= tri->pts[0].x ; i< tri->pts[1].x ; i++ ){	
			if( y1 < y2 ) {
				for( j=y1; j<y2; j++ )
					ecran2d_setPixel(i,j,tri->color);					
			}
			else {
				for( j=y2; j<y1; j++ )
					ecran2d_setPixel(i,j,tri->color);
			}
			y1 += a1;
			y2 += a2;
		}		
		for( i= tri->pts[1].x ; i< tri->pts[2].x ; i++ ){			
			if( y1 < y2 ) {
				for( j=y1; j<y2; j++ )
					ecran2d_setPixel(i,j,tri->color);
			}
			else {
				for( j=y2; j<y1; j++ )
					ecran2d_setPixel(i,j,tri->color);
			}	
			y1 += a1;
			y2 += a3;				
		}			
	}
	else if ( dx2 == 0 ) {

		if( tri->pts[0].y < tri->pts[1].y ) {
			y1 = tri->pts[0].y;
			y2 = tri->pts[1].y;
			for( i= tri->pts[0].x ; i< tri->pts[2].x ; i++ ){			
				for( j=y1; j<y2; j++ )
					ecran2d_setPixel(i,j,tri->color);
				y1 += a1;
				y2 += a3;
			}	
		}
		else {
			y1 = tri->pts[1].y;
			y2 = tri->pts[0].y;
			for( i= tri->pts[0].x ; i< tri->pts[2].x ; i++ ){			
				for( j=y1; j<y2; j++ )
					ecran2d_setPixel(i,j,tri->color);
				y1 += a3;
				y2 += a1;
			}	
		}
	}

	//glEnd();
}
*/



#endif

