#include "mbed.h"
#include "MotionController.h"
#include "IR.h"
#include "Map.h"
#include "MazeSolver.h"

#define DIST_BETWEEN_FRONT_IR 35
#define AVERAGING_RESOLUTION 10
#define MAX_IR_READING 500
#define MIN_IR_READING 52
#define SHUFFLE_DISTANCE 150
#define MAX_DIST_FROM_WALL 100
#define TURN_DISTANCE 50
#define FRONT_IR 0
#define LEFT_IR 1
#define RIGHT_IR 2
#define PAUSE_MOVEMENT_DELAY 300000

int angleToRotate = 0;
bool canMoveLeft = false;
bool canMoveRight = false;
bool canMoveForward = true;
bool canMoveBackward = true;

IR frontLeftIR(LEFT_FRONT);
IR frontRightIR(RIGHT_FRONT);
IR sideLeftIR(LEFT_SIDE);
IR sideRightIR(RIGHT_SIDE);

enum MovementMode
{
    DRIVE_TO_FINISH,
    FOLLOW_OBSTACLE,
    SELECT_DIRECTION,
    STOP
};

MovementMode movementMode;
// list to store front and side sensors readings
float IROutputList[5];

void IRAveraging()
{
    float frontSum = 0;
    float leftSum = 0;
    float rightSum = 0;
    float frontLeft = 0;
    float frontRight = 0;
    float frontToAdd = 0;
    float leftToAdd = 0;
    float rightToAdd = 0;

    for (int i = 0; i < AVERAGING_RESOLUTION; i++)
    {
        frontLeft = frontLeftIR.read();
        frontRight = frontRightIR.read();

        // if one sensor senses something close but the other senses something far, take the smaller reading
        if (abs(frontLeft - frontRight) > 120)
        {
            if (frontLeft > frontRight)
            {
                frontLeft = frontRight;
            }
            else
            {
                frontRight = frontLeft;
            }
        }
        frontToAdd = (frontLeft + frontRight) / 2;
        leftToAdd = sideLeftIR.read();
        rightToAdd = sideRightIR.read();
        // cap front sensor measurments within bounds
        if ((frontToAdd < MAX_IR_READING) & (frontToAdd > MIN_IR_READING))
        {
            frontSum += frontToAdd;
        }
        else if (frontToAdd < MIN_IR_READING)
        {
            frontSum += MIN_IR_READING;
        }
        else
        {
            frontSum += MAX_IR_READING;
        }

        // cap left sensor within bounds
        if ((leftToAdd < MAX_IR_READING) & (leftToAdd > MIN_IR_READING))
        {
            leftSum += leftToAdd;
        }
        else if (leftToAdd < MIN_IR_READING)
        {
            leftSum += MIN_IR_READING;
        }
        else
        {
            leftSum += MAX_IR_READING;
        }

        // cap right sensor within bounds
        if ((rightToAdd < MAX_IR_READING) & (rightToAdd > MIN_IR_READING))
        {
            rightSum += rightToAdd;
        }
        else if (rightToAdd < MIN_IR_READING)
        {
            rightSum += MIN_IR_READING;
        }
        else
        {
            rightSum += MAX_IR_READING;
        }

        wait_us(1000);
    }

    IROutputList[FRONT_IR] = frontSum / AVERAGING_RESOLUTION;
    IROutputList[LEFT_IR] = leftSum / AVERAGING_RESOLUTION;
    IROutputList[RIGHT_IR] = rightSum / AVERAGING_RESOLUTION;
    IROutputList[3] = frontLeft;
    IROutputList[4] = frontRight;
}

void mazeSolverSetup(void)
{
    movementMode = DRIVE_TO_FINISH;
    motorControl.setCurrentAngle(0);
}

void wallAlign()
{
    IRAveraging();
    if (IROutputList[0] > 200)
    {
        moveToObstacle();
    }
    motorControl.setAlign(true);
    for (int i = 0; i < 2; i++)
    {

        IRAveraging();

        float left = IROutputList[3];
        float right = IROutputList[4];
        float theta = 0;

        if (abs(left - right) < 2)
        {
        }
        else if (right > left)
        {
            theta = (180 / PI) * atan((right - left) / DIST_BETWEEN_FRONT_IR);
            motorControl.rotate(-theta);
        }
        else if (left > right)
        {
            theta = (180 / PI) * atan((left - right) / DIST_BETWEEN_FRONT_IR);
            motorControl.rotate(theta);
        }
    }
    motorControl.setAlign(false);
    IRAveraging();
    float frontDist = IROutputList[FRONT_IR];

    if (frontDist > MAX_DIST_FROM_WALL)
    {
        motorControl.forwardDist((frontDist - MAX_DIST_FROM_WALL));
    }
    else
    {
        motorControl.reverseDist((MAX_DIST_FROM_WALL - frontDist));
    }
}

void moveToObstacle()
{
    // check front sensors
    IRAveraging();
    float frontDist = IROutputList[FRONT_IR];
    // if room to move forward
    if (frontDist > MAX_DIST_FROM_WALL)
    {
        // move until next obstacle
        motorControl.forwardDist((frontDist - MAX_DIST_FROM_WALL));
    }
    wallAlign();
    drawObstacle();
}

void rotateToFinish()
{
    // rotate towards finish
    int angle = motorControl.getCurrentAngle();

    if (angle == 90)
    {
        motorControl.rotate(-90);
    }
    else if (angle == 270)
    {
        motorControl.rotate(90);
    }
    else if (angle == 180)
    {
        motorControl.rotate(180);
    }
    else if (angle == 0)
    {
    }
    else
    {
        errorScreen();
    }
}

void deadEndAvoid(void)
{
    motorControl.rotate(180);
    wait_us(PAUSE_MOVEMENT_DELAY);
    IRAveraging();

    motorControl.rotate(180);
    wait_us(PAUSE_MOVEMENT_DELAY);
    wallAlign();
    wait_us(PAUSE_MOVEMENT_DELAY);
    motorControl.reverseDist(IROutputList[FRONT_IR] + SHUFFLE_DISTANCE);
    wait_us(PAUSE_MOVEMENT_DELAY);

    IRAveraging();

    if (IROutputList[LEFT_IR] >= TURN_DISTANCE)
    {
        canMoveLeft = true;
    }
    else
    {
        canMoveLeft = false;
    }

    if (IROutputList[RIGHT_IR] >= TURN_DISTANCE)
    {
        canMoveRight = true;
    }
    else
    {
        canMoveRight = false;
    }

    if ((canMoveLeft == true) & (canMoveRight == false))
    {
        // go left
        angleToRotate = -90;
    }
    else if ((canMoveLeft == false) & (canMoveRight == true))
    {
        // go right
        angleToRotate = 90;
    }
    else if ((canMoveLeft == true) & (canMoveRight == true))
    {
        if (IROutputList[LEFT_IR] > IROutputList[RIGHT_IR])
        {
            angleToRotate = -90;
        }
        else
        {
            angleToRotate = 90;
        }
    }
    motorControl.rotate(angleToRotate);
    IRAveraging();
    motorControl.forwardDist(IROutputList[FRONT_IR] - MAX_DIST_FROM_WALL);
    movementMode = DRIVE_TO_FINISH;
}

void solveMaze()
{
    switch (movementMode)
    {
    case STOP:
    {
        motorControl.stop();
        printGrid();
        break;
    }
    case DRIVE_TO_FINISH:
    {
        rotateToFinish();
        moveToObstacle();

        if (getCurrentX() <= 2)
        {
            movementMode = STOP;
        }
        else
        {
            movementMode = SELECT_DIRECTION;
        }

        break;
    }

    case FOLLOW_OBSTACLE:
    {
        IRAveraging();

        while (IROutputList[FRONT_IR] < MAX_DIST_FROM_WALL)
        {
            drawObstacle();
            motorControl.rotate(angleToRotate);
            motorControl.forwardDist(SHUFFLE_DISTANCE);
            wait_us(PAUSE_MOVEMENT_DELAY);
            rotateToFinish();
            wait_us(PAUSE_MOVEMENT_DELAY);
            IRAveraging();
            wallAlign();
            wait_us(PAUSE_MOVEMENT_DELAY);
        }

        wait_us(PAUSE_MOVEMENT_DELAY);
        movementMode = DRIVE_TO_FINISH;
        break;
    }
    case SELECT_DIRECTION:
    {
        rotateToFinish();
        wait_us(PAUSE_MOVEMENT_DELAY);
        IRAveraging();
        if (IROutputList[FRONT_IR] >= MAX_DIST_FROM_WALL)
        {
            canMoveForward = true;
        }
        else
        {
            canMoveForward = false;
        }

        if (IROutputList[LEFT_IR] >= TURN_DISTANCE)
        {
            canMoveLeft = true;
        }
        else
        {
            canMoveLeft = false;
        }

        if (IROutputList[RIGHT_IR] >= TURN_DISTANCE)
        {
            canMoveRight = true;
        }
        else
        {
            canMoveRight = false;
        }

        if (canMoveForward == true)
        {
            // move forward
            movementMode = DRIVE_TO_FINISH;
            break;
        }
        else if ((canMoveLeft == true) & (canMoveRight == false))
        {
            // go left
            angleToRotate = -90;
        }
        else if ((canMoveLeft == false) & (canMoveRight == true))
        {
            // go right
            angleToRotate = 90;
        }
        else if ((canMoveLeft == true) & (canMoveRight == true))
        {
            if (IROutputList[LEFT_IR] > IROutputList[RIGHT_IR])
            {
                angleToRotate = -90;
            }
            else
            {
                angleToRotate = 90;
            }
        }
        else if ((canMoveLeft == false) & (canMoveRight == false))
        {
            deadEndAvoid();
            break;
        }

        movementMode = FOLLOW_OBSTACLE;
        break;
    }
    }
}

void IROutput()
{
    Serial.println("IR:");
    Serial.print("FRONT AVERAGE: ");
    Serial.println(IROutputList[FRONT_IR]);
    Serial.print("SIDE LEFT: ");
    Serial.println(IROutputList[LEFT_IR]);
    Serial.print("SIDE RIGHT: ");
    Serial.println(IROutputList[RIGHT_IR]);
    Serial.print("FRONT LEFT: ");
    Serial.println(IROutputList[3]);
    Serial.print("FRONT RIGHT: ");
    Serial.println(IROutputList[4]);
}