#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include "mbed.h"

using namespace mbed;

//switch case to loop through finite state machine to sovle the maze
void solveMaze(void);
//print IR values to serial
void IROutput(void);
// sets default robot heading to face the finish, sets the finite state machine to start by driving towards the finish
void mazeSolverSetup(void);
//takes a number of readings from the IR sensors then averages them, capping them at max and min values
void IRAveraging(void);
// uses triganometry to align the robot with a wall, then move a set distance away from it
void wallAlign(void);
//measure distance from obstacle then drive until a set distance away is reached
void moveToObstacle(void);
//compare current angle and rotate to the finish
void rotateToFinish(void);
//if surrounded and there is room to reverse, reverse then move until the farthest wall on the left or right is reached
void deadEndAvoid(void);

#endif //MAZESOLVER_H