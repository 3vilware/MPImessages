/*Compillar con: mpicxx messagesMPI.cpp -o mensajes
  Correr con: mpirun -np 8 ./mensajes
  (8 es el número de procesos.) */

#include <iostream>
#include <mpi.h>
#define ITERATIONS 1000
using namespace std;

int main(int argc, char *argv[]) {
  int rank;
  int procs;
  int iteraciones; //define la cantidad de iteraciones por core.
  int inicio, fin;
  int cantidad_loops;

  MPI::Init(argc, argv);

  rank = MPI::COMM_WORLD.Get_rank();
  procs = MPI::COMM_WORLD.Get_size();

  iteraciones = ITERATIONS/procs; //se divide el trabajo entre el numero de procesos.
  inicio = rank * iteraciones;
  fin = inicio + iteraciones;
  cantidad_loops = 0;

  for (int i = 0; i < fin ; i++) {
    cantidad_loops++;
  }

  cout << "El proceso " << rank << " realizara "<< cantidad_loops
       << " loops durante la ejecucion" << endl;

  if(rank != 0) // Si NO estamos en el proceso maestro
  {
    MPI::COMM_WORLD.Send(&cantidad_loops,1,MPI::INT,0,0);
    // enviamos dato, cantidad de datos, tipo de dato de MPI, proceso destino, etiqueta.
  }
  else // Si estamos en el proceso maestro->
  {
     int total_loops = cantidad_loops;

     for (int j = 1; j < procs; j++)
      {
          MPI::COMM_WORLD.Recv(&cantidad_loops, 1, MPI::INT, j, 0);
          // recibimos dato, cantidad de datos, tipo de dato de MPI, proceso fuente, etiqueta.
          total_loops += cantidad_loops;
      }
      //Por si quedan operaciones por realizar
      cantidad_loops = 0;
      for (int k = total_loops; k < ITERATIONS; k++) {
        cantidad_loops++;
      }

      cout << "\nEl proceso 0 realizo los " << cantidad_loops << " loops restantes" << endl;
  }

  MPI::Finalize();

  return 0;
}
