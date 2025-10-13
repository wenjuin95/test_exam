#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;

	int width = atoi(av[1]);
	int height = atoi(av[2]);
	int iterate = atoi(av[3]);

	if (width < 0 || height < 0 || iterate < 0)
		return 1;

	int y = 1;
	int x = 1;
	int pen = 0;
	int grid[2][height+2][width+2];
	char c;

	for(int d = 0; d < 2; d++)
		for (int y = 0; y < height + 2; y++)
			for (int x = 0; x < width + 2; x++)
				grid[d][y][x] = 0;

	while (read(0, &c, 1) > 0)
	{
		if (c == 'w' && y > 1)
			y--;
		else if (c == 's' && y < height)
			y++;
		else if (c == 'a' && x > 1)
			x--;
		else if (c == 'd' && x < width)
			x++;
		else if (c == 'x')
			pen = !pen;
		if (pen)
			grid[0][y][x] = 1;
	}

	for (int it = 0; it < iterate; it++)
	{
		int cur = it % 2;
		int next = (it + 1) % 2;

		for (int y = 0; y < height + 2; y++)
			for (int x = 0; x < width + 2; x++)
				grid[next][y][x] = 0;

		for (int y = 1; y <= height; y++)
		{
			for (int x = 1; x <= width; x++)
			{
				int n = 0;
				for (int dy = -1; dy <= 1; dy++)
				{
					for (int dx = -1; dx <= 1; dx++)
					{
						if (dy != 0 || dx != 0)
							n += grid[cur][dy+y][dx+x];
					}
				}
				if (grid[cur][y][x])
					grid[next][y][x] = (n == 2 || n == 3);
				else
					grid[next][y][x] = (n == 3);
			}
		}
	}

	int final = iterate % 2;
	for (int y = 0; y < height + 2 ; y++)
	{
		for (int x = 0; x < width + 2; x++)
		{
			if (grid[final][y][x] == 1)
				putchar('0');
			else
				putchar(' ');
		}
		putchar('\n');
	}
}
