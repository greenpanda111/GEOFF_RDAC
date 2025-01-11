#include "mbed.h"
#include "MotorController.h"
#include "IR.h"
#include "Map.h"

#define DIST_BETWEEN_FRONT_IR 35

IR frontLeftIR(LEFT_FRONT);
IR frontRightIR(RIGHT_FRONT);
IR sideLeftIR(LEFT_SIDE);
IR sideRightIR(RIGHT_SIDE);

enum MovementMode
{
    STOP,
    DRIVE_TO_FINISH,
    FOLLOW_OBSTACLE,
    FACE_FINISH,
    WALL_ALIGN,
    DIST_FROMT_WALL
};

MovementMode movementMode;
// list to store front and side sensors
float IROutputList[3];
// list of both front sensors for wall alignment
float frontIRList[2];

void frontIRAveraging()
{
    float left = 0;
    float right = 0;

    for (int i = 0; i < 10; i++)
    {
        left += frontLeftIR.read();
        right += frontRightIR.read();
        wait_us(1000);
    }
    frontIRList[0] = left / 10;
    frontIRList[1] = right / 10;
}
void IRAveraging()
{
    float frontIRSum = 0;
    float leftIRSum = 0;
    float rightIRSum = 0;

    for (int i = 0; i < 10; i++)
    {
        frontIRSum += ((float)frontLeftIR.read() + (float)frontRightIR.read()) / 2;
        leftIRSum += sideLeftIR.read();
        rightIRSum += sideRightIR.read();
        wait_us(1000);
    }

    IROutputList[0] = frontIRSum / 10;
    IROutputList[1] = leftIRSum / 10;
    IROutputList[2] = rightIRSum / 10;
}

void mazeSolverSetup(void)
{
    movementMode = WALL_ALIGN;
    motorControl.setCurrentAngle(0);
}

void solveMaze()
{
    switch (movementMode)
    {
    case STOP:
    {
        motorControl.stop();
        return;
    }
    case FACE_FINISH:
    {
        // rotate towards finish
        int angle = motorControl.getCurrentAngle();
        // if facing left
        if (angle == 270)
        {
            // turn right
            motorControl.rotate(90);
        }
        // if facing right
        else if (angle == 90)
        {
            // turn left
            motorControl.rotate(-90);
        }
        else
        {
            motorControl.rotate(180);
        }
        movementMode = DRIVE_TO_FINISH;
        break;
    }
    case DRIVE_TO_FINISH:
    {
        // check front sensors
        IRAveraging();
        // if no obstacle infront
        if (IROutputList[0] > 7)
        {
            Serial.println("no obstacle detected");
            // move until next obstacle
            motorControl.forwardDist(10 * (IROutputList[0] - 7));
        }
        else
        {
            //align to obstacle
            movementMode = WALL_ALIGN;
        }
        break;
    }

    case WALL_ALIGN:
    {
        for (int i=0;i<3;i++){
        frontIRAveraging();
        float left = frontIRList[0];
        float right = frontIRList[1];
        float theta = 0;

        if (abs(left - right) < 2)
        {
        }
        else if (right > left)
        {
            theta = (180/PI) * atan((right - left) / DIST_BETWEEN_FRONT_IR);
            motorControl.rotate(theta);
        }
        else if (left > right)
        {
            theta = (180/PI) * atan((left - right) / DIST_BETWEEN_FRONT_IR);
            motorControl.rotate(-theta);
        }
        //wait_us(2000000);
        }
        //movementMode = DIST_FROMT_WALL;
        break;
    }

    case DIST_FROMT_WALL:
    {
        IRAveraging();
        float front = IROutputList[0];

        if (front > 70)
        {
            motorControl.forwardDist((front-70));
        }
        else
        {
            motorControl.reverseDist((70 - front));
        }
        movementMode = FOLLOW_OBSTACLE;
        break;
    }
    case FOLLOW_OBSTACLE:
    {
        IRAveraging();
        // if left is clear
        if (IROutputList[1] >= 100)
        {
            // turn left
            motorControl.rotate(-90);
        }

        else if (IROutputList[2] >= 100)
        {
            // turn right
            motorControl.rotate(90);
        }
        else
        {
            motorControl.rotate(180);
        }
        wait_us(300000);
        motorControl.forwardDist(200);
        wait_us(300000);
        movementMode = FACE_FINISH;
        break;
    }
    }
}

void IROutput()
{

    Serial.println("IR:");
    Serial.print("FRONT: ");
    Serial.println(IROutputList[0]);
    Serial.print("SIDE LEFT: ");
    Serial.println(IROutputList[1]);
    Serial.print("SIDE RIGHT: ");
    Serial.println(IROutputList[2]);
}

void frontIROutput(){
    Serial.print("Left: ");
    Serial.println(frontIRList[0]);
    Serial.print("Right: ");
    Serial.println(frontIRList[1]);
}