#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include "mbed.h"

using namespace mbed;

void solveMaze(void);
void IROutput(void);
void mazeSolverSetup(void);
void IRAveraging(void);
void wallAlign(void);
void moveToObstacle(void);
void rotateToFinish(void);
void deadEndAvoid(void);

#endif //MAZESOLVER_H