// LU_Factorization.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "MATRIX.h"


int LU_fact(int n, int *A, int *L, int *U)
{

	/*int k,p,q,i,j;
	
	for (k=0; k<n; k++){
		for (p=0; p<=k; p++){
			L[k][p]=A[k][p];
			for (i=0; i<p; i++)
				L[k][p]-=L[k][i]*U[i][p];
		}
		for (q=k+1; q<n; q++){
			U[k][q]=A[k][q];
			for (j=0;j<k;j++)
				U[k][q]-=L[k][j]*U[j][q];
			U[k][q]/=L[k][k];
		}
	}


	printf("Matrix A:\n");
	for (p=0; p<4;p++){
		for (q=0; q<4;q++)
			printf("%f\t",A[p][q]);
		printf("\n");}
	printf("Matrix L:\n");
	for (p=0; p<4;p++){
		for (q=0; q<4;q++)
			printf("%f\t",L[p][q]);
		printf("\n");}
	printf("Matrix U:\n");
	for (p=0; p<4;p++){
		for (q=0; q<4;q++)
			printf("%f\t",U[p][q]);
		printf("\n");}

*/
	return 0;

}

