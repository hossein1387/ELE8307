

#include "stdio.h"
#include <stdlib.h> 

//#include "moteur3d.h"

#include "ss_solar_system.h"

#include "windows.h"
#include "GL/glut.h"

// ------------------------------------------------------------------
// VARIABLES GLOBALES
// ------------------------------------------------------------------
//scene3d_t			scene3d;
ss_solar_system_t	systemeSolaire;

int			time_ms;


/********************************************************************
	init()

	Fonction d'initialisation d'OpenGL
 ********************************************************************/
void init(void)
{
	objet3d_t	obj[16];
	lumiere3d_t  light[4];
	vecteur3d_t center, v1,v2;
	vecteur3d_t pts[4];
	triangle3d_t tri1;
	int i;
	
	// OPENGL //
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (0, 640, 480, 0, 0, 1);
	glMatrixMode (GL_MODELVIEW);
	glShadeModel(GL_FLAT);

	glClear(GL_COLOR_BUFFER_BIT);
	
	time_ms = glutGet(GLUT_ELAPSED_TIME);

	// SCENE3D //
	ss_solar_system_init(&systemeSolaire);
	

}

/********************************************************************
	display()

	Fonction d'affichage openGL
 ********************************************************************/
void display(void)
{
	int i;

	if( glutGet(GLUT_ELAPSED_TIME) - time_ms > 30 ) {
		//printf("time is %i ms \n", glutGet(GLUT_ELAPSED_TIME));
		time_ms = glutGet(GLUT_ELAPSED_TIME);
		//camera3d_rotateBank( &scene3d.camera, -0.1 );		
	}
	
	//glClear(GL_COLOR_BUFFER_BIT);
	
	// Update
	/*ss_solar_system_update(&systemeSolaire);
	
	for( i=0; i<SOLAR_SYSTEM_NB_PLANETS; i++ ){
		ss_solar_system_drawOrbit(&systemeSolaire, i);
	}

	ss_solar_system_sortPlanets(&systemeSolaire);
	for( i=0; i<SOLAR_SYSTEM_NB_PLANETS+1; i++ ){
		ss_solar_system_drawPlanet(&systemeSolaire, systemeSolaire.planetes_sorted_indexes[i]);		
	}*/

	ss_solar_system_iterate(&systemeSolaire);

	//ss_solar_system_drawPlanet(&systemeSolaire, -1);		// Draw Sun
	//ss_solar_system_drawBall(&systemeSolaire, 100,100, 10, 31);
	//ss_solar_system_drawBall3D( &systemeSolaire, 100, 100, 50, 31, &systemeSolaire.camera.dir, &systemeSolaire.camera.top );
	
	//ss_solar_system_drawLine(0,0,639,479, 31);

	//glutSwapBuffers();
}

/********************************************************************
	processNormalKeys()

	Ecouteur clavier
 ********************************************************************/
void processNormalKeys( unsigned char key, int x , int y ) {

	switch(key) {
		case 'w' : 
				//printf("W \n");
				camera3d_translateAlongDir(&systemeSolaire.camera, 50000000000);
				glutPostRedisplay();
				break;
		case 's' : 
				//printf("S \n");
				camera3d_translateAlongDir(&systemeSolaire.camera, -50000000000);
				glutPostRedisplay();
				break;		
		case 'a' : 
				//printf("A \n");
				camera3d_translateAlongSide(&systemeSolaire.camera, -10);
				glutPostRedisplay();
				break;
		case 'd' : 
				//printf("D \n");
				camera3d_translateAlongSide(&systemeSolaire.camera, 10);
				glutPostRedisplay();
				break;	
		case 'q' : 
				//printf("Q \n");
				camera3d_rotateHeading(&systemeSolaire.camera, -0.1);
				glutPostRedisplay();
				break;
		case 'e' : 
				//printf("E \n");
				camera3d_rotateHeading(&systemeSolaire.camera, 0.1);
				glutPostRedisplay();
				break;	
	}
}

/********************************************************************
	processSpecialKeys()

	Ecouteur clavier pour touches speciales

	GLUT_KEY_F1			F1 function key
	GLUT_KEY_F2			F2 function key
	GLUT_KEY_F3			F3 function key
	GLUT_KEY_F4			F4 function key
	GLUT_KEY_F5			F5 function key
	GLUT_KEY_F6			F6 function key
	GLUT_KEY_F7			F7 function key
	GLUT_KEY_F8			F8 function key
	GLUT_KEY_F9			F9 function key
	GLUT_KEY_F10		F10 function key
	GLUT_KEY_F11		F11 function key
	GLUT_KEY_F12		F12 function key
	GLUT_KEY_LEFT		Left function key
	GLUT_KEY_RIGHT		Up function key
	GLUT_KEY_UP			Right function key
	GLUT_KEY_DOWN		Down function key
	GLUT_KEY_PAGE_UP	Page Up function key
	GLUT_KEY_PAGE_DOWN	Page Down function key
	GLUT_KEY_HOME		Home function key
	GLUT_KEY_END		End function key
	GLUT_KEY_INSERT		Insert function key

 ********************************************************************/
void processSpecialKeys( int key, int x , int y ) {

	switch(key) {
		case GLUT_KEY_LEFT : 
				//printf("Left \n");
				camera3d_rotateBank( &systemeSolaire.camera, 0.1 );
				glutPostRedisplay();
				break;
		case GLUT_KEY_RIGHT : 
				//printf("Right \n");
				camera3d_rotateBank( &systemeSolaire.camera, -0.1 );
				glutPostRedisplay();
				break;
		case GLUT_KEY_UP : 
				//printf("Up \n");
				camera3d_rotatePitch( &systemeSolaire.camera, 0.1 );
				glutPostRedisplay();
				break;
		case GLUT_KEY_DOWN : 
				//printf("Down \n");
				camera3d_rotatePitch( &systemeSolaire.camera, -0.1 );
				glutPostRedisplay();
				break;
		case GLUT_KEY_PAGE_UP : 
				//printf("PgUp \n");
				camera3d_translateAlongTop( &systemeSolaire.camera, 500000000 );
				glutPostRedisplay();
				break;
		case GLUT_KEY_PAGE_DOWN : 
				//printf("PgDown \n");
				camera3d_translateAlongTop( &systemeSolaire.camera, -500000000 );
				glutPostRedisplay();
				break;
	}

}




/********************************************************************
	


 ********************************************************************/
void run_loop( void ) {
	
	glutPostRedisplay();

}



/********************************************************************
	main()


 ********************************************************************/
 int main(int argc, char **argv)
 {
	 //on initialise glut
	 glutInit(&argc, argv);

	 //on spécifie l'emploi du modèle chromatique et du double tampon
	 //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );		// Single buffering


	 //on spécifie la taille de la fenêtre
	 glutInitWindowSize(640, 480);

	 //on spécifie la position de la fenêtre
	 glutInitWindowPosition(500, 100);

	 //on crée une fenêtre
	 glutCreateWindow("ELE8307:: SYSTEME SOLAIRE 3D");

	 //on initialise
	 init();

	 ////////////// FONCTIONS DE CALLBACKS ////////////////////////

	 //
	 // Les fonctions de callbacks sont des fonctions que le programmeur va spécifier pour
	 // la lier à un événement (clic, touche appuyée, redimensionnement, etc).
	 //

	 glutDisplayFunc(display);
	 glutIdleFunc( run_loop );
	 glutKeyboardFunc(processNormalKeys);
	 glutSpecialFunc(processSpecialKeys);
	 //void glutKeyboardUpFunc(void (*func)(unsigned char key,int x,int y));
	 //void glutSpecialUpFunc(void (*func)(int key,int x, int y)); 
	 
	 // boucle infinie d'affichage
	 glutMainLoop();

	 return 0;
 } 

