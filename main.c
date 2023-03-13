#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "solver.h"

#define PRINT 0 // Whether or not to print the maze during flooding

int main()
{
    // Read maze files
    Maze* maze = (Maze*) malloc(sizeof(Maze));
    getMaze("./maze.txt", maze);

    // Init queue for flood
    Queue* q = (Queue*) malloc(sizeof(Queue));
    initQueue(q);

    flood(maze->start, maze, q);
    printMaze(maze);
    printf("\u25A0\n");
    return(0);
}

void enQueue(Queue* q, int value)
{
    if (q->back == MAX_Q_SIZE - 1) return;
    
    if (q->front == -1) q->front = 0;
    
    q->back++;
    q->items[q->back] = value;
    
}
void deQueue(Queue* q)
{
    if (q->front == -1) return;

    q->front++;

    if (q->front > q->back)
    {
        q->front = -1;
        q->back = -1;
    }
}
void initQueue(Queue* q)
{
    q->front = -1;
    q->back = -1;
}


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

void flood(int pos, Maze* maze, Queue* q)
{
    // Distance to cell above and below any given cell is the width
    const int dd = maze->width;  
    const int du = -maze->width; 
    const int dr = 1;
    const int dl = -1;

    // Return if out of bounds
    if ( (pos < 0) || (pos > strlen(maze->values)) ) return;

    printf("POS: %d CHAR: [%c]\n", pos, maze->values[pos]);
    
    // Add initial position to queue
    int n;
    int counter = 0;
    char distance = '#';
    enQueue(q, (pos << 8) | (distance + 1));

    // Queue is empty when both pointers are -1
    while ( q->front != -1 && q->back != -1)
    {

        n = q->items[q->front];
        printf("%d\n", n);
        distance = (char)(n & 255);
        n >>= 8;
        printf("--(n: %d)--(distance: (%c))--\n", n, distance);
        
        deQueue(q);

        printf("%c\n", distance);

        // Check if position is in bounds
        if ( (n < 0) || (n > strlen(maze->values)) || (maze->values[n] == '#') || (maze->values[n] == '\n'))
        {
            printf("Out of bounds\n");
            continue;
        }

        // Continue if position is already set
        if ( (maze->values[n] > ' ') && (maze->values[n] < '^') )
         {
            printf("Out of range\n");
            continue;
        }

        // Set value for position
        maze->values[n] = distance;

        enQueue(q, ((n + du) << 8) | (distance + 1));
        enQueue(q, ((n + dr) << 8) | (distance + 1));
        enQueue(q, ((n + dd) << 8) | (distance + 1));
        enQueue(q, ((n + dl) << 8) | (distance + 1));

        #if PRINT
        printMaze(maze);
        usleep(1000*1000);
        #endif
    }

}
