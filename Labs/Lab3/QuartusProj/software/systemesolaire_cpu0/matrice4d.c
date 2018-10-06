#include "matrice4d.h"


/*********************************************************
	matrice4d_init()

 *********************************************************/
void matrice4d_init( matrice4d_t *m ){

	int i,j;
	for( i=0; i<4; i++ )
		for( j=0; j<4; j++ )
			m->m[i][j] = 0;
}

/*********************************************************
	matrice4d_produit_m4d()

 *********************************************************/
void matrice4d_product_m4d( matrice4d_t *m, matrice4d_t *n, matrice4d_t *result ) {
	
	int i,j,k;
	for( i=0; i<4; i++ ) {
		for( j=0; j<4; j++ ) {
			result->m[i][j] = 0;
			for( k=0; k<4; k++ )
				result->m[i][j] +=	(m->m[i][k]*n->m[k][j]);
		}
	}
}

/*********************************************************
	matrice4d_produit_v4d()

 *********************************************************/
void matrice4d_product_v4d( matrice4d_t *m, vecteur4d_t *v, vecteur4d_t *result ) {

	int i;
	float vect[4];

	for( i=0; i<4; i++ ) {
		vect[i] = 0;
		vect[i] += m->m[i][0]*v->x;
		vect[i] += m->m[i][1]*v->y;
		vect[i] += m->m[i][2]*v->z;
		vect[i] += m->m[i][3]*v->w;
	}
	result->x = vect[0];
	result->y = vect[1];
	result->z = vect[2];
	result->w = vect[3];
}

/*********************************************************
	matrice4d_produit_v3d()

 *********************************************************/
void matrice4d_product_v3d( matrice4d_t *m, vecteur3d_t *v, vecteur4d_t *result ) {

	int i;
	float vect[4];

	for( i=0; i<4; i++ ) {
		vect[i] = 0;
		vect[i] += m->m[i][0]*v->x;
		vect[i] += m->m[i][1]*v->y;
		vect[i] += m->m[i][2]*v->z;
		vect[i] += m->m[i][3]*1;
	}
	result->x = vect[0];
	result->y = vect[1];
	result->z = vect[2];
	result->w = vect[3];
}

/*********************************************************
	matrice4d_invert()

	Inverts the 3x3 matrix, for use with matrices of 
	format show below.

	M = [ a b c 0 ]
		[ c d e 0 ]
		[ f g h 0 ]
		[ 0 0 0 1 ]

 *********************************************************/
void matrice4d_invert( matrice4d_t *m ) { 

	int i,j;
	float c[3][3];
	float adj[3][3];
	float det;

	// Cofactor Matrix
	c[0][0] = ( m->m[1][1]*m->m[2][2] - m->m[1][2]*m->m[2][1] );
	c[0][1] = ( m->m[1][0]*m->m[2][2] - m->m[1][2]*m->m[2][0] );
	c[0][2] = ( m->m[1][0]*m->m[2][1] - m->m[1][1]*m->m[2][0] );
	c[1][0] = ( m->m[0][1]*m->m[2][2] - m->m[0][2]*m->m[2][1] );
	c[1][1] = ( m->m[0][0]*m->m[2][2] - m->m[0][2]*m->m[2][0] );
	c[1][2] = ( m->m[0][0]*m->m[2][1] - m->m[0][1]*m->m[2][0] );
	c[2][0] = ( m->m[0][1]*m->m[1][2] - m->m[0][2]*m->m[1][1] );
	c[2][1] = ( m->m[0][0]*m->m[1][2] - m->m[0][2]*m->m[1][0] );
	c[2][2] = ( m->m[0][0]*m->m[1][1] - m->m[0][1]*m->m[1][0] );
	
	// Adjoint Matrix
	for( i=0; i<3; i++ ) {
		for( j=0; j<3; j++ ) {
			adj[i][j] = c[j][i];
		}
	}
	
	det =   ( m->m[0][0]*m->m[1][1]*m->m[2][2] + m->m[0][1]*m->m[1][2]*m->m[2][0] + m->m[0][2]*m->m[1][0]*m->m[2][1] )
		  - ( m->m[0][2]*m->m[1][1]*m->m[2][0] + m->m[0][1]*m->m[1][0]*m->m[2][2] + m->m[0][0]*m->m[1][2]*m->m[2][1] );

	for( i=0; i<3; i++ ) {
		for( j=0; j<3; j++ ) {
			m->m[i][j] = adj[i][j]/det;
		}
	}

}

/*********************************************************
	matrice4d_transpose()

 *********************************************************/
void matrice4d_transpose( matrice4d_t *m, matrice4d_t *result ) {

	int i,j;
	for( i=0; i<4; i++ ){
		for( j=0; j<4; j++ ) {
			result->m[i][j] = m->m[j][i];
		}
	}
}

void matrice4d_setIdentity( matrice4d_t *m ){

	matrice4d_init( m );
	m->m[0][0] = 1;
	m->m[1][1] = 1;
	m->m[2][2] = 1;
	m->m[3][3] = 1;

}


/*********************************************************
	matrice4d_setTranslationMatrix()

 *********************************************************/
void matrice4d_setTranslationMatrix( matrice4d_t *m, vecteur3d_t *v ) {

	matrice4d_init( m );
	m->m[0][0] = 1;
	m->m[1][1] = 1;
	m->m[2][2] = 1;
	m->m[3][3] = 1;
	m->m[0][3] = v->x;
	m->m[1][3] = v->y;
	m->m[2][3] = v->z;
}

/*********************************************************
	matrice4d_setRotationMatrixX()

 *********************************************************/
void matrice4d_setRotationMatrixX( matrice4d_t *m, float angle ) {

	matrice4d_init( m );
	m->m[0][0] = 1;
	m->m[1][1] = cos(angle);
	m->m[1][2] = -sin(angle);
	m->m[2][1] = sin(angle);
	m->m[2][2] = cos(angle);
	m->m[3][3] = 1;	
}


/*********************************************************
	matrice4d_setRotationMatrixY()

 *********************************************************/
void matrice4d_setRotationMatrixY( matrice4d_t *m, float angle ) {

	matrice4d_init( m );
	m->m[1][1] = 1;
	m->m[0][0] = cos(angle);
	m->m[0][2] = sin(angle);
	m->m[2][0] = -sin(angle);
	m->m[2][2] = cos(angle);
	m->m[3][3] = 1;	
}

/*********************************************************
	matrice4d_setRotationMatrixZ()

 *********************************************************/
void matrice4d_setRotationMatrixZ( matrice4d_t *m, float angle ) {

	matrice4d_init( m );
	m->m[2][2] = 1;
	m->m[0][0] = cos(angle);
	m->m[0][1] = -sin(angle);
	m->m[1][0] = sin(angle);
	m->m[1][1] = cos(angle);
	m->m[3][3] = 1;	
}

/*********************************************************
	matrice4d_setRotationMatrixN()

 *********************************************************/
void matrice4d_setRotationMatrixN( matrice4d_t *m, vecteur3d_t *normal, float angle ) {

	vecteur3d_t n;
	float c,s;

	n = *normal;
	vecteur3d_normalize(&n);
	c = cos(angle);
	s = sin(angle);

	m->m[0][0] = (n.x*n.x) + (1-n.x*n.x)*c;
    m->m[0][1] = (n.x*n.y*(1-c)) - n.z*s;
    m->m[0][2] = (n.x*n.z*(1-c)) + n.y*s;
    m->m[1][0] = (n.x*n.y*(1-c)) + n.z*s;
    m->m[1][1] = n.y*n.y + (1-n.y*n.y)*c;
    m->m[1][2] = n.y*n.z*(1-c) - n.x*s;
    m->m[2][0] = n.x*n.z*(1-c) - n.y*s;
    m->m[2][1] = n.y*n.z*(1-c) + n.x*s;
    m->m[2][2] = n.z*n.z + (1-n.z*n.z)*c;

	m->m[0][3] = 0;
	m->m[1][3] = 0;
	m->m[2][3] = 0;
	m->m[3][0] = 0;
	m->m[3][1] = 0;
	m->m[3][2] = 0;	
    m->m[3][3] = 1;

}

/*********************************************************
	matrice4d_setRotationMatrixXYZ()

 *********************************************************/
void matrice4d_setRotationMatrixXYZ( matrice4d_t *m, vecteur3d_t *X, vecteur3d_t *Y, vecteur3d_t *Z ) {

	m->m[0][0] = X->x;
    m->m[0][1] = X->y;
    m->m[0][2] = X->z;
    m->m[1][0] = Y->x;
    m->m[1][1] = Y->y;
    m->m[1][2] = Y->z;
    m->m[2][0] = Z->x;
    m->m[2][1] = Z->y;
    m->m[2][2] = Z->z;
    m->m[3][3] = 1;

	m->m[0][3] = 0;
	m->m[1][3] = 0;
	m->m[2][3] = 0;
	m->m[3][0] = 0;
	m->m[3][1] = 0;
	m->m[3][2] = 0;	   
}


/*********************************************************
	matrice4d_setProjectionMatrix()

 *********************************************************/
void matrice4d_setProjectionMatrix( matrice4d_t *m, float near_dist, float far_dist, float zoomx, float zoomy ) {

	matrice4d_init( m );

    m->m[0][0] = zoomx;     
    m->m[1][1] = zoomy;     
    m->m[2][2] = (far_dist+near_dist)/(far_dist-near_dist);
    m->m[2][3] = (2*near_dist*far_dist)/(near_dist-far_dist);
    m->m[3][2] = 1;
}

/*********************************************************
	matrice4d_print()

 *********************************************************/
void matrice4d_print( matrice4d_t *m ) {

	int i,j;

	for( i=0; i<4; i++ ) {
        printf("[ ");
        for( j=0; j<4; j++ ){     
            if( m->m[i][j] < 0 )
                printf("%2.2f ", m->m[i][j]);                
            else
                printf(" %2.2f ", m->m[i][j]);                
        }
        printf("] \n");
    }
    printf("\n");

}
