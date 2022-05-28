Name: Jacob Beal 

To run:
Start by running command make on the command line, this will generate two executables maze and mazep.

The executable maze takes 2 integer parameters -n (size of grid) -s (seed), enter on command line in the format ./maze -n 11 -s 4
Without specifying the parameters it will create a grid of size 11 with a seed of 0, without specifying parameters enter on command line in the format ./maze
maze will always use only 1 thread without the need to specify.

The executable mazep takes 2 integer parameters -n (size of grid) -s (seed), enter on command line in the format ./mazep -n 11 -s 4
Without specifying the parameters it will create a grid of size 11 with a seed of 0, without specifying parameters enter on command line in the format ./mazep
mazep will always use 4 threads without the need to specify.

Note:
    When specifying a grid size ensure that size is odd and greater than 2 otherwise the program will exit telling you to specify either a size greater than 2 or an odd number
    Typically the smaller the gridsize for mazep rank 0 will consume most,
    if not all the grid im not sure if this is a problem but im noting that it does happen.  