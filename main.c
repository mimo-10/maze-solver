#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WIDTH  30
#define MAX_HEIGHT 30

typedef struct {
    char values[MAX_HEIGHT * MAX_WIDTH];
    int height;
    int width;
    int start;
} Maze;

void getMaze(const char* path, Maze* maze)
{
    FILE *fp;
    char values[MAX_WIDTH * MAX_HEIGHT];
    char ch;
    int counter = 0;
    int row = 0;

    fp = fopen(path, "r");

    while (ch != EOF) {
        ch = fgetc(fp);
        if ( ch == '^')
            maze->start = counter;
        // Newline => new row
        if ( ch == '\n'){
            maze->width = counter;
            row++;
        }

        values[counter++] = ch;
    }

    values[counter] = '\0';
    strcpy(maze->values, values);
    maze->height = row;
}

void flood(int pos, Maze* maze)
{
    /* Directions: 1 - up, 2 - right, 3 - down, 4 -left */

    const int down  = maze->width;  // Offset to cell below any given cell
    const int up    = -maze->width; 
    const int right = 1;
    const int left  = -1;

    // Ignore edges, already flooded cells and out of bounds
    if ( (pos < 0) || (pos > strlen(maze->values)) || (maze->values[pos] == '#') || (maze->values[pos] == '0') ) return;
    
    maze->values[pos] = 'F';

    flood(pos + up,    maze);
    flood(pos + right, maze);
    flood(pos + down,  maze);
    flood(pos + left,  maze);
}
int main()
{
    // Read maze files
    Maze* maze = (Maze*) malloc(sizeof(Maze));
    getMaze("./maze.txt", maze);

    printf("%s\n", maze->values);
    flood(maze->start, maze);
    printf("%s\n", maze->values);

    // Convert maze to graph
    return(0);
}