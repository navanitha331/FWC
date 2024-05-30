#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matfun.h"
#include "geofun.h"

int main() {
           int m=2,n=1,p=1;	
	    double **A,**B,**C,side_AB, side_BC, side_CA;
	    double angleA;
            double **mat = createMat(2, 3);
    
            
	    A = createMat(m,n);
            B = createMat(m,n);
            C = createMat(m,n);
            
            A[0][0] =  1;
            A[1][0] = -1;
            B[0][0] =  -4;
            B[1][0] =  6;
            C[0][0] = -3;
            C[1][0] =  -5;
            double **side_ab, **side_bc, **side_ca;
            double **angle_ba,**transpose_ba;
            side_ab = Matsub(A,B,m,n);//A-B
            side_bc = Matsub(B,C,m,n);//B-C
            side_ca = Matsub(C,A,m,n);//C-A
            angle_ba = Matsub(B,A,m,n);//B-A
            
            side_AB = Matnorm(side_ab,m);
            side_BC = Matnorm(side_bc,m); 
            side_CA = Matnorm(side_ca,m);
            transpose_ba = transposeMat(angle_ba,m,n);
    	    angleA = calculateAngleA(1, -1, 0, 4, -5, 8 );
    	    
            printf("AB= %lf\n", side_AB);
    	    printf("BC= %lf\n", side_BC);
            printf("CA= %lf\n", side_CA);
            printf("angleA= %lf\n", angleA);
           
            freeMat(A,2);
    	    freeMat(B,2);
    	    freeMat(C,2);
    	    freeMat(side_ab,2);
    	    freeMat(side_bc,2);
    	    freeMat(side_ca,2);
    	    freeMat(angle_ba,2);
    	    
    	    
    	     return 0;
}
