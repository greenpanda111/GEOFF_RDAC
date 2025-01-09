#include "mbed.h"
#include "MotorController.h"
#include "IR.h"

IR frontLeftIR(LEFT_FRONT);
IR frontRightIR(RIGHT_FRONT);
IR sideLeftIR(LEFT_SIDE);
IR sideRightIR(RIGHT_SIDE);

int IROutputList[4];

void IRRead()
{
    IROutputList[0] = frontLeftIR.read()*10;
    IROutputList[1] = frontRightIR.read()*10;
    IROutputList[2] = sideLeftIR.read()*10;
    IROutputList[3] = sideRightIR.read()*10;
}

void solveMaze()
{
    IRRead();
    // if pointing towards the finish
    if (motorControl.getCurrentAngle() == 0)
    {
        // if nothing immediately infront of robot
        if ((int)IROutputList[2] > 100)
        {
            Serial.println("no obstacle detected");
            // move towards finish
            motorControl.forwardDist(IROutputList[2] - 40);
        }
        // if there is an obstacle
        else if (IROutputList[1]>40)
        {
            // turn away from obstacle
            Serial.print("rotating away from obstacle");
            motorControl.rotate(-90);
        }else{
            Serial.print("rotating away from obstacle");
            motorControl.rotate(90);    
        }
    }
    // if not pointing towards finish and there is no obstacle
    else if (IROutputList[2] > 100)
    {
        // move forward
        motorControl.forwardDist(IROutputList[0] - 40);

        // rotate to face finish
        motorControl.rotate(-90);
    }
}



void IROutput()
{

    Serial.print("IR:");
    for (int i = 0; i < 4; i++)
    {
        Serial.print(IROutputList[i]);
        if (i < 3)
        {
            Serial.print(",");
        }
    }
    Serial.println("");
}