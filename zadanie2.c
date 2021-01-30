#include <stdio.h>
#include "mpi.h"

float func(float x) {
    return x*x;
}

int main(int argc, char **argv) {
    int p;
    int n;
    int tag = 50;
    int tag2 = 60;
    int tag3 = 70;
    int i = 1;
    float h, wynik;
    float a = 2;
    float b = 3;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &p);
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Status status;
    if(p == n-1) {
        h = (b - a)/(float)n;
        wynik = (func(a) + func(b))/(float)2;
        MPI_Send(&h, 1, MPI_FLOAT, p - 1, tag, MPI_COMM_WORLD);
        MPI_Send(&wynik, 1, MPI_FLOAT, p - 1, tag2, MPI_COMM_WORLD);
        MPI_Send(&i, 1, MPI_INT, p - 1, tag3, MPI_COMM_WORLD);
    }
    if((p > 0) && (p < n-1)) {
        MPI_Recv(&h, 1, MPI_FLOAT, p + 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&wynik, 1, MPI_FLOAT, p + 1, tag2, MPI_COMM_WORLD, &status);
        MPI_Recv(&i, 1, MPI_INT, p + 1, tag3, MPI_COMM_WORLD, &status);
        wynik += func(a + i*h);
        i++;
        if(p != 0) {            
            MPI_Send(&h, 1, MPI_FLOAT, p - 1, tag, MPI_COMM_WORLD);
            MPI_Send(&wynik, 1, MPI_FLOAT, p - 1, tag2, MPI_COMM_WORLD);
            MPI_Send(&i, 1, MPI_INT, p - 1, tag3, MPI_COMM_WORLD);
        }  
    }
    if(p == 0) {
        MPI_Recv(&h, 1, MPI_FLOAT, p + 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&wynik, 1, MPI_FLOAT, p + 1, tag2, MPI_COMM_WORLD, &status);
        MPI_Recv(&i, 1, MPI_INT, p + 1, tag3, MPI_COMM_WORLD, &status);
        wynik += func(a + i*h);
        wynik = wynik * h;
        printf("Wynik calkowania x * x: %f przy uzyciu %d trapezow. Przedzial <%f, %f>\n", wynik, n, a, b);
    }
    MPI_Finalize();
    return 0;
}