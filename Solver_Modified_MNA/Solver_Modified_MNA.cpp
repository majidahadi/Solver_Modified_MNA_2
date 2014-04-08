// Solver_MNA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "conio.h"
#include <regex>
#include <complex>
#include "MATRIX.h"

#define M_PI       3.14159265358979323846

using namespace std;

struct node {
	char type;			//stores element type
	int Na, Nb;			// Nodes between which element is connected
	complex <float> value;			// Value of element
};


int main()
{
	int i=0;
	complex <float> factor(1,0);
	node vertex[50];
	ifstream in("simple_circuit.txt");
	string line;

	complex <float> s(0,2*M_PI);
	complex <float> minus_s(0,-2*M_PI);
	complex <float> one(1,0);
	complex <float> zero(0,0);
	cout << "S=" << s << endl;
	

	if(!in) {
		cerr<<"circuit.txt could not be opened for reading\n"<<endl;
		exit(1);
	}
	
	while(getline(in, line)) {
		cout << line << endl;

		tr1::cmatch res;
		//tr1::regex rx("([rlgc]d)s+(d)s+(d)s+(d)");
		tr1::regex rx("([rLgcIV][0-9]+) +([0-9]+) +([0-9])+ +([0-9]+.?[0-9]*)");
		if(regex_search(line.c_str(), res, rx)) {
		//	cout << "Match" <<endl;
		}
		cout << "Values: " << res[1] <<" "<< res[2] << " " << res[3] << " " << res[4] << endl; 

		vertex[i].type = line[0];
		//cout << "Type of element: " << vertex[i].type << endl;
		vertex[i].Na = stoi(res[2]);
		//cout << "Node A: " << vertex[i].Na << endl;
		vertex[i].Nb = stoi(res[3]);
		//cout<< "Node B: " << vertex[i].Nb << endl;
		vertex[i].value = stof(res[4]);
		//cout << "Value: "<< vertex[i].value << endl;
		i++;
	}


	int length=i;
	int size=0;
	int LV=0;
	for (int p=1; p<length; p++){
		size=max(size,vertex[p].Na);
		size=max(size,vertex[p].Nb);
		if (vertex[p].type=='L'|| vertex[p].type=='V')
			LV++;
	}
	size+=LV;
	//size++;
	cout <<"No. of nodes=" << size <<endl;

	complex <float>* B = new complex <float>[size];
	complex <float>* Z = new complex <float>[size];
	complex <float>* X = new complex <float>[size];
	complex <float>** A = new complex <float>*[size];
	for(int p = 0; p < size; p++)
		A[p] = new complex <float>[size];
	complex <float>** L = new complex <float>*[size];
	for(int p = 0; p < size; p++)
		L[p] = new complex <float>[size];
	complex <float>** U = new complex <float>*[size];
	for(int p = 0; p < size; p++)
		U[p] = new complex <float>[size];

	for (int q=0; q<size; q++)
			B[q]=0;
	for (int p=0; p<size;p++)
		for (int q=0; q<size; q++){
			A[p][q]=0;
			L[p][q]=0;
			U[p][q]= (p != q ? 0 : 1);
		}

	for (int p=0; p<size;p++){
		for (int q=0; q<size; q++)
			cout << A[p][q] << ' ';
		cout << endl;
	}


	for (int p=0;p<length;p++){
		if (vertex[p].type=='g' || vertex[p].type=='c') {
			if (vertex[p].type=='g') {
				cout << "It is a conductance" << endl;
				factor=one;
			} else {
				cout << "It is a capacitance" << endl;
				factor=s;
			}
				
				cout << "Factor= "<< factor << endl;
				vertex[p].value = vertex[p].value*factor;
				cout<< "AFter using s, value = " << vertex[p].value << endl;

			if (vertex[p].Nb==0) {
				cout << "Nb is 0" << endl;
				//cout << "Index" << vertex[p].Na << endl; 
				A[(vertex[p].Na)-1][(vertex[p].Na)-1]=A[(vertex[p].Na)-1][(vertex[p].Na)-1]+vertex[p].value;
				cout << "Vertex value:" << A[vertex[p].Na-1][vertex[p].Na-1] << endl;
			}
			else if (vertex[p].Na==0)
				A[vertex[p].Nb-1][vertex[p].Nb-1]=A[vertex[p].Nb-1][vertex[p].Nb-1]+vertex[p].value;
			else {
				A[vertex[p].Na-1][vertex[p].Na-1]=A[vertex[p].Na-1][vertex[p].Na-1]+vertex[p].value;
				A[vertex[p].Na-1][vertex[p].Nb-1]=A[vertex[p].Na-1][vertex[p].Nb-1]-vertex[p].value;
				A[vertex[p].Nb-1][vertex[p].Na-1]=A[vertex[p].Nb-1][vertex[p].Na-1]-vertex[p].value;
				A[vertex[p].Nb-1][vertex[p].Nb-1]=A[vertex[p].Nb-1][vertex[p].Nb-1]+vertex[p].value;
			
			}

		} 
		else if (vertex[p].type=='L' || vertex[p].type=='V') {
			if (vertex[p].type=='L') {
				cout << "It is an nductance" << endl;
				factor=minus_s;
			} else {
				cout << "It is a voltage source" << endl;
				factor=zero;
			}
				
			cout << "Factor= "<< factor << endl;
			cout<< "AFter using s, value = " << vertex[p].value << endl;

			if (vertex[p].Na-1 >= 0){
				A[vertex[p].Na-1][size-LV]+=1;
				A[size-LV][vertex[p].Na-1]+=1;}
			if (vertex[p].Nb-1 >= 0){
				A[vertex[p].Nb-1][size-LV]+=-1;
				A[size-LV][vertex[p].Nb-1]+=-1;}
			A[size-LV][size-LV]+=factor*vertex[p].value;			

			LV--;
		}
		if (vertex[p].type=='I' || vertex[p].type=='V') {
			if (vertex[p].Na-1 >= 0)
				B[vertex[p].Na-1 ]-=vertex[p].value;
			if (vertex[p].Nb-1 >= 0)
				B[vertex[p].Nb-1 ]+=vertex[p].value;		
		}


	} 

	for(int p=0; p<size;p++){
		for (int q=0; q<size; q++)
			cout << (A[p][q]) << " ";
		cout << endl;
	}
	cout << "B Matrix" << "\n";
	for (int q=0; q<size; q++)
			cout << (B[q]) << "\n";
	/////////////////////////////////LU Factorization///////////////////////////////
	
	for (int k=0; k<size ; k++){
		for (int p=0; p<=k; p++){
			L[k][p]=A[k][p];
			for (int m=0; m<p; m++)
				L[k][p]-=L[k][m]*U[m][p];
		}
		for (int q=k+1; q<size; q++){
			U[k][q]=A[k][q];
			for (int n=0;n<k;n++)
				U[k][q]-=L[k][n]*U[n][q];
			U[k][q]/=L[k][k];
		}
	}

	cout << "L Matrix" << "\n";
	for(int p=0; p<size;p++){
		for (int q=0; q<size; q++)
			cout << (L[p][q]) << " ";
		cout << endl;
	}
	cout << "U Matrix" << "\n";
	for(int p=0; p<size;p++){
		for (int q=0; q<size; q++)
			cout << (U[p][q]) << " ";
		cout << endl;
	}

	////////////////////////////////////solving for Z matrix and X matrix////////////////////////////
	Z[0]=B[0]/L[0][0];
	complex <float> sum;
	for (int m=1; m<size; m++){
		sum=0;
		for (int n=0; n<m; n++)
			sum += L[m][n]*Z[n];
		Z[m]=(B[m]-sum)/L[m][m];
	}

	X[size-1]=Z[size-1];
	for (int m=size-2; m>=0; m--){
		sum=0;
		for (int n=m+1; n<=size-1; n++)
			sum += U[m][n]*Z[n];
		X[m]=Z[m]-sum;
	}

	cout << "X Matrix" << "\n";
	for (int q=0; q<size; q++)
			cout << (X[q]) << "\n";


	getch();
	return 0;
}

