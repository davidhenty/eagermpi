## Eager MPI

Simple program to work out the eager limit experimentally by seeing at
which message size MPI_Send becomes synchronous and the (incorrectly
written) code therefore deadlocks.

Ping-pong happens betweens ranks 0 and size-1 (not 0 and 1) as this
sometimes makes it easier to ensure that the active processes are on
different nodes.