#include <stdio.h>
#include <math.h>
#include "mpi.h"
int main(int argc, char **argv) {
    int p;
    double pi = 4;
    double suma = 0;
    int n;
    int tag = 50;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &p);
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Status status;
    if(p == 0) {
        MPI_Send(&suma, 1, MPI_DOUBLE, p + 1, tag, MPI_COMM_WORLD);
    }
    if((p>0)&&(p<=n-1)) {
        MPI_Recv(&suma, 1, MPI_DOUBLE, p - 1, tag, MPI_COMM_WORLD, &status);
        suma += pow(-1,p - 1)/(2*p-1);
        printf("Numer procesu: %d suma: %f\n", p, suma);
        if(p!= n-1){
            MPI_Send(&suma, 1, MPI_DOUBLE, p + 1, tag, MPI_COMM_WORLD);
        }
        if(p == n-1) {
            suma += pow(-1, p - 1)/(2*p-1);            
            pi *= suma;
            printf("Liczba pi przyblizona: %f\n", pi);
        }
    }
    MPI_Finalize();
    return 0;
}