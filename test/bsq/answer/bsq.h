#ifndef BSQ_H
#define BSQ_H

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

typedef struct s_map {
	int row;
	int col;
	char empty; // "."
	char obst; // "o"
	char full; // "x"
	char **grid;
} t_map;

#endif
