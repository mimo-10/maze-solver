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
    

    printf("%d\n", maze->goals[0]);
    flood(maze->goals[0], maze, q);
    printMaze(maze);
    solve(maze);
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
    int goalsCounter = 0;

    maze->width = 0;

    fp = fopen(path, "r");

    while (ch != EOF) 
    {
        ch = fgetc(fp);

        if ( ch == 'E')
        {
            maze->goals[goalsCounter++] = counter;
            values[counter] = '!'; // Lowest char above [SPACE]
        }
        else if ( ch == '\n')
        {
            // Width = counter up until first newline, i.e. length of a row
            if ( maze->width == 0) maze->width = counter;
            row++;
            values[counter] = '\n';
        } 
        else if ( ch == '^')
        {
            values[counter] = '~';
            maze->start = counter;
        }
        else if ( ch == ' ') 
        {
            values[counter] = ' ';
        }
        else if ( ch == '#') 
        {
            values[counter] = '}';
        }

        counter++;
    }

    values[counter-1] = '\0';
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
    char distance = ' ';
    enQueue(q, (pos << 8) | (distance + 1));

    // Queue is empty when both pointers are -1
    while ( q->front != -1 && q->back != -1)
    {

        n = q->items[q->front];
        distance = (char)(n & 255);
        n >>= 8;
        // printf("--(n: %d)--(distance: (%c))--\n", n, distance);
        
        deQueue(q);

        // printf("%c, %d\n", distance, q->back);

        // Check if position is in bounds
        if ( (n < 0) || (n > strlen(maze->values)) || (maze->values[n] == '}') || (maze->values[n] == '\n'))
        {
            continue;
        }

        // Continue if an already set position is better
        if ( (maze->values[n] != ' ') && (maze->values[n] < distance) )
        {
            continue;
        }
        // Continue if position is the starting position
        if ( (maze->values[n] == '~') )
            continue;

        // Set value for position
        maze->values[n] = distance;

        enQueue(q, ((n + du) << 8) | (distance + 1));
        enQueue(q, ((n + dr) << 8) | (distance + 1));
        enQueue(q, ((n + dd) << 8) | (distance + 1));
        enQueue(q, ((n + dl) << 8) | (distance + 1));

        #if PRINT
        printMaze(maze);
        usleep(200*1000);
        #endif
    }
}

void solve(Maze* maze){
    int pos = maze->start;
    int u, d, l, r;
    char uc, dc, lc, rc;
    char next;
    int found = 0;
    do
    {
        printMaze(maze);
        printf("\n\n");

        // Find the smallest neighbour
        u = pos - maze->width;
        d = pos + maze->width;
        r = pos + 1;
        l = pos - 1;

        uc = maze->values[u]; lc = maze->values[l]; dc = maze->values[d]; rc = maze->values[r];

        next = uc;
        pos = u;
        printf("U:%c L:%c D:%c R:%c\n", uc, lc, dc, rc);
        printf("%c POS: %d\n", maze->values[pos], pos);
        if(next > lc && lc != 0)
        {
            next = lc;
            pos = l;
        }
        printf("(%c)\n", next);

        if(next > dc && dc != 0)
        {
            next = dc;
            pos = d;
        }

        printf("(%c)\n", next);

        if(next > rc && rc != 0)
        {
            next = rc;
            pos = r;
        }
        printf("(%c)\n", next);
        
        printf("%d %d\n", next, maze->values[u]);
        

        printf("%c POS: %d\n", maze->values[pos], pos);
        usleep(100*1000);

        if ( maze->values[pos] == '!' )
            found = 1;

        maze->values[pos] = '~';
    } while ( !found );
}
