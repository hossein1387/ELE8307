#include "system.h"
#define ECRAN2D_WIDTH			640
#define ECRAN2D_HEIGHT			480

typedef struct {

	int x0,y0,x1,y1;

} ss_orbit_line_t;			// Orbit Screen line segments


void ecran2d_setPixel( int x, int y, int color ) {
   // IOWR(VGA_0_BASE,(y*640)+x,color);
	int addr;
	x     =  ((y & 0x000003FF)<<10) + (x & 0x000003FF);
	color =  ((color & 0x000003FF)<<10) + (0 & 0x000003FF);
	addr  =  ALT_CI_DRAWLINE_0(0, x, color);
}

void ecran2d_clear() {

    int i,j;
    for( i=0; i<ECRAN2D_WIDTH; i++ ){
        for( j=0; j<ECRAN2D_HEIGHT; j++ ){
            ecran2d_setPixel(i,j,0);
        }
    }
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
void ss_orbit_line_draw( ss_orbit_line_t *line, int color, int with_specialization ) {

	int word1, word2, ret;
	int x0,y0,x1,y1;
    int Dx,Dy, steep;
    int ystep, xstep,TwoDy, TwoDyTwoDx, E, xDraw, yDraw, x,y;

    if(with_specialization==1)
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


void draw_cross(int with_specialization)
{
	ss_orbit_line_t line;
	int color = 255;
	ecran2d_clear();
	line.x0 = 0;
	line.y0 = 0;
	line.x1 = 640;
	line.y1 = 480;
	ss_orbit_line_draw(&line, color, with_specialization);
	line.x0 = 640;
	line.y0 = 0;
	line.x1 = 0;
	line.y1 = 480;
	ss_orbit_line_draw(&line, color, with_specialization);
}

int main()
{
    draw_cross(1);
    
    exit(0);
    
 }
