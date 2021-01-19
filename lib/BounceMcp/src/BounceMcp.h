/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
  Main code by Thomas O Fredericks (tof@t-o-f.info)
  Previous contributions by Eric Lowry, Jim Schimpf and Tom Harkaway
  MCP edition by xppl
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * @todo Make BounceMcp more abstract. Split it from the hardware layer.
 * @body Remove deboucing code from BounceMcp and make a new Debounce class from that code. BounceMcp should extend Debounce. 
 */


#ifndef BounceMcp_h
#define BounceMcp_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Uncomment the following line for "LOCK-OUT" debounce method
//#define BOUNCE_LOCK_OUT

// Uncomment the following line for "BOUNCE_WITH_PROMPT_DETECTION" debounce method
//#define BOUNCE_WITH_PROMPT_DETECTION

#include <inttypes.h>
#include "MCPExtender.h"

/**
    @example bounce.ino
    Simple example of the Bounce library that switches the debug LED when a button is pressed.
*/

/**
    @example change.ino
    This example toggles the debug LED (pin 13) on or off when a button on pin 2 is pressed.
*/

/**
    @example bounce_multiple.ino
    Detect the falling edge of multiple buttons. Eight buttons with internal pullups. Toggles a LED when any button is pressed. Buttons on pins 2,3,4,5,6,7,8,9
*/

/**
    @example BounceMcpbuttons.ino
     Example of two instances of the Bounce class that switches the debug LED when either one of the two buttons is pressed.
 */



/**
     @brief  The Debouce class. Just the deboucing code separated from all harware.
*/
class Debouncer
{
 // Note : this is private as it migh change in the futur
private:
  static const uint8_t DEBOUNCED_STATE = 0b00000001;
  static const uint8_t UNSTABLE_STATE  = 0b00000010;
  static const uint8_t CHANGED_STATE   = 0b00000100;

private:
  inline void changeState();
  inline void setStateFlag(const uint8_t flag)    {state |= flag;}
  inline void unsetStateFlag(const uint8_t flag)  {state &= ~flag;}
  inline void toggleStateFlag(const uint8_t flag) {state ^= flag;}
  inline bool getStateFlag(const uint8_t flag)    {return((state & flag) != 0);}

public:
	/*!
    @brief  Create an instance of the Debounce class.

    @endcode
*/
	Debouncer();

	    /**
    @brief  Sets the debounce interval in milliseconds.
            
    @param    interval_millis
    		The interval time in milliseconds.
     
     */
	void interval(uint16_t interval_millis);

	/*!
    @brief   Updates the pin's state. 

    Because Bounce does not use interrupts, you have to "update" the object before reading its value and it has to be done as often as possible (that means to include it in your loop()). Only call update() once per loop().

    @return True if the pin changed state.
*/

	bool update();

    /**
     @brief Returns the pin's state (HIGH or LOW).

     @return HIGH or LOW.
     */
	bool read();

    /**
    @brief Returns true if pin signal transitions from high to low.
    */
	bool fell();

    /**
    @brief Returns true if pin signal transitions from low to high.
    */
	bool rose();



public:
    /**
     @brief Returns true if the state changed on last update.

     @return True if the state changed on last update. Otherwise, returns false.
*/
  bool changed( ) { return getStateFlag(CHANGED_STATE); }

      /**
     @brief Returns the duration in milliseconds of the current state. 

     Is reset to 0 once the pin rises ( rose() ) or falls ( fell() ).
    
      @return The duration in milliseconds (unsigned long) of the current state.
     */

  unsigned long duration();

  /**
     @brief Returns the duration in milliseconds of the previous state. 

     Takes the values of duration() once the pin changes state.
    
      @return The duration in milliseconds (unsigned long) of the previous state. 
     */
  unsigned long previousDuration();     

protected:
  void begin();
  virtual bool readCurrentState() =0;
  unsigned long previous_millis;
  uint16_t interval_millis;
  uint8_t state;
  unsigned long stateChangeLastTime;
  unsigned long durationOfPreviousState;

};


/**
@brief The Debouncer:Bounce class. Links the Deboucing class to a hardware pin.
     
     */
class Bounce : public Debouncer
{


public:

/*!
    @brief  Create an instance of the Bounce class.

    @code

    // Create an instance of the Bounce class.
    Bounce() button;

    @endcode
*/
	Bounce(MCPExtender* mcp);


/*!
    @brief  Attach to a pin and sets that pin's mode (INPUT, INPUT_PULLUP or OUTPUT).
            
    @param    pin
              The pin that is to be debounced.
    @param    mode
              A valid Arduino pin mode (INPUT, INPUT_PULLUP or OUTPUT).
*/
	void attach(int pin, int mode);

    /**
    Attach to a pin for advanced users. Only attach the pin this way once you have previously set it up. Otherwise use attach(int pin, int mode).
    */
	void attach(int pin);

  Bounce(MCPExtender* mcp, uint8_t pin, unsigned long interval_millis ) : Bounce(mcp) {
    attach(pin);
    interval(interval_millis);
  }

  ////////////////
  // Deprecated //
  ////////////////

     /**
    @brief Deprecated (i.e. do not use). Included for partial compatibility for programs written with Bounce version 1
    */
	bool risingEdge() { return rose(); }
     /**
    @brief Deprecated (i.e. do not use). Included for partial compatibility for programs written with Bounce version 1
    */
	bool fallingEdge() { return fell(); }
     /**
    @brief Deprecated (i.e. do not use). Included for partial compatibility for programs written with Bounce version 1
    */


protected:
  MCPExtender* mcp;
	uint8_t pin;

	virtual bool readCurrentState() { return mcp->digitalRead(pin); }
	virtual void setPinMode(int pin, int mode) {
    if(mode == INPUT_PULLUP) mcp->pinMode(pin, INPUT, HIGH);
    else if(mode == INPUT) mcp->pinMode(pin, INPUT);
	}



};

/**
     @brief The Debouncer:Bounce:Button class. The Button class matches an electrical state to a physical action.
     */
class Button : public Bounce{
protected:
    bool stateForPressed = 1; // 
  public:
	/*!
    @brief  Create an instance of the Button class. By default, the pressed state is matched to a HIGH electrical level.

    @code

    // Create an instance of the Button class.
    Button() button;

    @endcode
*/
   Button(MCPExtender* mcp) : Bounce(mcp) { }

    /*!
    @brief Set the electrical state (HIGH/LOW) that corresponds to a physical press. By default, the pressed state is matched to a HIGH electrical level.
            
    @param    state
              The electrical state (HIGH/LOW) that corresponds to a physical press.

*/
   void setPressedState(bool state){
    stateForPressed = state;
  }

    /*!
    @brief Returns true if the button was physically pressed          
*/
  bool pressed() {
    return changed() && (read() == stateForPressed);
  };

        /*!
    @brief Returns true if the button was physically released          
*/
  bool released() {
    return  changed() && (read() != stateForPressed);
  };



};

#endif
