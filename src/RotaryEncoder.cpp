#include <RotaryEncoder.h>

AbsoluteRotaryEncoder::AbsoluteRotaryEncoder(const byte resolution)
{   
  this->_resolution = resolution;
  this->_nPos = pow(2, _resolution);
  this->_lastPos = 0;
  this->_nRotation = 0;
  this->_rotateDirection = STILL;
}

int AbsoluteRotaryEncoder::binToPos()
{
  int pos = 0;
  for (uint8_t i = 0; i < this->_resolution; i++)
  {
    pos += (this->_bin[i]) * (1 << (this->_resolution - (i + 1))); 
  }
  return pos;
}

void AbsoluteRotaryEncoder::init(const byte *gpio, bool multiTurn)
{
  this->_isMultiTurn = multiTurn;

  for (uint8_t i = 0; i < this->_resolution; i++)
  {
    this->_gpio[i] = gpio[i];
    pinMode(this->_gpio[i], INPUT); 
    this->_bin[i] = 0;
  }

  readBinISR();
}

void AbsoluteRotaryEncoder::readBinISR()
{
  for (uint8_t j = 0; j < this->_resolution; j++)
  {
    this->_bin[j] = digitalRead(this->_gpio[j]);
  }
}

int AbsoluteRotaryEncoder::getPosition()
{
  if (this->_isMultiTurn)
  {
    return this->_nPos * this->_nRotation + binToPos();
  }
  
  return binToPos();
}

float AbsoluteRotaryEncoder::getDegree()
{ 
  return (float) getPosition() / this->_nPos * 360; 
}

float AbsoluteRotaryEncoder::posToDeg(int &position)
{
  return (float) position / this->_nPos * 360;
}

bool AbsoluteRotaryEncoder::isZero()
{
  return binToPos() == 0;
}

int AbsoluteRotaryEncoder::getRotateDirection()
{
  return this->_rotateDirection;
}

int AbsoluteRotaryEncoder::getNumberOfRotation()
{
  return this->_nRotation;
}

void AbsoluteRotaryEncoder::setNumberOfRotation(int n)
{
  this->_nRotation = n;
}

void AbsoluteRotaryEncoder::loop()
{
  int currentPos = getPosition();
  if (this->_lastPos != currentPos)
  {
    if (abs(this->_lastPos - currentPos) > ((int) (this->_nPos * 0.75)))
    {
      // Finished one full rotation
      if (this->_rotateDirection == ROTATE_CW)
      {
        this->_nRotation++;
      }
      else if (this->_rotateDirection == ROTATE_CCW)
      {
        this->_nRotation--;
      }
    }
    else
    {
      if (this->_lastPos < currentPos)
      {
        this->_rotateDirection = ROTATE_CW;
      }
      else if (this->_lastPos > currentPos)
      {
        this->_rotateDirection = ROTATE_CCW;
      }
    }
    this->_lastPos = currentPos;
  } 
}