
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



/********************************************************************
    main()
 ********************************************************************/
 int main() 
 {
    
     int i;
     printf("Entering main()... \n");
     
     init();
     
     //for( ;; ) {          
     for( i=0; i<1; i++ ) {
        display();
     } 

     printf("Exiting main()... \n");
     
     
     exit(0);
     //return 0;
 } 

