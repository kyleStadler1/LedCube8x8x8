

//typedef uint8_t (*animationFunction_t) (uint8_t x, uint8_t y, uint8_t z, uint8_t frame);
//typedef uint8_t (*animationSetup_t) (char **animationName, animationFunction_t *animationFunction, unsigned long *frameTimeMs);


uint8_t createSmallCube (uint8_t xOrg, uint8_t yOrg, uint8_t zOrg, uint8_t x, uint8_t y, uint8_t z)
  { uint8_t value = 0;
    if ((xOrg == x || xOrg+1 == x) && (yOrg == y || yOrg+1 == y) && (zOrg == z || zOrg+1 == z)){
      value = 1;
    }
    return value;
  }


uint8_t animationFunctionCubeBounce (uint8_t x, uint8_t y, uint8_t z, uint8_t frame)
{ 
//  uint8_t randX = (random (0,8));
//  uint8_t randY = (random (0,8));
//  uint8_t randZ = (random (0,8));
//  uint8_t xOrgArray[] = {0,0,0,1,2,2,2,2,2,2,2,1,0,0,0,0};
//  uint8_t yOrgArray[] = {0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,1};
//  uint8_t zOrgArray[] = {2,1,0,0,0,1,2,2,2,1,0,0,0,1,2,2};

  //uint8_t 
  //return (createSmallCube( xOrgArray[frame],yOrgArray[frame],zOrgArray[frame],x,y,z));



  //return (createSmallCube( randX , randY ,randZ ,x,y,z));
}

uint8_t animationSetupCubeBounce (char **animationName, animationFunction_t *animationFunction, unsigned long *frameTimeMs)
{
  *animationName = __FUNCTION__;
  *animationFunction = animationFunctionCubeBounce;
  *frameTimeMs = 500;
  return 60;
}





uint8_t planeTilt (uint8_t x, uint8_t y, uint8_t z, uint8_t frame, uint8_t xHinge, uint8_t yHinge, uint8_t zHinge, float slope) 
{ 
  uint8_t value = 0;
  if (frame >= 11){
        if (z == 0){
          value = 1;
        }
      }
    if (xHinge == 0 && frame < 11){
      if(x == int (.2+(slope*z))){
        value = 1;
      }
    }
    if (xHinge == 7 && frame < 11){
      if(x == (7+int (.2+(slope*z*(-1))))){
        value = 1;
      }
    }
    if (yHinge == 0 && frame < 11){
      if(y == int (.2+(slope*z))){
        value = 1;
      }
    }
    if (yHinge == 7 && frame < 11){
      if(y == (7+int (.2+(slope*z*(-1))))){
        value = 1;
      }
    }
    

  return value;
}

uint8_t animationFunctionPlanesFalling (uint8_t x, uint8_t y, uint8_t z, uint8_t frame)
{ uint8_t frameArray[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  float slopeArray[] = {0,0.01, 0.4, 0.6, 0.8, 1, 1.2,1.4, 1.6 ,2,7,9,12,15,17,19};
  uint8_t hingeOrginX[] = {0,9,7,9};
  uint8_t hingeOrginY[] = {9,0,9,7};
  return planeTilt(x,y,z,frameArray[frame%15],hingeOrginX[frame/15],hingeOrginY[frame/15],0,slopeArray[frameArray[frame%15]]);
}



uint8_t animationSetupCubePlanesFalling(char **animationName, animationFunction_t *animationFunction, unsigned long *frameTimeMs)
{
  *animationName = __FUNCTION__;
  *animationFunction = animationFunctionPlanesFalling;
  *frameTimeMs = 32;
  return 60;
}


uint8_t animationFunctionRain (uint8_t x, uint8_t y, uint8_t z, uint8_t frame)
{
  //Serial.print(actualFrame);
  static uint8_t drops[8][8] = {0};
  static uint8_t prevFrame = (uint8_t)~0;
  static uint8_t ztrigger;
  uint8_t probability;
  uint8_t zbyte;
  probability = (76 - (frame - 8))/12;
  if (frame != prevFrame) {
    if (!frame) {
            ztrigger = z;  // discover first z in frame
            //Serial.print("\n");
    }
    prevFrame = frame;
  }
  if (z == ztrigger) {
    zbyte = frame ? drops[x][y] : 0; // if frame = 0 u get . 0 but if frame not 0 then u get drops x,y
    uint8_t newDrop = (random(0,101) % 100) < probability;  // 20% chance of new drop
    zbyte = (newDrop << 7) | (zbyte >> 1);    // new drops start at the top
    drops[x][y] = zbyte;
  } else {
    zbyte = drops[x][y];
  }
  return (zbyte >> z) & 1;
}

uint8_t animationSetupCubeRain (char **animationName, animationFunction_t *animationFunction, unsigned long *frameTimeMs)
{
  *animationName = __FUNCTION__;
  *animationFunction = animationFunctionRain;
  *frameTimeMs = 64;
  return 80;
}

uint8_t animationFunctionPlanesCrossing (uint8_t x, uint8_t y, uint8_t z, uint8_t frame)
{ 
  uint8_t frameSequence[] = {0,1,2,3,4,5,6,7,6,5,4,3,2,1};
  uint8_t value = 0;
  if (x == frameSequence[frame%13]){
    value = 1;
  }
 if (y == frameSequence[frame%13]){
  value = 1;
 }
 return value;

}

uint8_t animationSetupCubePlanesCrossing (char **animationName, animationFunction_t *animationFunction, unsigned long *frameTimeMs)
{
  *animationName = __FUNCTION__;
  *animationFunction = animationFunctionPlanesCrossing;
  *frameTimeMs = 45;
  return 13;
}

////

uint8_t growCube (uint8_t startX, uint8_t startY, uint8_t startZ, uint8_t x, uint8_t y, uint8_t z, uint8_t sizeOfCube)
  {
    uint8_t value = 0; //make the cube look like a cube and startx+ sizeofcube basically = the old currx
    int8_t xMulti;
    int8_t yMulti;
    int8_t zMulti;
    if (startX == 7){
      xMulti = -1;
    } else {
      xMulti = 1;
    }
    if (startY == 7){
      yMulti = -1;
    } else {
      yMulti = 1;
    }
    if (startZ == 7){
      zMulti = -1;
    } else {
      zMulti = 1;
    }
    if (startX == x && startY == y && startZ == z){
      value = 1;
    }

    if (startX + sizeOfCube * xMulti == x || startX == x){
      if (startY + sizeOfCube * yMulti == y || startY == y){
        value = 1;
      }
      if (startZ + sizeOfCube * zMulti == z || startZ == z){
        value = 1;
      }
    }
    if (startY + sizeOfCube * yMulti == y || startY == y){
      if(startZ + sizeOfCube * zMulti == z || startZ == z){
        value = 1;
      }
    }

    if (startX == 0){
      if (x > (startX + sizeOfCube * xMulti)){
        value = 0;
      }
    }
    if (startX == 7){
      if (x < (startX + sizeOfCube * xMulti)){
        value = 0;
      }
    }
    if (startY == 0){
      if (y > (startY + sizeOfCube * yMulti)){
        value = 0;
      }
    }
    if (startY == 7){
      if (y < (startY + sizeOfCube * yMulti)){
        value = 0;
      }
    }
    if (startZ == 0){
      if (z > (startZ + sizeOfCube * zMulti)){
        value = 0;
      }
    }
    if (startZ== 7){
      if (z < (startZ + sizeOfCube * zMulti)){
        value = 0;
      }
    }
    return value;
  }
static uint8_t animationFunctionCubePulseToCorners (uint8_t x, uint8_t y, uint8_t z, uint8_t frame)
{
  uint8_t sizeOfCubeArray[] = {0,1,2,3,4,5,6,7,6,5,4,3,2,1, 0};   //try repeating the 0
  uint8_t xStartList[] = {0,7,7,0};
  uint8_t yStartList[] = {0,7,0,7};
  uint8_t zStartList[] = {0,7,0,7};
  uint8_t orderList[] = {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0};
  //uint8_t counter;
//  if (frame <= 6){
//    counter = 0;
//  }else{
//    counter = (((frame-6)/14)%7);
//  }
  uint8_t counter = (orderList[frame]);

    
  
  return growCube(xStartList[counter],yStartList[counter],zStartList[counter],x,y,z,sizeOfCubeArray[frame%14]);
}
  

static uint8_t animationSetupCubePulseToCorners (char **animationName, animationFunction_t *animationFunction, unsigned long *frameTimeMs)
{
  *animationName = __FUNCTION__;
  *animationFunction = animationFunctionCubePulseToCorners;
  *frameTimeMs = 32;
  return 57;
}



///

static uint8_t animationFunctionCubePulse(uint8_t x, uint8_t y, uint8_t z, uint8_t frame)
{
  uint8_t framePattern[] = {0, 1, 2, 3, 2, 1};
  frame = framePattern [frame%6];
  uint8_t extremeBig = 7-frame%4;
  uint8_t extremeSmall = frame%4;
  uint8_t value = 0;
  if (x==extremeSmall || (x==extremeBig)) {
    if (y==extremeSmall || (y==extremeBig)) {
       value = 1;
    }
    if (z==extremeSmall || (z==extremeBig)) {
       value = 1;
     }
   }
   if (y==extremeSmall || (y==extremeBig)) {
     if (z==extremeSmall || (z==extremeBig)) {
       value = 1;
     }
   }
   if (x < extremeSmall || x > extremeBig) {
     value = 0;
   }
   if (y < extremeSmall || y > extremeBig) {
     value = 0;
   }
   if (z < extremeSmall || z > extremeBig) {
     value = 0;
   }



   
   return value;
}

static uint8_t animationSetupCubePulse(char **animationName, animationFunction_t *animationFunction, unsigned long *frameTimeMs)
{
  *animationName = __FUNCTION__;
  *animationFunction = animationFunctionCubePulse;
  *frameTimeMs = 64;
  return 6;
}

/////
