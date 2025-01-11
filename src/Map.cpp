#include "Map.h"
#include "mbed.h"
#include "arduino.h"
#include "Map.h"
#include "Adafruit_NeoPixel.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_GFX.h"
#include "MotionController.h"

int previousLocation[2];
int currentLocation[2];

const unsigned char PROGMEM bitmap[] ={

     0xff, 0xff, 0x9e, 0x19, 0x90, 0x11, 0x90, 0x71, 0x80, 0x1, 0x80, 0x1, 
  0x84, 0x1, 0x84, 0x1, 0x84, 0x1, 0x84, 0x1, 0x86, 0x71, 0x90, 0x41, 
  0x90, 0x41, 0x90, 0x41, 0x93, 0x41, 0xff, 0xff, 
};

using namespace mbed;

    Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, MAP_PIN,
                                                   NEO_TILE_BOTTOM + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_ZIGZAG +
                                                       NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                                                   NEO_BGR + NEO_KHZ800);

void mapSetup()
{
    matrix.begin();
    matrix.show();
    matrix.setTextWrap(false);
    matrix.setBrightness(10);
    matrix.setTextColor(matrix.Color(0, 0, 255));
    matrix.setCursor(0, 0);
    mapDrawBoundary();
    matrix.show();
}

void mapReset()
{
    matrix.fillScreen(OFF);
    mapDrawBoundary();
    previousLocation[0] = 0;
    previousLocation[1] = 0;
    currentLocation[0] = 0;
    currentLocation[1] = 0;
    matrix.show();
}

void mapOverwriteLocation(int newLocation[2])
{
    currentLocation[0] = newLocation[0];
    currentLocation[1] = newLocation[1];
    mapUpdate();
    
}

void mapUpdate(void) {
    
    matrix.drawPixel(previousLocation[0], previousLocation[1], OFF);
    matrix.drawPixel(currentLocation[0], currentLocation[1], BLUE);

    matrix.show();
}

void mapTranslation(int distance)
{
    
    int angle = motorControl.getCurrentAngle();
    if (angle==0){
        currentLocation[0]+=distance;
    }
    else if (angle==90){
        currentLocation[1]+=distance;
    }
    else if (angle==180){
        currentLocation[0]-=distance;
    }
    else if (angle==270){
        currentLocation[1]-=distance;
    }
    else{
       Serial.println("Needs aligning");
    }
    
    mapUpdate();
}

void mapDrawBoundary()
{
    matrix.drawRect(2,0,12,16,GREEN);
}
