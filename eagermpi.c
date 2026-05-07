#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

/*
 *  Simple program to work out the eager limit experimentally by
 *  seeing at which message size MPI_Send becomes synchronous and the
 *  (incorrectly written) code therefore deadlocks.
 */

int main(void)
{
  int kilo=1024;
  int mega=kilo*kilo;

  int max=16*mega;

  int msg, i;
  int rank, size, dest;
  char *buffer;
  MPI_Status status;

  buffer = (char *) malloc(max);

  MPI_Init(NULL, NULL);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (size < 2)
    {
      if (rank == 0) printf("ERROR: must run on at least 2 processes\n");
      MPI_Finalize();
      return 1;
    }

  msg = 1;
  dest = size-rank-1;
        
  while (msg <= max)
    {
      if (rank == 0 || rank == size-1)
        {
          printf("Starting MPI_Send of %d bytes on rank %d ...\n", msg, rank);
          MPI_Send(buffer, msg, MPI_BYTE, dest, 0, MPI_COMM_WORLD);
          printf("Finished MPI_Send of %d bytes on rank %d\n", msg, rank);

          printf("Starting MPI_Recv of %d bytes on rank %d ...\n", msg, rank);
          MPI_Recv(buffer, msg, MPI_BYTE, dest, 0, MPI_COMM_WORLD, &status);
          printf("Finished MPI_Recv of %d bytes on rank %d\n", msg, rank);
        }

      MPI_Barrier(MPI_COMM_WORLD);
      msg = msg*2;
    }

  MPI_Finalize();
  return 0;
}
