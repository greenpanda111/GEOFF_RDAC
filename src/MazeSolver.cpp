#include "mbed.h"
#include "MotionController.h"
#include "IR.h"
#include "Map.h"

#define DIST_BETWEEN_FRONT_IR 35
#define AVERAGING_RESOLUTION 10
#define MAX_IR_READING 500
#define MIN_IR_READING 52
#define SHUFFLE_DISTANCE 50
#define MAX_DIST_FROM_WALL 100

IR frontLeftIR(LEFT_FRONT);
IR frontRightIR(RIGHT_FRONT);
IR sideLeftIR(LEFT_SIDE);
IR sideRightIR(RIGHT_SIDE);

enum MovementMode
{
    DRIVE_TO_FINISH,
    FOLLOW_OBSTACLE,
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

    IROutputList[0] = frontSum / AVERAGING_RESOLUTION;
    IROutputList[1] = leftSum / AVERAGING_RESOLUTION;
    IROutputList[2] = rightSum / AVERAGING_RESOLUTION;
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
    for (int i = 0; i < 2; i++)
    {
        motorControl.setAlign(true);
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
            motorControl.rotate(theta);
        }
        else if (left > right)
        {
            theta = (180 / PI) * atan((left - right) / DIST_BETWEEN_FRONT_IR);
            motorControl.rotate(-theta);
        }
    }

    IRAveraging();
    float frontDist = IROutputList[0];

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
    float frontDist = IROutputList[0];
    // if room to move forward
    if (frontDist > MAX_DIST_FROM_WALL)
    {
        // move until next obstacle
        motorControl.forwardDist((frontDist - MAX_DIST_FROM_WALL));
    }
    else
    {
        motorControl.reverseDist((MAX_DIST_FROM_WALL - frontDist));
        // align to obstacle
        wallAlign();
    }
}

void rotateToFinish()
{
    // rotate towards finish
    int angle = motorControl.getCurrentAngle();

    if(angle>180){
        motorControl.rotate(360-angle);
    }
    else{
        motorControl.rotate(-angle);
    }
}

void solveMaze()
{
    switch (movementMode)
    {
    case DRIVE_TO_FINISH:
    {
        rotateToFinish();
        moveToObstacle();
        movementMode = FOLLOW_OBSTACLE;
        break;
    }

    case FOLLOW_OBSTACLE:
    {
        IRAveraging();
        int angleToRotate;
        // if left is clear
        if (IROutputList[1] >= 150)
        {
            // turn left
            angleToRotate = 90;
            motorControl.rotate(90);
        }

        else if (IROutputList[2] >= 150)
        {
            // turn right
            angleToRotate = -90;
            motorControl.rotate(-90);
        }
        else
        {
            angleToRotate = 180;
            motorControl.rotate(180);
        }
        wait_us(300000);

        motorControl.forwardDist(150);
        wait_us(300000);
        rotateToFinish();
        IRAveraging();

        while (IROutputList[0] < 150)
        {
            motorControl.rotate(angleToRotate);
            wait_us(300000);
            motorControl.forwardDist(SHUFFLE_DISTANCE);
            rotateToFinish();
            IRAveraging();
            wallAlign();
            wait_us(300000);
        }
        motorControl.rotate(angleToRotate);
        wait_us(300000);
        motorControl.forwardDist(150);
        wait_us(300000);
        movementMode = DRIVE_TO_FINISH;

        break;
    }
    }
}

void IROutput()
{

    Serial.println("IR:");
    Serial.print("FRONT AVERAGE: ");
    Serial.println(IROutputList[0]);
    Serial.print("SIDE LEFT: ");
    Serial.println(IROutputList[1]);
    Serial.print("SIDE RIGHT: ");
    Serial.println(IROutputList[2]);
    Serial.print("FRONT LEFT: ");
    Serial.println(IROutputList[3]);
    Serial.print("FRONT RIGHT: ");
    Serial.print(IROutputList[4]);
}