/*
 * NCD16PWM.h
 *
 *  Created on: Sep 8, 2015
 *      Author: travis
 */

#include "spark_wiring_i2c.h"
#include "spark_wiring_usbserial.h"
#include "spark_wiring_constants.h"
#include "spark_wiring.h"

#ifndef NCD16PWM_H_
#define NCD16PWM_H_

class NCD16PWM{
public:
	NCD16PWM(void);

	void setAddress(int a0, int a1, int a2, int a3, int a4, int a5);

	void setChannelBrightness(int channel, int val);

	void setAllChannelsBrightness(int* vals);

	void setAllChannelsBrightness(int val);

	int readChannelBrightness(int channel);

	int* readAllChannelsBrightness(int* buffer);

private:
	bool initialized = false;
};



#endif /* NCD16PWM_H_ */
