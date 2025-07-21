#include "bsq.h"

//check for the input
int check_input(t_map *map, FILE *file)
{
	char *line = NULL;
	size_t len = 0;
	int read_byte;
	int i = 0;
	if (!file || !map)
		return 1;
	read_byte = getline(&line, &len, file);
	if (read_byte == -1) {
		free(line);
		return 1;
	}
	map->row = 0;
	while (line[i] >= '0' && line[i] <= '9') //check the first line first word is a number
	{
		map->row = map->row * 10 + (line[i] - '0');
		i++;
	}
	if (i == 0 || map->row <= 0 || !line[i] || !line[i+1] || !line[i+2]) //check first line is empty
	{
		free(line);
		return 1;
	}
	map->empty = line[i++]; //move these position if is not empty
	map->obst = line[i++];
	map->full = line[i++];
	//check for empty character and the position is not the same
	if (!map->empty || !map->obst || !map->full || map->empty == map->obst || map->empty == map->full || map->obst == map->full || map->full == '\n')
	{
		free(line);
		return 1;
	}
	free(line);
	return 0;
}

//check for the grid
int check_grid(t_map *map, FILE *file)
{
	if (!map || !file)
		return 1;
	size_t len = 0;
	int read_byte;
	char *line = NULL;
	int row = 0;

	map->grid = calloc(map->row, sizeof(char *));
	if (!map->grid)
		return 1;
	while ((read_byte = getline(&line, &len, file)) != -1 && row < map->row)
	{
		if (row == 0) //check for the first line
			map->col = read_byte - 1;
		else if (read_byte -1 != map->col) //if last line is not the same length
		{
			free(line);
			return 1;
		}
		map->grid[row] = calloc(map->col + 1, sizeof(char));
		if (!map->grid[row])
		{
			free(line);
			return 1;
		}
		int col = 0;
		while (col < map->col)
		{
			if (line[col] != map->empty && line[col] != map->obst) //check if the line has no "empty" and "obst"
			{
				free(line);
				return 1;
			}
			map->grid[row][col] = line[col];
			col++;
		}
		map->grid[row][map->col] = '\0';
		row++;
	}
	free(line);
	if (row == map->row) //if the row is equal to the map->row
		return 0;
	else
		return 1;
}

void free_map(t_map *map)
{
	if (map->grid)
	{
		for (int i = 0; i < map->row; i++)
			free(map->grid[i]);
		free(map->grid);
	}
	return ;
}

int find_smallest(int a, int b, int c)
{
	int min = a;
	if (b < min) min = b;
	if (c < min) min = c;
	return min;
}

void find_big_square(t_map *map)
{
	int max_row = 0;
	int max_col = 0;
	int max_size = 0;

	// Allocate and initialize grid
	int **grid = calloc(map->row, sizeof(int*));
	for (int i = 0; i < map->row; i++)
		grid[i] = calloc(map->col, sizeof(int));

	// Fill the grid with 0(obst) and 1(can put square) based on the map
	for (int i = 0; i < map->row; i++)
	{
		for (int j = 0; j < map->col; j++)
		{
			if (map->grid[i][j] == map->empty)
			{
				if (i == 0 || j == 0) //fill the first row and col to 1
					grid[i][j] = 1;
				else //calculate the grid square size
					grid[i][j] = find_smallest(grid[i-1][j], grid[i][j-1], grid[i-1][j-1]) + 1;
				if (grid[i][j] > max_size) // find the biggest square
				{
					max_size = grid[i][j];
					max_row = i;
					max_col = j;
				}
			}
		}
	}

	//get the biggest grid square and fill it with "full"
	for (int i = max_row - max_size + 1; i <= max_row; i++) {
		for (int j = max_col - max_size + 1; j <= max_col; j++) {
			map->grid[i][j] = map->full;
		}
	}

	// Free grid
	for (int i = 0; i < map->row; i++)
		free(grid[i]);
	free(grid);
}

void print_answer(t_map *map)
{
	if (!map || !map->grid)
		return ;
	for (int i = 0; i < map->row; i++)
		fprintf(stdout,"%s\n", map->grid[i]);
}

int main(int ac, char **av)
{
	if (ac > 1)
	{
		for (int i = 1; i < ac; i++)
		{
			t_map map = {0};
			FILE *file = fopen(av[i], "r");
			if (!file || check_input(&map, file) == 1 || check_grid(&map, file) == 1)
			{
				free_map(&map);
				fclose(file);
				fprintf(stderr, "map error\n");
			}
			else
			{
				find_big_square(&map);
				print_answer(&map);
				free_map(&map);
				fclose(file);
			}
		}
	}
	else
	{
		t_map map = {0};
		if (check_input(&map, stdin) == 1 || check_grid(&map, stdin) == 1)
			fprintf(stderr, "map error\n");
		else
		{
			find_big_square(&map);
			print_answer(&map);
			free_map(&map);
		}
	}
	return (0);
}
