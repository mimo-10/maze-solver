
#define MIN(a,b) ((a < b) ? a : b)
#define MAX(a,b) ((a > b) ? a : b)
#define ABS(x)   ((x > 0) ? -x : x)

#define MAX_WIDTH  30
#define MAX_HEIGHT 30
#define MAX_GOALS  10
#define MAX_PATH   100
#define MAX_Q_SIZE 10000000

typedef struct {
    char values[MAX_HEIGHT * MAX_WIDTH];
    int height;
    int width;
    int goals[MAX_GOALS];
    int goalsCounter;
    int start;
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
void flood(int pos, Maze* maze, Queue* q);
void solve(Maze* maze);