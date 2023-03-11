#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WIDTH  30
#define MAX_HEIGHT 30

typedef struct {
    char values[MAX_HEIGHT * MAX_WIDTH];
    int height;
    int width;
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

        // Newline => new row
        if ( ch == '\n'){
            maze->width = counter;
            row++;
        }

        values[counter] = ch;
        counter++;

    }

    values[counter] = '\0';
    strcpy(maze->values, values);
    maze->height = row;
}

typedef struct TreeNode TreeNode;

struct TreeNode
{
  int value;
  TreeNode* next;
  TreeNode* prev;
};
int main()
{
    // Read maze files
    Maze* maze = (Maze*) malloc(sizeof(Maze));
    getMaze("./maze.txt", maze);

    printf("%s\n", maze->values);


    // Convert mazes to graph
    // Solve with A*
    return(0);
}