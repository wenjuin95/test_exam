#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_bsq
{
	int line, width;
	char empty, obst, full;
	char **map;
}	t_bsq;

int min3(int a, int b, int c)
{
	return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

int ft_strlen(char *s)
{
	int i = 0;
	while(s[i])
		i++;
	return i;
}

t_bsq *read_map(char *filename)
{
	FILE *file = filename ? fopen(filename, "r") : stdin;
	if (!file) return NULL;

	t_bsq *map = malloc(sizeof(t_bsq));
	if (fscanf(file, "%d %c %c %c\n", &map->line, &map->empty, &map->obst, &map->full) != 4)
	{
		free(map);
		if(filename)
			fclose(file);
		return NULL;
	}

	map->map = malloc(sizeof(char *) * map->line);
	map->width = 0;
	for (int i = 0; i < map->line; i++)
	{
		char *line = NULL;
		size_t len = 0;
		if (getline(&line, &len, file) == -1)
		{
			free(map);
			if(filename)
				fclose(file);
			return NULL;
		}
		int line_len = ft_strlen(line);
		if (line[line_len - 1] == '\n')
			line[line_len - 1] = '\0';
		if (i == 0)
			map->width = ft_strlen(line);
		else if ((int)ft_strlen(line) != map->width)
		{
			free(map);
			if(filename)
				fclose(file);
			return NULL;
		}
		map->map[i] = line;
	}

	if (filename)
		fclose(file);

	return map;
}

int validate_map(t_bsq *map)
{
	if (!map || map->line <= 0 || map->width <= 0)
		return 0;
	if (map->empty == map->obst || map->empty == map->full || map->obst == map->full)
		return 0;
	for (int y = 0; y < map->line; y++)
		for (int x = 0; x < map->width; x++)
		{
			char c = map->map[y][x];
			if (c != map->empty && c != map->obst)
				return 0;
		}
	return 1;
}

void solve_bsq(t_bsq *map)
{
	int **dp = malloc(sizeof(int *) * map->line);
	for (int i = 0; i < map->line; i++)
		dp[i] = calloc(map->width, sizeof(int));

	int max_size = 0;
	int best_y = 0;
	int best_x = 0;

	for (int y = 0; y < map->line; y++)
	{
		for (int x = 0; x < map->width; x++)
		{
			if (map->map[y][x] == map->obst)
				dp[y][x] = 0;
			else if (y == 0 | x == 0)
				dp[y][x] = 1;
			else
				dp[y][x] = min3(dp[y-1][x], dp[y][x-1], dp[y-1][x-1]) + 1;

			if (dp[y][x] > max_size)
			{
				max_size = dp[y][x];
				best_y = y;
				best_x = x;
			}
		}
	}

	int start_y = best_y - max_size + 1;
	int start_x = best_x - max_size + 1;
	for (int i = start_y; i < start_y + max_size; i++)
		for (int j = start_x; j < start_x + max_size; j++)
			map->map[i][j] = map->full;

	for (int i = 0; i < map->line; i++)
		free(dp[i]);
	free(dp);
}

void free_map(t_bsq *map)
{
	if (!map) return;
	for (int i = 0; i < map->line; i++)
		free(map->map[i]);
	free(map->map);
	free(map);
}

void print_map(t_bsq *map)
{
	if (!map) return;
	for (int i = 0; i < map->line; i++)
		fprintf(stdout, "%s\n", map->map[i]);
}

void process_file(char *filename)
{
	t_bsq *map = read_map(filename);
	if (!map || !validate_map(map))
	{
		free_map(map);
		return;
	}
	solve_bsq(map);
	print_map(map);
	free_map(map);
}

int main(int ac, char **av)
{
	if (ac == 1)
		process_file(NULL);
	else
	{
		for (int i = 1; i < ac; i++)
			process_file(av[i]);
	}
	return 0;
}

