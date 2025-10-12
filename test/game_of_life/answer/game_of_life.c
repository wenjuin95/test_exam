#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int ac, char **av) {
    if (ac != 4)
        return 1;

	//read the output
    int width = atoi(av[1]);
    int height = atoi(av[2]);
    int iteration = atoi(av[3]);

	// check output
    if (width < 0 || height < 0 || iteration < 0)
        return 0;

	//pen and grid declaration
    int x = 1;
	int y = 1;
    int pen = 0;
    int grid[2][height + 2][width + 2];
    char c;

	//initialize grid to 0
    for (int d = 0; d < 2; d++)
        for (int y = 0; y < height + 2; y++)
            for (int x = 0; x < width + 2; x++)
                grid[d][y][x] = 0;

	//read input and draw on grid
    while(read(0, &c, 1) > 0)
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
            pen = !pen; //if pen on then off, if pen off then on
        if (pen) //if pen on then assign 1
            grid[0][y][x] = 1;
    }

	//game of life iterations
    for(int it = 0; it < iteration; it++) //loop the iteration given (if 0 mean no iteration)
    {
        int cur = it % 2; //index of the current grid
        int next = (it + 1) % 2; //index of the next grid

		//clear 2d array
        for (int y = 0; y < height + 2; y++)
            for(int x = 0; x < width + 2; x++)
                grid[next][y][x] = 0;

		//apply the rules of the game of life
        for (int y = 1; y <= height; y++)
        {
            for (int x = 1; x <= width; x++)
            {
				//count alive neighbors
                int n = 0; //number of alive neighbors (out of 8)
                for (int dy = -1; dy <= 1; dy++)
                {
                    for(int dx = -1; dx <= 1; dx++)
                        if (dx != 0 || dy != 0) //not the cell itself
                            n += grid[cur][dy + y][dx + x]; //add 1 if neighbor is alive
                }
                if (grid[cur][y][x] == 1) //if the cell is alive
                    grid[next][y][x] = (n == 2 || n == 3); //stay alive if it has 2 or 3 alive neighbors
                else //if the cell is dead
                    grid[next][y][x] = (n == 3); //become alive if it has exactly 3 alive neighbors
            }
        }
    }

	//print the final grid
    int final = iteration % 2; //index of the final grid depend on the iterate number you give
    for (int y = 1; y <= height; y++)
    {
        for (int x = 1; x <= width; x++)
        {
            if (grid[final][y][x]) //if the cell is alive
                putchar('0');
            else //if the cell is dead
                putchar(' ');
        }
        putchar('\n');
    }
}
