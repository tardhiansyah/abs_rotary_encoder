#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <Arduino.h>

#define STILL 0
#define ROTATE_CW 1
#define ROTATE_CCW 2

class AbsoluteRotaryEncoder
{
  public:
    /**
     * @brief Construct a new Absolute Rotary Encoder object.
     * 
     * @param resolution in binary digit (e.g 8 bit or 10 bit).
     */
    AbsoluteRotaryEncoder(const byte resolution);

    /**
     * @brief Destroy the Absolute Rotary Encoder object.
     * 
     */
    ~AbsoluteRotaryEncoder() {};

    /**
     * @brief Initialize the rotary encoder.
     *        Multi turn mode (true): Keep track number of full rotation.
     *        Single turn mode (false): Reset the position to zero after one full rotation. (default)
     * @param gpio binary code pins.
     * @param multiTurn set mode to multi turn or single turn.
     */
    void init(const byte *gpio, bool multiTurn = false);

    /**
     * @brief Get the number of rotation of rotary encoder.
     * 
     * @return int
     */
    int getNumberOfRotation();

    /**
     * @brief Set the current number of rotation of rotary encoder.
     *        You can call this to set number of rotation based on.
     *        the last/prev number of rotation prior to system restart (e.g from EEPROM).
     *        By default this library doesn't automatically save the number of rotation, so you must save it manually.
     * @param n number of rotation
     */
    void setNumberOfRotation(int n);

    /**
     * @brief Put this function inside  an interrupt funtion
     * 
     */
    void readBinISR();

    /**
     * @brief Get the position reading of the rotary encoder
     * 
     * @return int position
     */
    int getPosition();

    /**
     * @brief Get the angle reading of the rotary encoder
     * 
     * @return float degree (0 - 360)
     */
    float getDegree();

    /**
     * @brief Convert encoder position to angle (0 - 360 degree)
     * 
     * @param position
     * @return float degree (0 - 360)
     */
    float posToDeg(int& position);

    /**
     * @brief Check for the encoder zero position
     * 
     * @return true
     * @return false 
     */
    bool isZero();

    /**
     * @brief Put this function inside void loop of the main file
     *        It's for keep track number of rotations
     * 
     */
    void loop();

    /**
     * @brief Get the rotate direction of the rotary encoder
     * 
     * @return int 1: CW
     * @return int 2: CCW
     */
    int getRotateDirection();

  private:
    /**
     * @brief Convert binary digit to encoder position
     * 
     * @return int position
     */
    int binToPos();

    byte _resolution;
    byte _gpio[32];
    bool _bin[32];
    bool _isMultiTurn;
    int _nPos;
    int _lastPos;
    int _nRotation;
    int _rotateDirection;
};

#endif