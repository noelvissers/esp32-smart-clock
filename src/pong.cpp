#include <Arduino.h>
#include "pong.h"
#include "display.h"
#include "config.h"

unsigned int posBat = 5;      //1-6
unsigned int posBall_X = 7;   //0-15
unsigned int posBall_Y = 5;   //0-7
unsigned int speedBall = 250; //10-250
unsigned int speedBat = 50;   //delay in ms for bat to move
bool dirBall_X = 0;           //0 = Left, 1 = Right
bool dirBall_Y = 0;           //0 = Up, 1 = Down
bool wall = false;
long posWall = 2;
long lifeWall = 1;
unsigned int lastBallPosChange = 0;

unsigned long lastUp = 0;
unsigned long lastDown = 0;

void reset()
{
  posBat = 5;
  posBall_X = 7;
  posBall_Y = 5;
  speedBall = 250;
}

bool generateWall()
{
  long wallChance = random(0, 3); //1 in 3 to get a wall
  posWall = random(2, 7);
  lifeWall = random(1, 3);

  if (wallChance == 0)
    return true;
  else
    return false;
}

void CPong::start()
{
  Serial.println("[Pong] Starting...");

  for (;;)
  {
    //Check bat movement up
    if (_buttonPlusPressed && !_buttonMinPressed)
    {
      if ((millis() - lastUp) > speedBat)
      {
        posBat++;
        if (posBat > 6)
          posBat = 6;
        lastUp = millis();
      }
    }

    //Check bat movement down
    if (!_buttonPlusPressed && _buttonMinPressed)
    {
      if ((millis() - lastDown) > speedBat)
      {
        posBat--;
        if (posBat < 1)
          posBat = 1;
        lastDown = millis();
      }
    }

    //Check for exit button (select)
    if (_buttonSelectSet)
    {
      Serial.println("[Pong] Stopped...");
      _buttonSelectSet = false;
      break;
    }

    //handle bat bounce
    if (posBall_X == 14)
    {
      if (((posBall_Y - 1) == posBat) || (posBall_Y == posBat) || ((posBall_Y + 1) == posBat))
      {
        //Bounced
        dirBall_Y = !dirBall_Y;
        dirBall_X = 0;
        speedBall = speedBall - 10;
        if (speedBall < 10)
          speedBall = 10;
      }
    }

    //Check for loss
    if (posBall_X == 15)
    {
      //Ball is on the same line as bat, so you lost...
      //Play restart animation
      reset();
    }

    //Handle wall bounce
    if (wall && dirBall_X)
    {
      if ((posBall_X + 1) == posWall)
      {
        //Bounced
        dirBall_X = 0;
        dirBall_Y = !dirBall_Y;
        lifeWall--;
        if (lifeWall <= 0)
          wall = false;
      }
    }

    //Handle ball pos
    //X
    if ((millis() - lastBallPosChange) > speedBall)
    {
      lastBallPosChange = millis();

      if (posBall_X == 0)
      {
        dirBall_X = 1;
        dirBall_Y = !dirBall_Y;
      }

      if (dirBall_X) //RIGHT
        posBall_X++;
      else //LEFT
      {
        posBall_X--;
        if (posBall_X == 0 && !wall)
          wall = generateWall();
      }

      //Y
      if (posBall_Y == 0 || posBall_Y == 1)
        posBall_Y = !posBall_Y;
      if (dirBall_Y) //UP
        posBall_Y++;
      else //DOWN
        posBall_Y--;
    }

    //Draw pong
    //...
    //Display.printPong(posBat, posBall_X, posBall_Y, posWall, wall);

    delay(5);
  }
  reset();
}

