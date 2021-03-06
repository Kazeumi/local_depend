#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double floor(double x);

//Fc declaration

	//Memory allocation
void message_error(char error_text[]);
double *d_vector(long size); //for generating double vector[1]...vector[size]
void free_d_vector(double *x);

	//Solving ODE
void differential (double time, double in[], double out[]);
double dx_dt(double time, double vr[]);
double dy_dt(double time, double vr[]);
void rk4(double N_in[], double k1[], int n, double t, double h, double N_out[], void (*diff)(double, double [], double []) );


// Global variables
int j_x = 1, j_y = 2, j_p = 3; //index for x, y, p
double a1=0.5, a2=0.3, b1=0.7, b2=1.1; 		  // model parameter
double r1=0.7, r2=0.7, K1=1.2, K2=1.2, m=0.5; // model parameter
double t=0.0; //time
double deltat=0.01; //time step for rk4
double *N; // initialize array for population size
double *dfdt;


// Main function
int main(void){
	
	dfdt = d_vector(3); //set vector for 'differential' output
	N = d_vector(3);
	N[j_x] = 1.0; //set initial population size
	N[j_y] = 1.0; //set initial population size
	N[j_p] = 1.0; //set initial population size

	int i; //for loop index
	double T=300.2; // T: time of simulation

	for (i=0; i < T/deltat; i++){
		
		if (i % (int)floor(0.1/deltat) == 0 || i == 0){
			printf("%lf\t%lf\t%lf\t%lf\n", t, N[j_x], N[j_y], N[j_p]);
		}
		differential(t, N, dfdt);
		rk4(N, dfdt, 3, t, deltat, N, differential); //first N is input, second N is output
		t += deltat;
	}; //end for: end solving DE


	free_d_vector(N);
	free_d_vector(dfdt);
	return 0;
};



/* ODE Solver Functions */

double dx_dt(double time, double vr[]){
	return (r1*(1.0 - vr[j_x]/K1) - a1*vr[j_p])*vr[j_x];
};

double dy_dt(double time, double vr[]){
	return (r2*(1.0 - vr[j_y]/K2) - a2*vr[j_p])*vr[j_y];
};

double dp_dt(double time, double vr[]){
	return (b1*vr[j_x] + b2*vr[j_y] - m)*vr[j_p]; //DE for predator
};

void differential (double time, double in[], double out[]){
	out[j_x] = dx_dt(time, in);
	out[j_y] = dy_dt(time, in);
	out[j_p] = dp_dt(time, in);
};



/* 
void (*diff)(double, double [], double []) is a function pointer.
In order to use functions as inputs, it is included in the parameter of the function 'rk4'.
In this case, the function 'differential' is taken as input to the function 'rk4'.
*/

void rk4(double N_in[], double k1[], int n, double t, double h, double N_out[], void (*diff)(double, double [], double []) )
{
	int i;
	double *k2, *k3, *k4, *D;
	// n: dimension of the ode
	// N_in: input population size ni. For 2-dim case n1=(pop. size of x); n2=(pop. size of y)
	// k1: parameter of rk4;  k1 = f(D,t); i.e. the output from differential is input of rk4
	k2 = d_vector(n); // rk4 k2
	k3 = d_vector(n); // rk4 k3
	k4 = d_vector(n); // rk4 k4
	D = d_vector(n); // Dummy variable for calculating coefficients of k_i in rk4 

	for (i=1;i<=n;i++) D[i] = N_in[i] + (h/2.0)*k1[i];
	(*diff)(t+h/2.0, D, k2);
	for (i=1;i<=n;i++) D[i] = N_in[i] + (h/2.0)*k2[i];
	(*diff)(t+h/2.0, D, k3);
	for (i=1;i<=n;i++) D[i] = N_in[i] + h*k3[i];
	(*diff)(t+h, D, k4);

	for (i=1;i<=n;i++) N_out[i] = N_in[i] + (h/6.0)*(k1[i] + 2.0*k2[i] + 2.0*k3[i] + k4[i]);

	free_d_vector(D);
	free_d_vector(k2);
	free_d_vector(k3);
	free_d_vector(k4);

};


/* Memory Allocation Functions */

void message_error(char error_text[])
/*Standard error handler*/
{
    printf("There are some errors...\n");
    printf("%s\n", error_text);
    printf("...now existing to system...\n");
    exit(1);
};

double *d_vector(long size){ //for generating double vector[1]...vector[size]
    double *x; //pointer
    
    x = malloc((size + 1)*sizeof(double));
    //allocation of memory space to stock (size + 1) double variables & malloc returns the address of the top of the allocated memory space to x
    //the reason to prepare (size + 1) is just to use from x[1] to x[size]
    
    if(x == NULL) message_error("allocation failure in d_vector()"); //if memory allocation was failed, malloc returns NULL and x becomes NULL
    return x; //return the address of the top of the allocated memory space to x
};

void free_d_vector(double *x)
{
    free(x);
};
