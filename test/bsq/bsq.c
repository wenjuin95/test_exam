#include "bsq.h"

int check_input(t_map *map, FILE *file)
{
	char *line = NULL;
	size_t len = 0;
	int i = 0, read_byte;
	if (!file || !map)
		return 1;
	read_byte = getline(&line, &len, file);
	if (read_byte == -1) {
		free(line);
		return 1;
	}
	map->row = 0;
	while (line[i] >= '0' && line[i] <= '9')
		map->row = map->row * 10 + (line[i++] - '0');
	if (i == 0 || map->row <= 0 || !line[i] || !line[i+1] || !line[i+2]) {
		free(line);
		return 1;
	}
	map->empty = line[i++];
	map->obst = line[i++];
	map->full = line[i++];
	if (!map->empty || !map->obst || !map->full ||
		map->empty == map->obst || map->empty == map->full || map->obst == map->full) {
		free(line);
		return 1;
	}
	free(line);
	return 0;
}

int main(int ac, char **av)
{
	if (ac == 2)
	{
		t_map map = {0};
		FILE *file = fopen(av[1], "r");
		if (file == NULL)
		{
			fprintf(stderr, "Error: map failed\n");
			return (1);
		}
		if (check_input(&map, file) == 1)
		{
			fprintf(stderr, "Error: map failed\n");
			// free_map(&map);
			fclose(file);
			return (1);
		}
		else
		{
			fprintf(stdout, "Map is valid.\n");
		}
	}
}
