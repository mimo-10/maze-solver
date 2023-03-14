#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "solver.h"

#define PRINT 0 // Whether or not to print the maze during flooding

void printDistances(Maze* maze)
{
    // printf("maze distances\n");
    for (int i = 0; i < strlen(maze->values); i++)
    {
    printf("[%d : %d]",i, maze->distances[i]);

    }
    // {
    //     printf("[%d]", maze->distances[i]);
    //     if (i-1 % maze->width == 0)
    //         printf("\n");
    // }
    // printf("\n");
}

int main()
{
    Maze* maze = (Maze*) malloc(sizeof(Maze));

    // Read and intialize maze
    getMaze("./maze2.txt", maze);
    printMaze(maze);
    // Finds the optimal solution using a modified flood fill from all exits
    findSolution(maze);
    printDistances(maze);

    // Solves the maze by taking the fastest path
    showSolution(maze);
    
    // Print the maze with the intitial formatting
    //prettyPrintMaze(maze);
    printMaze(maze);
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

void prettyPrintMaze(Maze* maze)
{
    char ch = ' ';
    int counter = 0;

    while (ch != '\0')
    {
        ch = maze->values[counter++];
        switch (ch)
        {
        case '#':
            printf("%c", '#');
            break;
        case '^':
            printf("%c", '^');
            break;
        case 'E':
            printf("%c", 'E');
            break;
        case '\n':
            printf("\n");
            break;
        default:
            printf("%c", ' ');
            break;
        }
    }
    printf("\n");
}
void printMaze(Maze* maze)
{
    printf("%s\n", maze->values);
}


void getMaze(const char* path, Maze* maze)
{
    FILE *fp;
    char values[MAX_WIDTH * MAX_HEIGHT];
    int distances[MAX_WIDTH * MAX_HEIGHT];
    char ch;
    int counter = 0;
    int row = 0;
    int goalsCounter = 0;

    maze->width = 0;

    fp = fopen(path, "r");

    // Read chars and convert special characters
    while (ch != EOF) 
    {
        ch = fgetc(fp);
        
        if ( ch == 'E')
        {
            maze->goals[goalsCounter++] = counter;
            values[counter] = 'E'; // Lowest char above [SPACE]
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
    printf("GOALS COUNTER %d\n", goalsCounter);
    maze->goalsCounter = goalsCounter;
    values[counter-1] = '\0';
    strcpy(maze->values, values);
}

int flood(int pos, Maze* maze, Queue* q)
{
    printf("GOALS %d\n", maze->goalsCounter);
    /* TODO
    fix this shit. Char is not enough for the distance in bigger mazes.
    
    */
    // Distance to cell above and below any given cell is the width
    const int dd =  maze->width;  
    const int du = -maze->width; 
    const int dr = 1;
    const int dl = -1;

    int endFound = 0;

    // printf("POS: %d CHAR: [%c]\n", pos, maze->values[pos]);
    
    // Add initial position to queue
    __int32_t n;
    __int16_t counter  = 0;
    __int16_t distance = 0;
    __int16_t position = 0;

    // A queue element n will store both the position of the cell
    // and its manhattan distance to the flood origin
    enQueue(q, (pos << 16) | (distance));

    // Queue is empty when both pointers are -1
    while ( q->front != -1 && q->back != -1)
    {   
        // printf("----WHILE----\n");
        // printf("Front: %d Back: %d", q->front, q->back);
        n = q->items[q->front];
        distance = (__int16_t)(n & 65535);
        position = (__int16_t)(n >> 16);
        // printf("--(n: %d)--(distance: (%c))--\n", n, distance);
        // printf("Front: %d Back: %d\n", q->front, q->back);
        // printf("Up: (%c) Down: (%c) Left: (%c) Right: (%c)\n", maze->values[du],maze->values[dd],maze->values[dl],maze->values[dr]);
        deQueue(q);

        // Check if position is in bounds
        if ( (position < 0) || (position > strlen(maze->values)) || (maze->values[position] == '\n') )
        {
            continue;
        }

        if(maze->values[position] == '#')
        {
            maze->distances[position] = INT_MAX;
            continue;
        }

        // Continue if an already set position is better
        // TODO: An iterations shouldn't check spaces filled by itself at all 
        if ( (maze->values[position] == 'F') && (maze->distances[position] < distance) )
        {
            continue;
        }
        // printf("GOALS %d distance %d\n", maze->goalsCounter, distance);
        // Return as soon as solution is found
        if ( (maze->values[position] == '^') )
        {
            maze->distances[position] = distance;
            return(1);
        }
    
        // printMaze(maze);
        // Set value for position
        maze->values[position]    = 'F';
        maze->distances[position] = distance;
        // printf("Distance %d\n", maze->distances[position]);

        enQueue(q, ((position + du) << 16) | (distance + 1));
        enQueue(q, ((position + dl) << 16) | (distance + 1));
        enQueue(q, ((position + dr) << 16) | (distance + 1));
        enQueue(q, ((position + dd) << 16) | (distance + 1));
        // printDistances(maze);
        // printf("\n\n");

        #if PRINT
        printf("Char: \n%s\n", maze->values);
        printf("Position: %d\n", position);
        printf("Distance: %d\n", distance);
        usleep(100*1000);
        #endif
    }
    printf("GOALS %d\n", maze->goalsCounter);
}

void findSolution(Maze* maze)
{
    // Init queue for flood
    Queue* q = (Queue*) malloc(sizeof(Queue));
    int solutionFound = 0;
    for (int i = 0; i < maze->goalsCounter; i++)
    {
        initQueue(q);

        printf("Flood %d\n", i+1);
        // printf("Starting at char (%c) at pos (%d)\n", maze->values[maze->goals[i]], maze->goals[i]);

        if ( flood(maze->goals[i], maze, q) )
            solutionFound = 1;
        // printMaze(maze);
    }
    if (!solutionFound)
    {
        printf("No solution found!\n");
        exit(1);
    }
}
int isLegalMove(Maze* maze, int p, int d, int bestStep)
{
    printf("%d %d %d %d\n", d < bestStep, (d != 0), p > 0, p < strlen(maze->values));
    printf("%d %d\n", d, bestStep);

    if ( d < bestStep && p > 0 && p < strlen(maze->values) )
    {
        if (d == 0)
            return(bestStep == 1);
        else   
            return(1);
    }

    return(0);
}

void showSolution(Maze* maze){
    int pos = maze->start;
    int u, d, l, r;
    int ud, dd, ld, rd;
    int bestStep;
    int found = 0;
    do
    {
        printf("SHOW\n");
        // printMaze(maze);
        // printf("\n\n");
        // Find the smallest neighbour
        u = pos - maze->width;
        d = pos + maze->width;
        r = pos + 1;
        l = pos - 1;

        ud = maze->distances[u]; ld = maze->distances[l]; dd = maze->distances[d]; rd = maze->distances[r];
        bestStep = maze->distances[pos];
        // printf("maze len %d\n", maze->length);
        // printf("%d %d %d %d\n", ud < bestStep, (ud != 0), u > 0, u < strlen(maze->values));

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

        printf("BEST STEP: %d POS: %d\n", bestStep, pos);


        if ( bestStep == 0 )
            found = 1;

        maze->values[pos] = '*';
        // printDistances(maze);
    } while ( !found );
        // exit(1);
}
