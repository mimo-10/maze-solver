#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_WIDTH  30
#define MAX_HEIGHT 30

typedef struct {
    char values[MAX_HEIGHT * MAX_WIDTH];
    int height;
    int width;
    int start;
} Maze;

void printMaze(Maze* maze)
{
    printf("%s\n", maze->values);
}

void getMaze(const char* path, Maze* maze)
{
    FILE *fp;
    char values[MAX_WIDTH * MAX_HEIGHT];
    char ch;
    int counter = 0;
    int row = 0;

    maze->width = 0;

    fp = fopen(path, "r");

    while (ch != EOF) {
        ch = fgetc(fp);

        if ( ch == '^')
            maze->start = counter;

        // Newline => new row
        if ( ch == '\n'){
            // Width = counter up until first newline, i.e. length of a row
            if ( maze->width == 0) maze->width = counter;
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

    // Distance to cell above  and below any given cell is the width
    const int down  = maze->width;  
    const int up    = -maze->width; 

    const int right = 1;
    const int left  = -1;

    // Ignore edges, already flooded cells and out of bounds
    if ( (pos < 0) || (pos > strlen(maze->values)) || (maze->values[pos] == '#') || (maze->values[pos] == 'F') || (maze->values[pos] == '\n')) return;
    
    printf("POS: %d CHAR: [%c]\n", pos, maze->values[pos]);
    
    printMaze(maze);
    
    // Fill only empty spaces
    if ( maze->values[pos] == ' ' ) maze->values[pos] = 'F';

    usleep(50*1000);

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

    printMaze(maze);
    flood(maze->start, maze);
    printMaze(maze);

    return(0);
}