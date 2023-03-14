#include "solver.h"

int main(int argc, char* argv[])
{
    Maze* maze = (Maze*) malloc(sizeof(Maze));
    printf("%s\n", argv[1]);

    // Read and intialize maze
    getMaze(argv[1], maze);

    // Find the optimal solution using a modified flood fill from all exits
    findSolution(maze);

    // Solves the maze by taking the fastest path
    showSolution(maze, atoi(argv[2]));
    
    // Print the maze
    printMaze(maze);
    
    return(0);
}