#include <stdio.h>
#include <omp.h>

double f(double x){
    return (4.0/(1.0+ x*x));
}

int main(){
   
    int n = 50000;
    double X_0 = 0, X_1 = 1;
    double dx = (X_1 - X_0)/n;
    double A = 0;

    for (int i = 0; i < n; i++)
    {
        A = A + (f(X_0 + i*dx)*dx);
    }
    printf("Resultado: (%d)", A);
    
    /*#pragma omp parallel num_threads(128)
    {   
        int ID = omp_get_thread_num();
        printf("Hello (%d)\n", ID); 
    }*/


   

    return 0;


}