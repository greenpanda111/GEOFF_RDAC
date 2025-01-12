#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include "mbed.h"

using namespace mbed;

void solveMaze(void);
void IROutput(void);
void mazeSolverSetup(void);
float IRAveraging(void);
void wallAlign(void);
void moveToObstacle(void);
void rotateToFinish(void);

#endif //MAZESOLVER_H