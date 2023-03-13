# Queue Flood fill maze solver

## The program
The program will execute the following steps to solve a maze:
1. Read a maze txt file
2. Replace special characters
3. Perform a modified queue based flood fill on the maze for every exit
4. Move the agent to the adjacent cell with the lowest value until the value is 0 (optimal exit reached)

## Modified queue based flood fill
Modified here refers to the fact that the algorithm needs to not only flood, but to flood the best way (to find the optimal exit path). This means that after the first flood fill, the subsequent flood fills need to be mindful to not overwrite and create worse paths. This is achieved by simply checking if the new value is greater than the current value. If it is, continue flooding and if not, terminate. We can terminate because if at any point the new value is lesser then the subsequent path can not be a better path than the existing path.

Queue is used to ensure that every cell gets the correct manhattan distance from the flood origin. 

## Replacing special characters
The algorithm at all times will operate on an array of chars. This means that while flooding, the manhattan distance will be represented by a char. 'E', '#' and '^' are all characters that would ideally be in the range of chars to represent the distance and are thus changed. 'E' is changed to '!', the lowest char in the range while '#' and '^' are changed to '{' and '\~' respectively. This means that the solving algorithm will never pick a wall or the starting position as any legal space will be lower than these two. The solver also replaces any previous cells with '\~' to mark the path its taken.
