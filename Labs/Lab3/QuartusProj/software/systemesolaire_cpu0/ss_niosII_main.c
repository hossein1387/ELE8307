
#include "stdio.h"
#include <stdlib.h> 

#include "ss_solar_system.h"

// ------------------------------------------------------------------
// VARIABLES GLOBALES
// ------------------------------------------------------------------
ss_solar_system_t   systemeSolaire;


/********************************************************************
    init()
 ********************************************************************/
void init(void) {   
 
    int i;
    
    ecran2d_clear(); 
    ss_solar_system_init(&systemeSolaire);  
}

/********************************************************************
    display()
 ********************************************************************/
void display(void) {    
    
    ss_solar_system_iterate(&systemeSolaire);    
}


void draw_cross(int with_specialization)
{
	ss_orbit_line_t line;
	int color = (with_specialization==0) ? 254 : 255;
	ecran2d_clear();
	line.x0 = 0;
	line.y0 = 0;
	line.x1 = 640;
	line.y1 = 480;
	ss_orbit_line_draw(&line, color);
	line.x0 = 640;
	line.y0 = 0;
	line.x1 = 0;
	line.y1 = 480;
	ss_orbit_line_draw(&line, color);
}

/********************************************************************
    main()
 ********************************************************************/
 int main() 
 {
    
     int i;
     printf("Entering main()... \n");
     
     init();
     
/*
     //for( ;; ) {          
     for( i=0; i<1; i++ ) {
        display();
     } 

     printf("Exiting main()... \n");
     
*/

     draw_cross(1);

     exit(0);
     //return 0;
 } 


