
#define MIN(a,b) ((a < b) ? a : b)
#define MAX(a,b) ((a > b) ? a : b)
#define ABS(x)   ((x > 0) ? -x : x)

#define MAX_WIDTH  50
#define MAX_HEIGHT 50
#define MAX_GOALS  10
#define MAX_PATH   100
#define MAX_Q_SIZE 100000 // This is probably a bad idea considering embedded systems, I think a circular queue would solve it

typedef struct {
    char values[MAX_HEIGHT * MAX_WIDTH];
    int  distances[MAX_HEIGHT * MAX_WIDTH];
    int  height;
    int  width;
    int  goals[MAX_GOALS];
    int  goalsCounter;
    int  start;
    int  length;
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
int  flood(int pos, Maze* maze, Queue* q);
void findSolution(Maze* maze);
void showSolution(Maze* maze);
int  isLegalMove(Maze* maze, int p, int d, int bestStep);