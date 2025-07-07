#include"unistd.h"
#include"stdlib.h"
#include"stdio.h"
#include"fcntl.h"


typedef struct s_map{
	int row;
	int col;
	char empty; // "."
	char obst;  // "o"
	char full;  // "x"
	char **grid; // 2d array of char
}  t_map;


//check the first line of the file, if it has 4 info(number, empty, obst, full)
int init_map(t_map *map, FILE *file){
	if (!file || !map)
		return (0);
	size_t len = 0;
	ssize_t read;
	char *line = NULL;

	read = getline(&line, &len, file);
	if (read == -1){
		free(line);
		return (0);
	}

	// convert the first line of the first word to an integer
	int i = 0;
	while (line[i] >= '0' && line[i] <= '9'){
		map->row = map->row * 10 + (line[i] - '0');
		i++;
	}

	//if the number is 0 or the first character is not a digit
	if (i == 0 || map->row <= 0){
		free(line);
		return (0);
	}

	//if the first line doesn't have 4 info(number, empty, obst, full)
	if (read < i + 3){
		free(line);
		return (0);
	}

	//if the first line has more than 4 info
	map->empty = line[i++]; // Read the empty character
	map->obst = line[i++]; // Read the obstacle character
	map->full = line[i++]; // Read the full character

	//if is empty, obst was empty, full was empty, empty was obst, empty was full, obst was full
	if (!map->empty || !map->obst || !map->full || map->empty == map->obst || map->empty == map->full || map->obst == map->full){
		free(line);
		return (0); // mean failed
	}

	//if the first line has more than 4 info
	free (line);
	return (1); // mean success
}

//check if the grid is valid, if the grid has the same length
int read_grid(t_map *map, FILE *file){
	if (!map || !file)
		return (0);
	size_t len = 0;
	ssize_t read;
	char *line = NULL;
	int row = 0;

	//set the row size
	map->grid = calloc(map->row, sizeof(char *));
	if (!map->grid)
		return (0);

	//read the grid line by line
	while ((read = getline(&line, &len, file)) != -1 && row < map->row){

		//if the line is empty mean the rest was empty
		if (row == 0) // if first line is empty
			map->col = read - 1;
		else if (read - 1 != map->col){ //check if the last line has the same length
			free (line);
			return (0);
		}

		//if is not set the colum size
		map->grid[row] = malloc(sizeof(char) * map->col + 1);
		if (!map->grid[row]){
			free (line);
			return (0);
		}

		//set the line to the grid
		for (int col = 0; col < map->col; col++){
			 //if the line doesn't have "obst" or "empty"
			if (line[col] != map->empty && line[col] != map->obst){
				free(line);
				return (0);
			}

			//assign the line to the grid
			map->grid[row][col] = line[col];
		}
		map->grid[row][map->col] = '\0';
		row++;
	}

	free(line);
	return (row == map->row); // if row equal to map->row mean success else mean failed
}

// free 2d array
void    free_map(t_map *map){
	if (map->grid){
		for (int i = 0; i < map->row; i++)
			free(map->grid[i]);
		free (map->grid);
	}
	return ;
}

//compare 3 integers and return the minimum
int min3(int a, int b, int c){
	int result = a;
	if (b < result)
		result = b;
	if (c < result)
		result = c;
	return (result);
}

//find the biggest square in the grid
void    find_biggest_square(t_map *map){
	int max_i = 0;
	int max_j = 0;
	int max_size = 0;

	//memory allocation for 2d array
	int **dp = calloc(map->row, sizeof(int *));
	for (int i = 0; i < map->row; i++){
		dp[i] = calloc(map->col, sizeof(int));
	}

	//check row
	for (int i = 0; i < map->row; i++){
		dp[i][0] = map->grid[i][0] == map->empty ? 1 : 0; //if each first line in row is "empty", set to 1 else set to 0
		if (dp[i][0] > max_size){ //if the first line in row is "empty" is bigger then max_size is 0
			max_size = dp[i][0]; //overwrite max_size to "empty" that is 1
			max_i = i; //count each i to max_i
			max_j = 0; //set max_j to 0 because not use
		}
	}

	//check column
	for (int j = 0; j < map->col; j++){
		dp[0][j] = map->grid[0][j] == map->empty ? 1 : 0; //if each first line in column is "empty", set to 1 else set to 0
		if (dp[0][j] > max_size){ //if the first line in column is "empty" is bigger then max_size is 0
			max_size = dp[0][j]; //overwrite max_size to "empty" that is 1
			max_i = 0; //set max_i to 0 because not use
			max_j = j; //count each j to max_j
		}
	}

	// check the rest of the grid
	for (int i = 1; i < map->row; i++){
		for (int j = 1; j < map->col; j++){
			if (map->grid[i][j] == map->empty){ //if each grid is "empty"
				dp[i][j] = 1 + min3(dp[i -1][j], dp[i][j-1], dp[i-1][j-1]); // set the value to 1 + minimum of the three neighbors (top, left, top-left)
				if (dp[i][j] > max_size){ //if the value is bigger than max_size
					max_size = dp[i][j]; // overwrite max_size to the value
					max_i = i; // count each i to max_i
					max_j = j; // count each j to max_j
				}
			}
		}
	}

	//fill the "full" square in the grid
	for (int i = max_i - max_size + 1; i <= max_i && i < map->row; i++){
		for (int j = max_j - max_size + 1; j <= max_j && j < map->col; j++){
			map->grid[i][j] = map->full;
		}
	}

	//free the memory of row and column
	for (int i = 0; i < map->row; i++)
		free(dp[i]);
	free (dp);
}

// print the grid to stdout
void    print_grid(t_map *map){
	if (!map || !map->grid)
        return;

	for (int i = 0; i < map->row; i++){
		fprintf(stdout, "%s\n", map->grid[i]);
	}
}

int main(int ac, char **av){
	if (ac == 1){
		t_map map = {0};

		if (!init_map(&map, stdin) || !read_grid(&map, stdin)){
			fprintf(stderr, "Error: map failed");
			free_map(&map);
			return (1);
		}
		find_biggest_square(&map);
		print_grid(&map);
		free_map(&map);
		return (0);
	}
	else {
		int i = 1;
		while (i < ac){
			t_map map = {0}; // initialize to zero
			FILE *file = fopen(av[i], "r");
			if (!file){
				fprintf(stderr, "Error: map failed\n");
				i++;
				continue;
			}
			if (!init_map(&map, file) || !read_grid(&map, file)){
				fprintf(stderr, "Error: map failed");
				free_map(&map);
				fclose(file);
			}
			else{
				find_biggest_square(&map);
				print_grid(&map);
				free_map(&map);
				fclose(file);
			}
			fprintf(stdout, "\n");
			i++;
		}
		return (0);
	}
	return (1);
}
