#include "solver.h"

int main(int argc, char* argv[])
{
    Maze* maze = (Maze*) malloc(sizeof(Maze));
    if ( maze == NULL )
        return(1);

    // Read and intialize maze
    getMaze(argv[1], maze);

    // Find the optimal solution using a modified flood fill from all exits
    if ( floodAll(maze) == 1)
    {
        printf("Could not find a solution\n");
        return(1);
    };
    
    // Solves the maze by taking the fastest path
    if ( showShortestPath(maze, atoi(argv[2])) == 1 ) 
    {
        printf("No solutions for the maze in under %d steps\n", atoi(argv[2]));
        return(1);
    };
    
    // Print the maze
    printMaze(maze);
    
    return(0);
}