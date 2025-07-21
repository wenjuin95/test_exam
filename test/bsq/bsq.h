#ifndef BSQ_H
#define BSQ_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct s_map {
	int row;
	int col;
	int empty;
	int obst;
	int full;
	int **grid;
} t_map;

#endif
