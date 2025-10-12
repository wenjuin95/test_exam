#include <stdio.h>
#include <stdlib.h>

typedef struct s_bsq {
    int each_line;
    int line_len;
    char empty;
    char obst;
    char full;
    char **map;
    int sq_dimension;
    int row;
    int col;
} t_bsq;

int ft_strlen(char *s)
{
    int i = 0 ;
    while (s[i])
        i++;
    return i;
}

int is_digit(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    return 0;
}

void pirnt_arr(char **arr)
{
    for (int i = 0; arr && arr[i]; i++)
        fprintf(stdout, "%s\n", arr[i]);
}

void free_arr(char **arr)
{
    int i = 0;
    if (!arr)
        return ;
    while (arr && arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

char **read_file(char *filename, t_bsq *data)
{
    FILE *filestream = filename ? fopen(filename, "r") : stdin;
    if(!filestream)
    {
        fprintf(stderr, "Error: open file\n");
        return NULL;
    }

    char **map = NULL;
    char *line = NULL;
    size_t len = 0;

    int read = getline(&line, &len, filestream);
    if (read == -1)
    {
        if (filestream)
            fclose(filestream);
        free(line);
        fprintf(stderr, "Error: error getline\n");
        return NULL;
    }
    //if last line is newline the make it null and decrease the position
    if (line[read-1] == '\n')
    {
        line[read-1] = '\0';
        read--;
    }

    //check firstchar in the line is digit
    int number = 0;
    int pos = 0;
    while (is_digit((unsigned char)line[pos]))
    {
        number = number * 10 + (line[pos] - '0');
        pos++;
    }
    if (pos == 0 || number <= 0)
    {
        fprintf(stderr, "Error: first char not a number\n");
        free(line);
        if(filestream)
            fclose(filestream);
        return NULL;
    }
    //check if there are exactly 3 char after the number
    if (read - pos != 3)
    {
        fprintf(stderr, "Error: no [empty], [obstacle], [full] found\n");
        free(line);
        if(filestream)
            fclose(filestream);
        return NULL;
    }
    //check if 3 missing one
	if (line[pos] == line[pos + 1] || line[pos] == line[pos + 2] || line[pos + 1] == line[pos + 2])
	{
		free(line);
		fprintf(stdout, "Error: missing [empty], [obstacle], [full]\n");
		if (filestream)
			fclose(filestream);
		return (NULL);
	}
    data->each_line = number; //number of lines in the map
    data->empty = line[pos]; //get [.]
    data->obst = line[pos+1]; //get [o]
    data->full = line[pos+2]; //get [x]
    free(line); //free the line after use it

    //allocate memory for map
    map = malloc((data->each_line + 1) * sizeof(char *));
    if (!map)
    {
        if(filestream)
            fclose(filestream);
        fprintf(stdout, "Error: malloc error\n");
        return NULL;
    }
    //set all pointer to null
    for(int i = 0; i <= data->each_line; i++)
        map[i] = NULL;

    //read with stdin or file
    int i = 0;
    while(1)
    {
        char *line = NULL;
        size_t len = 0;
        if(getline(&line, &len, filestream) == -1)
        {
            free(line);
            break;
        }
        // check if extra line
        if (i >= data->each_line)
        {
            free(line);
            free_arr(map);
            if(filestream)
                fclose(filestream);
            fprintf(stderr, "Error: map has more line than specified\n");
            return NULL;
        }
        //check if last line is newline
        int line_len = ft_strlen(line);
        if (line[line_len-1] == '\n')
        {
            line[line_len-1] = '\0';
            line_len--;
        }
        //check if line length is same as previous line then assign line to this current line
        if (i == 0)
            data->line_len = line_len;
        //check if line not same with prev line
        else if (line_len != data->line_len)
        {
            fprintf(stderr, "Error: dimension mismatch\n");
            free(line);
            free_arr(map);
            if(filename)
                fclose(filestream);
            return NULL;
        }
        //assign each line to map
        map[i] = line;
        i++;
    }
    if(filestream)
        fclose(filestream);

    //check if line number is same as specified in first line
    if (i != data->each_line)
    {
        free_arr(map);
        fprintf(stderr, "Error: map has fewer line than specified\n");
        return NULL;
    }
    //check if map contain only empty or obst
    for (int y = 0; y < data->each_line; y++)
    {
        for (int x = 0; x < data->line_len; x++)
        {
            if (map[y][x] != data->empty && map[y][x] != data->obst)
            {
                fprintf(stderr,"Error: map that read no [empty] and [obstacle]\n");
                free_arr(map);
                return NULL;
            }
        }
    }
    return map;
}

void print_filled_map(t_bsq data)
{
    int x, y = 0;

    while(y < data.each_line)
    {
        x = 0;
        while (x < data.line_len)
        {
            // if x and y in the square area then print full char else print the map char
            if (x >= data.row && x < data.row + data.sq_dimension &&
                y >= data.col && y < data.col + data.sq_dimension)
                fprintf(stdout, "%c", data.full);
            else
                fprintf(stdout, "%c", data.map[y][x]);
            x++;
        }
        y++;
        fprintf(stdout, "\n");
    }
}

int check_square(t_bsq data, int row, int col, int dimension)
{
    int y, x;
    y = 0;
    while (y < dimension)
    {
        x = 0;
        while (x < dimension)
        {
            if (data.map[col + y][row + x] == data.obst)
                return 0;
            x++;
        }
        y++;
    }
    return 1;
}

void find_bsq(t_bsq *data)
{
    //get the smaller dimension between line_len and each_line
    int dimension = data->line_len > data->each_line ? data->each_line : data->line_len;
    int y, x;

    //fill the square from the biggest dimension to smaller dimension
    while (dimension > 0)
    {
        y = 0;
        while (y + dimension <= data->each_line)
        {
            x = 0;
            while(x + dimension <= data->line_len)
            {
                //check if square is valid in the map
                if (check_square(*data, x, y, dimension) == 1)
                {
                    data->sq_dimension =dimension;
                    data->row = x;
                    data->col = y;
                    return ;
                }
                x++;
            }
            y++;
        }
        dimension--;
    }
}

void init_data(t_bsq *data)
{
    data->each_line = 0;
    data->line_len = 0;
    data->empty = '\0';
    data->obst = '\0';
    data->full = '\0';
    data->map = NULL;
    data->sq_dimension = 0;
    data->row = 0;
    data->col = 0;
}

int main(int ac, char **av)
{
    t_bsq data;
    init_data(&data);
    if (ac == 1)
        data.map = read_file(NULL, &data);
    else if (ac == 2)
        data.map = read_file(av[1], &data);
    else
        fprintf(stdout, "Usage: ./a.out [file]\n");
    if (data.map)
    {
        find_bsq(&data);
        if(ac == 1)
            fprintf(stdout, "\n");
        print_filled_map(data);
        free_arr(data.map);
    }
}