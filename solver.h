#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h> // in order the use the standard `int` for a universel sizing;

#define MAX_WIDTH  50
#define MAX_HEIGHT 50
#define MAX_GOALS  10
#define MAX_PATH   100
#define MAX_Q_SIZE 100000 // This is probably a bad idea considering embedded systems, I think a circular queue would solve it

typedef struct {
    char values[MAX_HEIGHT * MAX_WIDTH];
    int  distances[MAX_HEIGHT * MAX_WIDTH];
    int  width;
    int  goals[MAX_GOALS];
    int  goalsCounter;
    int  start;
} Maze;

typedef struct {
    int front;
    int back;
    int items[MAX_Q_SIZE];
} Queue;

void enQueue(Queue* q, int value);
void deQueue(Queue* q);
void initQueue(Queue* q);
void getMaze(const char* path, Maze* maze);
void printMaze(Maze* maze);
int  floodAll(Maze* maze);
int  showShortestPath(Maze* maze, int maxSteps);
int  flood(int pos, Maze* maze, Queue* q);
int  isLegalMove(Maze* maze, int p, int d, int bestStep);


void enQueue(Queue* q, int value)
{
    if (q->back == MAX_Q_SIZE - 1) 
        return;

    if (q->front == -1) 
        q->front = 0;
    
    q->back++;
    q->items[q->back] = value;
    
}
void deQueue(Queue* q)
{
    if (q->front == -1) 
        return;

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
    FILE* fp;
    char  values[MAX_WIDTH * MAX_HEIGHT];
    int   distances[MAX_WIDTH * MAX_HEIGHT];
    char  ch;
    int   counter = 0;
    int   row = 0;
    int   goalsCounter = 0;

    maze->width = 0;

    fp = fopen(path, "r");

    // Read chars and convert special characters
    while ( ch != EOF ) 
    {
        ch = fgetc(fp);
        
        if ( ch == 'E')
        {
            maze->goals[goalsCounter++] = counter;
            values[counter] = 'E'; 
        }
        else if ( ch == '\n')
        {
            // Width = counter up until first newline, i.e. length of a row
            if ( maze->width == 0) 
                maze->width = counter+1;
            row++;
            values[counter] = '\n';
        } 
        else if ( ch == '^')
        {
            values[counter] = '^';
            maze->start = counter;
        }
        else if ( ch == ' ') 
        {
            values[counter] = ' ';
        }
        else if ( ch == '#') 
        {
            values[counter] = '#';
        }

        counter++;
    }

    maze->goalsCounter = goalsCounter;
    strcpy(maze->values, values);
}

int flood(int pos, Maze* maze, Queue* q)
{
    // Distance to cell above and below any given cell is the width
    const int dd =  maze->width;  
    const int du = -maze->width; 
    const int dr = 1;
    const int dl = -1;

    int endFound = 0;
    // imported from stdint, this will make it universel;
    int32_t n; 
    int16_t counter  = 0;
    int16_t distance = 0;
    int16_t position = 0;

    // A queue element n will store both the position of the cell
    // and its manhattan distance to the flood origin
    enQueue(q, (pos << 16) | (distance));

    // Queue is empty when both pointers are -1
    while ( q->front != -1 && q->back != -1)
    {   
        n = q->items[q->front];

        // Extract distance and position from the compound element
        distance = (int16_t)(n & 65535); // imported from stdint, this will make it universel;
        position = (int16_t)(n >> 16);
        
        deQueue(q);

        // Check if position is in bounds
        if ( (position < 0) || (position > strlen(maze->values)) || (maze->values[position] == '\n') )
            continue;

        // Continue if wall
        if(maze->values[position] == '#')
        {
            maze->distances[position] = INT_MAX;
            continue;
        }

        // Continue if an already set position is better
        if ( (maze->values[position] == 'F') && (maze->distances[position] < distance) )
            continue; // TODO: An iterations shouldn't check spaces filled by itself at all 

        // Check if starting position
        if ( maze->values[position] == '^' )
        {
            maze->distances[position] = distance;
            return(1);
        }
    
        // Set distance value for position and note filled
        maze->values[position]    = 'F';
        maze->distances[position] = distance;

        enQueue(q, ((position + du) << 16) | (distance + 1));
        enQueue(q, ((position + dl) << 16) | (distance + 1));
        enQueue(q, ((position + dr) << 16) | (distance + 1));
        enQueue(q, ((position + dd) << 16) | (distance + 1));
    }
    return(0);
}

int floodAll(Maze* maze)
{
    // Init queue for flood
    Queue* q = (Queue*) malloc(sizeof(Queue));
    int    solutionFound = 0;

    for (int i = 0; i < maze->goalsCounter; i++)
    {
        initQueue(q);
        
        if ( flood(maze->goals[i], maze, q) )
            solutionFound = 1;
    }
    if ( !solutionFound )
    {
        return(1);
    }
    return(0);
}

int isLegalMove(Maze* maze, int p, int d, int bestStep)
{
    // Check if next distance is better and next position is in bounds
    if ( d < bestStep && p > 0 && p < strlen(maze->values) )
    {
        // Next distance may be 0 iff current distance is 1
        if (d == 0)
            return(bestStep == 1);
        else   
            return(1);
    }

    return(0);
}

int showShortestPath(Maze* maze, int maxSteps){
    int pos = maze->start;
    int u, d, l, r;     // Positions
    int ud, dd, ld, rd; // Distances
    int bestStep;
    int found = 0;
    int counter = 0;

    do
    {
        if ( counter++ > maxSteps)
        {
            if ( !found )
                return(1);
        }

        // Find adjacent cells
        u = pos - maze->width;
        d = pos + maze->width;
        r = pos + 1;
        l = pos - 1;
        
        // Get distances of adjacent cells
        ud = maze->distances[u];
        ld = maze->distances[l];
        dd = maze->distances[d];
        rd = maze->distances[r];

        // Init best move to the current cell
        bestStep = maze->distances[pos];

        // Find the best move 
        if ( isLegalMove(maze, u, ud, bestStep) )
        {
            bestStep = ud;
            pos = u;
        }

        if ( isLegalMove(maze, r, rd, bestStep) )
        {
            bestStep = rd;
            pos = r;
        }

        if ( isLegalMove(maze, d, dd, bestStep) )
        {
            bestStep = dd;
            pos = d;
        }

        if ( isLegalMove(maze, l, ld, bestStep) )
        {
            bestStep = ld;
            pos = l;
        }

        // If the best step is 0, the goal is found
        if ( bestStep == 0 )
            found = 1;
        
        // Leave trail of chars 
        maze->values[pos] = 'O';

    } while ( !found );

    // Clean up residue F's
    for ( int i = 0; i < strlen(maze->values); i++)
    {
        if ( maze->values[i] == 'F' ) 
            maze->values[i] = ' '; 
    }
    return(0);
}
