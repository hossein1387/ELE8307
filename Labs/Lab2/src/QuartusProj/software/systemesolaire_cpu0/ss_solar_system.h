#ifndef SS_SOLAR_SYSTEM_H
#define SS_SOLAR_SYSTEM_H

#include "ecran2d.h"

#include "ss_solar_system_constants.h"
#include "ss_planet.h"
#include "ss_orbit.h"
#include "camera3d.h"
#include "vecteur3d.h"
#include "vecteur4d.h"

#include "matrice4d.h"
#include "math_constants.h"


#define SOLAR_SYSTEM_NB_PLANETS			9
#define SOLAR_SYSTEM_TIMESTEP_SEC		3600	


typedef struct {

		camera3d_t	camera;

		ss_orbit_t	orbites[SOLAR_SYSTEM_NB_PLANETS];

		ss_planet_t	planetes[SOLAR_SYSTEM_NB_PLANETS+1];		
		int			planetes_sorted_indexes[SOLAR_SYSTEM_NB_PLANETS+1];
		
		float	dVx[SOLAR_SYSTEM_NB_PLANETS];	// speedVariations
		float	dVy[SOLAR_SYSTEM_NB_PLANETS];

} ss_solar_system_t;


void ss_solar_system_init( ss_solar_system_t *system );
void ss_solar_system_iterate( ss_solar_system_t *system );

void ss_solar_system_calculateSpeedVariations( ss_solar_system_t *system );
void ss_solar_system_update( ss_solar_system_t *system );

void ss_solar_system_sortPlanets( ss_solar_system_t *system );

#endif
