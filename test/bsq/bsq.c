#include "bsq.h"

int main(int ac, char **av)
{
	if (ac == 2)
	{
		t_map *map = {0};
		FILE *file = fopen(av[1], "r");
		if (file == NULL)
		{
			fprintf(stderr, "map error\n")
		}
	}
	return 0;
}
