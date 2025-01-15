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
#define FRONT_LEFT_IR 3
#define FRONT_RIGHT_IR 4
#define LEFT_IR 1
#define RIGHT_IR 2
#define PAUSE_MOVEMENT_DELAY 300000
#define ALIGNMENT_ATTEMPTS 2
#define MIN_ALIGNMENT_DIFFERENCE 2
#define DEAD_END_REVERSE_DIST 250
#define FINISH_X 2

int angleToRotate = 0;
bool canMoveLeft = false;
bool canMoveRight = false;
bool canMoveForward = true;
bool canMoveBackward = true;

// Build IR objects with all four addresses
IR frontLeftIR(LEFT_FRONT);
IR frontRightIR(RIGHT_FRONT);
IR sideLeftIR(LEFT_SIDE);
IR sideRightIR(RIGHT_SIDE);

// Different states for switch case finite state machine
enum MovementMode
{
    DRIVE_TO_FINISH,
    FOLLOW_OBSTACLE,
    SELECT_DIRECTION,
    STOP
};

MovementMode movementMode;

// List to store front and side sensors readings
float IROutputList[5];

void IRAveraging()
{
    // Define temparary variables to store data to be written to IR array
    float frontSum = 0;
    float leftSum = 0;
    float rightSum = 0;
    float frontLeftSum = 0;
    float frontRightSum = 0;
    float frontLeft = 0;
    float frontRight = 0;
    float frontToAdd = 0;
    float frontLeftToAdd = 0;
    float frontRightToAdd = 0;
    float leftToAdd = 0;
    float rightToAdd = 0;

    // Take as many readings as the resolution dictates
    for (int i = 0; i < AVERAGING_RESOLUTION; i++)
    {
        // Read both front sensors
        frontLeft = frontLeftIR.read();
        frontRight = frontRightIR.read();
        frontLeftToAdd = frontLeft;
        frontRightToAdd = frontRight;

        // If one sensor senses something close but the other senses something far, take the smaller reading
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
        // Average the front two sensors into one
        frontToAdd = (frontLeft + frontRight) / 2;
        // Read the side sensors
        leftToAdd = sideLeftIR.read();
        rightToAdd = sideRightIR.read();

        // Cap front sensor measurments within bounds
        frontToAdd = fmax(fmin(frontToAdd, MAX_IR_READING), MIN_IR_READING);
        frontSum += frontToAdd;

        frontLeftToAdd = fmax(fmin(frontLeftToAdd, MAX_IR_READING), MIN_IR_READING);
        frontLeftSum += frontLeftToAdd;

        frontRightToAdd = fmax(fmin(frontRightToAdd, MAX_IR_READING), MIN_IR_READING);
        frontRightSum += frontRightToAdd;

        leftToAdd = fmax(fmin(leftToAdd, MAX_IR_READING), MIN_IR_READING);
        leftSum += leftToAdd;

        rightToAdd = fmax(fmin(rightToAdd, MAX_IR_READING), MIN_IR_READING);
        rightSum += rightToAdd;

        wait_us(1000);
    }

    // Divide reading sums by the number of times looped
    IROutputList[FRONT_IR] = frontSum / AVERAGING_RESOLUTION;
    IROutputList[LEFT_IR] = leftSum / AVERAGING_RESOLUTION;
    IROutputList[RIGHT_IR] = rightSum / AVERAGING_RESOLUTION;
    IROutputList[FRONT_LEFT_IR] = frontLeftSum / AVERAGING_RESOLUTION;
    IROutputList[FRONT_RIGHT_IR] = frontRightSum / AVERAGING_RESOLUTION;
}

void mazeSolverSetup(void)
{
    // Set starting state to drive to the finish
    movementMode = DRIVE_TO_FINISH;
    // Set starting angle to be facing the finish
    motorControl.setCurrentAngle(0);
}

void wallAlign()
{
    // Take sensor measurements
    IRAveraging();
    float frontDist = IROutputList[FRONT_IR];

    // Move to a set distance from the wall infront
    if (frontDist > MAX_DIST_FROM_WALL)
    {
        motorControl.forwardDist((frontDist - MAX_DIST_FROM_WALL));
    }
    else
    {
        motorControl.reverseDist((MAX_DIST_FROM_WALL - frontDist));
    }

    // Set alignment flag high so that the angle of the robot is not changed when turning to align
    motorControl.setAlign(true);

    // Align multiple times to increase accuracy
    for (int i = 0; i < ALIGNMENT_ATTEMPTS; i++)
    {
        // Take sensore measurements
        IRAveraging();
        float left = IROutputList[FRONT_LEFT_IR];
        float right = IROutputList[FRONT_RIGHT_IR];
        float theta;

        // If sensors are almost identical, don't align
        if (abs(left - right) < MIN_ALIGNMENT_DIFFERENCE)
        {
        }

        else if (right > left)
        {
            // Calculate angle to turn and convert from radians to degrees
            theta = (180 / PI) * atan((right - left) / DIST_BETWEEN_FRONT_IR);
            // Rotate counterclockwise
            motorControl.rotate(-theta);
        }
        else if (left > right)
        {
            // Calculate angle to turn and convert from radians to degrees
            theta = (180 / PI) * atan((left - right) / DIST_BETWEEN_FRONT_IR);
            // Rotate clockwise
            motorControl.rotate(theta);
        }
    }
    // Set alignment flag low so any future turns will change the angle of the robot
    motorControl.setAlign(false);
}

void moveToObstacle()
{
    // Take sensor measurements
    IRAveraging();
    float frontDist = IROutputList[FRONT_IR];

    // If room to move forward
    if (frontDist > MAX_DIST_FROM_WALL)
    {
        // Move until next obstacle
        motorControl.forwardDist((frontDist - MAX_DIST_FROM_WALL));
    }
    // Save obstacle location to occupancy grid and draw it onto the matrix
    drawObstacle();
}

void rotateToFinish()
{
    int angle = motorControl.getCurrentAngle();

    // If facing right
    if (angle == 90)
    {
        // Turn left
        motorControl.rotate(-90);
    }
    // If facing left
    else if (angle == 270)
    {
        // Turn right
        motorControl.rotate(90);
    }
    // If facing away from finish
    else if (angle == 180)
    {
        // Turn 180
        motorControl.rotate(180);
    }
    // Do nothing if already facing finish
    else if (angle == 0)
    {
    }
    else
    {
        // angle should only ever be changed by 90
        errorScreen();
    }
}

void deadEndAvoid(void)
{
    // Turn away from dead end
    motorControl.rotate(180);

    // Take sensor measurements
    IRAveraging();
    int reverseDist = IROutputList[FRONT_IR];

    // Check if there is room to reverse
    if (reverseDist > SHUFFLE_DISTANCE)
    {
        // Turn back to face dead end
        motorControl.rotate(180);
        // Align to the wall infront
        wallAlign();
        // Reverse out of dead end
        motorControl.reverseDist(DEAD_END_REVERSE_DIST);
        // Take sensor measurementes
        IRAveraging();

        // Check  to see what directions are clear
        if (IROutputList[LEFT_IR] >= SHUFFLE_DISTANCE)
        {
            canMoveLeft = true;
        }
        else
        {
            canMoveLeft = false;
        }

        if (IROutputList[RIGHT_IR] >= SHUFFLE_DISTANCE)
        {
            canMoveRight = true;
        }
        else
        {
            canMoveRight = false;
        }

        // If there is only room to move left
        if ((canMoveLeft == true) & (canMoveRight == false))
        {
            // Go left
            angleToRotate = -90;
        }
        // If there is only room to move right
        else if ((canMoveLeft == false) & (canMoveRight == true))
        {
            // Go right
            angleToRotate = 90;
        }
        // If both directions are available
        else if ((canMoveLeft == true) & (canMoveRight == true))
        {
            // Choose the direction with the most room
            if (IROutputList[LEFT_IR] > IROutputList[RIGHT_IR])
            {
                angleToRotate = -90;
            }
            else
            {
                angleToRotate = 90;
            }
        }

        // Rotate to the decided direction
        motorControl.rotate(angleToRotate);
        // Take sensor measurements
        IRAveraging();
        // Move until a wall is reached
        motorControl.forwardDist(IROutputList[FRONT_IR] - MAX_DIST_FROM_WALL);
        // Attempt to drive to the finish
        movementMode = DRIVE_TO_FINISH;
    }
    // Assume the robot is fully enclosed
    else
    {
        errorScreen();
        movementMode = STOP;
    }
    return;
}

void solveMaze()
{
    switch (movementMode)
    {
    // State for when the robot is at the finish or blocked in
    case STOP:
    {
        // Stop all motors
        motorControl.stop();
        // Print the occupancy grid to serial
        printGrid();
        break;
    }
    case DRIVE_TO_FINISH:
    {
        // Face the finish
        rotateToFinish();
        // Drive to the nearest obstacle
        moveToObstacle();
        // Wall align to the obstacle
        wallAlign();

        // If the robot is at the end of the map
        if (getCurrentX() <= FINISH_X)
        {
            // Set end state
            movementMode = STOP;
        }
        else
        {
            // Change state to decide which way to move in
            movementMode = SELECT_DIRECTION;
        }

        break;
    }

    case FOLLOW_OBSTACLE:
    {
        // Take sensor measurements
        IRAveraging();
        // While there is not room to move forward
        while (IROutputList[FRONT_IR] < MAX_DIST_FROM_WALL)
        {
            // Save location of obstacle
            drawObstacle();
            // Continue to head in the decided direction
            motorControl.rotate(angleToRotate);
            motorControl.forwardDist(SHUFFLE_DISTANCE);
            // Face the finish
            rotateToFinish();
            // Take sensor measurements
            IRAveraging();
            // Align to the obstacle
            wallAlign();
        }

        // If loop exited, drive forwards to the finish
        movementMode = DRIVE_TO_FINISH;
        break;
    }
    case SELECT_DIRECTION:
    {
        // Rotate to face the finish
        rotateToFinish();
        // Take sensor measurements
        IRAveraging();

        // Check to see what directions are clear
        if (IROutputList[FRONT_IR] >= MAX_DIST_FROM_WALL)
        {
            canMoveForward = true;
        }
        else
        {
            canMoveForward = false;
        }

        if (IROutputList[LEFT_IR] >= SHUFFLE_DISTANCE)
        {
            canMoveLeft = true;
        }
        else
        {
            canMoveLeft = false;
        }

        if (IROutputList[RIGHT_IR] >= SHUFFLE_DISTANCE)
        {
            canMoveRight = true;
        }
        else
        {
            canMoveRight = false;
        }

        // If there is room to move forward
        if (canMoveForward == true)
        {
            // Switch state to move forward towards the finish
            movementMode = DRIVE_TO_FINISH;
            break;
        }
        // If there is only room to move left
        else if ((canMoveLeft == true) & (canMoveRight == false))
        {
            // Go left
            angleToRotate = -90;
        }
        // If there is only room to move right
        else if ((canMoveLeft == false) & (canMoveRight == true))
        {
            // Go right
            angleToRotate = 90;
        }
        // If there is room to move left and right
        else if ((canMoveLeft == true) & (canMoveRight == true))
        {
            // Move in the direction with the most space
            if (IROutputList[LEFT_IR] > IROutputList[RIGHT_IR])
            {
                angleToRotate = -90;
            }
            else
            {
                angleToRotate = 90;
            }
        }
        // If there isn't room to move left, right, or forwards
        else if ((canMoveLeft == false) & (canMoveRight == false))
        {
            // Run function to reverse and move out of dead end
            deadEndAvoid();
            break;
        }
        // If turning direction is decided, switch state to follow along the obstacle in that direction
        movementMode = FOLLOW_OBSTACLE;
        break;
    }
    }
}

// Debugging function to output IR data to serial
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
    Serial.println(IROutputList[FRONT_LEFT_IR]);
    Serial.print("FRONT RIGHT: ");
    Serial.println(IROutputList[FRONT_RIGHT_IR]);
}