//==============================================================================
// KNOX-KNOX
// DEVELOPED FOR CYBER NINJAS, SARASOTA FLORIDA
// BY STUDENTS FROM THE UNIVERSITY OF SOUTH FLORIDA:
//    BRENDON BOLLES, STEPHEN DARIES, ANDREW KIVELA, ZACHARY STANDRIDGE
//==============================================================================

//Custom Wiegand Library
//This Library is a mash up of two libraries that are open source.
//1: https://github.com/jpliew/Wiegand-NG-Multi-Bit-Wiegand-Library-for-Arduino
//2: https://github.com/monkeyboard/Wiegand-Protocol-Library-for-Arduino
//Thanks to JP Liew and MonkeyBoard for the use of these libraries

#ifndef _WIEGAND_KNOX_H
#define _WIEGAND_KNOX_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class WiegandKnox {

public:
	WiegandKnox();
	void begin();
	void begin(int pinD0, int pinIntD0, int pinD1, int pinIntD1);
	bool available();
	void clear();
	void pause();
	unsigned long getCode();
	int getWiegandType();
	
private:
	static void ReadD0();
	static void ReadD1();
	static bool DoWiegandConversion ();
	static unsigned long GetCardId (volatile unsigned long *codehigh, volatile unsigned long *codelow, char bitlength);
	
	static volatile unsigned long 	_cardTempHigh;
	static volatile unsigned long 	_cardTemp;
	static volatile unsigned long 	_lastWiegand;
	static volatile int				_bitCount;	
	static int				_wiegandType;
	static unsigned long	_code;
};

#endif