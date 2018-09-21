#include "ss_solar_system.h"


#define LINECLIP_CODE_RIGHT		8
#define	LINECLIP_CODE_TOP		4
#define LINECLIP_CODE_LEFT		2
#define	LINECLIP_CODE_BOTTOM	1


/********************************************************************
	ss_solar_system_init()
	
	Solar system initialization. Sets all planets and the sun. 

  *******************************************************************/
void ss_solar_system_init( ss_solar_system_t *ss ) {

	int i;
	// Initialize orbits
	ss_orbit_init(&ss->orbites[0], MERCURE_ORBIT_A);
	ss_orbit_init(&ss->orbites[1], VENUS_ORBIT_A);
	ss_orbit_init(&ss->orbites[2], TERRE_ORBIT_A);
	ss_orbit_init(&ss->orbites[3], MARS_ORBIT_A);
	ss_orbit_init(&ss->orbites[4], JUPITER_ORBIT_A);
	ss_orbit_init(&ss->orbites[5], SATURNE_ORBIT_A);
	ss_orbit_init(&ss->orbites[6], URANUS_ORBIT_A);
	ss_orbit_init(&ss->orbites[7], NEPTUNE_ORBIT_A);
	ss_orbit_init(&ss->orbites[8], PLUTON_ORBIT_A);

	// Initializes planets
	ss_planet_init( &ss->planetes[0], MERCURE_RELATIVE_MASS*EARTH_MASS, MERCURE_RAYON,	MERCURE_ORBIT_A,	MERCURE_ORBIT_T,	COLOR_RED );
	ss_planet_init( &ss->planetes[1], VENUS_RELATIVE_MASS*EARTH_MASS,	VENUS_RAYON,	VENUS_ORBIT_A,		VENUS_ORBIT_T,		COLOR_GREEN );
	ss_planet_init( &ss->planetes[2], TERRE_RELATIVE_MASS*EARTH_MASS,	TERRE_RAYON,	TERRE_ORBIT_A,		TERRE_ORBIT_T,		COLOR_BLUE );
	ss_planet_init( &ss->planetes[3], MARS_RELATIVE_MASS*EARTH_MASS,	MARS_RAYON,		MARS_ORBIT_A,		MARS_ORBIT_T,		COLOR_RED );
	ss_planet_init( &ss->planetes[4], JUPITER_RELATIVE_MASS*EARTH_MASS, JUPITER_RAYON,	JUPITER_ORBIT_A,	JUPITER_ORBIT_T,	COLOR_BROWN );
	ss_planet_init( &ss->planetes[5], SATURN_RELATIVE_MASS*EARTH_MASS,	SATURNE_RAYON,	SATURNE_ORBIT_A,	SATURNE_ORBIT_T,	COLOR_YELLOW );
	ss_planet_init( &ss->planetes[6], URANUS_RELATIVE_MASS*EARTH_MASS,	URANUS_RAYON,	URANUS_ORBIT_A,		URANUS_ORBIT_T,		COLOR_CYAN );
	ss_planet_init( &ss->planetes[7], NEPTUNE_RELATIVE_MASS*EARTH_MASS, NEPTUNE_RAYON,	NEPTUNE_ORBIT_A,	NEPTUNE_ORBIT_T,	COLOR_PURPLE );
	ss_planet_init( &ss->planetes[8], PLUTON_RELATIVE_MASS*EARTH_MASS,	PLUTON_RAYON,	PLUTON_ORBIT_A,		PLUTON_ORBIT_T,		COLOR_WHITE );
	// Sun as the "+1" planet
	ss_planet_init( &ss->planetes[9], SOLEIL_RELATIVE_MASS*EARTH_MASS,	SOLEIL_RAYON,	0,					0,					COLOR_YELLOW );
	

	// Initialize Camera
	camera3d_init( &ss->camera );

}

/********************************************************************
	ss_solar_system_iterate()
	
	
  *******************************************************************/
void ss_solar_system_iterate( ss_solar_system_t *system ) {

	int i;
	int sorted_planet_index;

	// Transformer les orbites en lignes pretes pour l'ecran
	for( i=0; i< SOLAR_SYSTEM_NB_PLANETS; i++ ) {
		ss_orbit_render( &system->orbites[i], &system->camera );
	}

	// Determiner les params d'affichage des planetes
	for( i=0; i< SOLAR_SYSTEM_NB_PLANETS+1; i++ ) {
		ss_planet_render( &system->planetes[i], &system->camera );
	}

	ss_solar_system_sortPlanets(system);
	ss_solar_system_calculateSpeedVariations(system);
	ss_solar_system_update(system);

	// Redessiner les orbites
	for( i=0; i< SOLAR_SYSTEM_NB_PLANETS; i++ ) {
		ss_orbit_clear( &system->orbites[i] );
		ss_orbit_draw( &system->orbites[i] );
		ss_orbit_swapDisplayBuffer(&system->orbites[i]);
	}

	// Redessiner les planetes
	for( i=0; i< SOLAR_SYSTEM_NB_PLANETS+1; i++ ) {
		sorted_planet_index = system->planetes_sorted_indexes[i];
		ss_planet_clear( &system->planetes[sorted_planet_index] );
		ss_planet_draw( &system->planetes[sorted_planet_index] );
		ss_planet_swapDisplayBuffer(&system->planetes[sorted_planet_index]);
		
	}
	
}

/********************************************************************
    ss_solar_system_calculateSpeedVariations()
    
    
  *******************************************************************/
void ss_solar_system_calculateSpeedVariations( ss_solar_system_t *system ) {

	int i,j;
	float dV, dVx, dVy;
	vecteur3d_t	u;		// f dir
	vecteur3d_t v1,v2;
	float vx, vy;
	float r;

	for( i=0; i< SOLAR_SYSTEM_NB_PLANETS; i++ ) {

		r = vecteur3d_length(&system->planetes[i].center);
		u = system->planetes[i].center;
		vecteur3d_negate(&u);
		vecteur3d_normalize(&u);

		dV = GRAVITATIONAL_CONSTANT*(EARTH_MASS*SOLEIL_RELATIVE_MASS)/(r*r);
		system->dVx[i] = u.x * dV * SOLAR_SYSTEM_TIMESTEP_SEC;
		system->dVy[i] = u.y * dV * SOLAR_SYSTEM_TIMESTEP_SEC;

		for( j=0; j<SOLAR_SYSTEM_NB_PLANETS; j++ ) {
			if( i != j ) {
				u = system->planetes[j].center;
				vecteur3d_sub(&u, &system->planetes[i].center);
				r = vecteur3d_length(&u);
				vecteur3d_normalize(&u);
				dV = GRAVITATIONAL_CONSTANT*(system->planetes[j].mass)/(r*r);
				system->dVx[i] += u.x * dV * SOLAR_SYSTEM_TIMESTEP_SEC;
				system->dVy[i] += u.y * dV * SOLAR_SYSTEM_TIMESTEP_SEC;
			}
		}
	}

}


/********************************************************************
	ss_solar_system_update()
	
	Updates the positions of each planet. 

  *******************************************************************/
void ss_solar_system_update( ss_solar_system_t *ss ) {

	int i,j;
	float dt_sec = 3600;

	for(i=0; i<SOLAR_SYSTEM_NB_PLANETS; i++) {
		ss_planet_updateSpeed( &ss->planetes[i], ss->dVx[i], ss->dVy[i] );
		ss_planet_updatePos( &ss->planetes[i], SOLAR_SYSTEM_TIMESTEP_SEC );
	}
}

/********************************************************************
	ss_solar_system_sortPlanets()
	
	Sorts the planet by distance from cam to planet. (desc)

  *******************************************************************/
void ss_solar_system_sortPlanets( ss_solar_system_t *system ) {

	int i,n,swapped,t;
	float d;
	float planet_dist[SOLAR_SYSTEM_NB_PLANETS+1];

	for( i=0; i<SOLAR_SYSTEM_NB_PLANETS+1; i++ ) {
		system->planetes_sorted_indexes[i] = i;
		planet_dist[i] = vecteur3d_distance( &system->camera.center, &system->planetes[i].center );
	}

	n = SOLAR_SYSTEM_NB_PLANETS+1;
	do {
		swapped = 0;
		n--;
		for( i=0; i<n; i++ ){
			
			if( planet_dist[i+1] > planet_dist[i] ) {
				
				d = planet_dist[i];
				planet_dist[i] = planet_dist[i+1];
				planet_dist[i+1] = d;
				t = system->planetes_sorted_indexes[i];
				system->planetes_sorted_indexes[i] = system->planetes_sorted_indexes[i+1];
				system->planetes_sorted_indexes[i+1] = t;
				swapped = 1;
			}
		}
	}
	while ( swapped == 1 );

}


