/*
 * NCD16PWM.cpp
 *
 *  Created on: Sep 8, 2015
 *      Author: travis
 */
#include "NCD16PWM.h"

int address = 64;

NCD16PWM::NCD16PWM(){

}

void NCD16PWM::setAddress(int a0, int a1, int a2, int a3, int a4, int a5){
	int retrys = 0;
	if(a0 ==1){
		address = address | 1;
	}
	if(a1 ==1){
		address = address | 2;
	}
	if(a2 ==1){
		address = address | 4;
	}
	if(a3 ==1){
		address = address | 8;
	}
	if(a4 ==1){
		address = address | 16;
	}
	if(a5 ==1){
		address = address | 32;
	}
    //Start I2C port
	Serial.println("Wire.begin()");
    Wire.begin();
    //Open connection to specified address
    retryAddress:
    Serial.println("Wire.beginTransmission(address)");
    Wire.beginTransmission(address);
    Serial.println("Wire.endTransmission()");
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
            retrys++;
            goto retryAddress;
        }else{
        	Serial.println("Set Address Command failed");
            initialized = false;
            retrys = 0;
        }

    }else{
    	Serial.println("Set Address Command Successful");
    	Wire.beginTransmission(address);
    	Wire.write(254);
    	Wire.write(5);
    	Wire.endTransmission();
    	Wire.beginTransmission(address);
    	Wire.write(0);
    	Wire.write(161);
    	Wire.endTransmission();
    	initialized = true;
    }
}

void NCD16PWM::setChannelBrightness(int channel, int val){
	int reg = (channel*4)+2;

	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.write(0);
	Wire.write(0);
	Wire.write(val);
	Wire.write(val>>8);
	byte status = Wire.endTransmission();
	if(status != 0){
		Serial.println("Write failed");
	}else{
		//Command successful
	}
}

void NCD16PWM::setAllChannelsBrightness(int* vals){
	Wire.beginTransmission(address);
	Wire.write(6);
	for(int i = 0; i < 6; i++){
		Wire.write(0);
		Wire.write(0);
		Wire.write(vals[i]);
		Wire.write(vals[i]>>8);
	}
	Wire.endTransmission();

	Wire.beginTransmission(address);
	Wire.write(30);
	for(int i = 6; i < 11; i++){
		Wire.write(0);
		Wire.write(0);
		Wire.write(vals[i]);
		Wire.write(vals[i]>>8);
	}
	Wire.endTransmission();

	Wire.beginTransmission(address);
	Wire.write(50);
	for(int i = 11; i < 16; i++){
		Wire.write(0);
		Wire.write(0);
		Wire.write(vals[i]);
		Wire.write(vals[i]>>8);
	}
	Wire.endTransmission();

}

void NCD16PWM::setAllChannelsBrightness(int val){

	int msb = val/256;
	int lsb = val-msb;

	Wire.beginTransmission(address);
	Wire.write(252);
	Wire.write(lsb);
	Wire.write(msb);
	Wire.endTransmission();

}

int NCD16PWM::readChannelBrightness(int channel){
	int reg = (channel*4)+4;

	Wire.beginTransmission(address);
	Wire.write(reg);
	byte status = Wire.endTransmission();
	if(status != 0){
		return 256;
	}
	Wire.requestFrom(address, 2);
	byte lsb = Wire.read();
	byte msb = Wire.read();
	int reading = (msb*256)+lsb;
	return reading;
}

int* NCD16PWM::readAllChannelsBrightness(int* buffer){
	//Read first 32 bytes
	Wire.beginTransmission(address);
	Wire.write(6);
	byte status = Wire.endTransmission();
	if(status != 0){
		buffer[0] = 256;
		return buffer;
	}
	Wire.requestFrom(address, 32);
	byte buf[64];

	for(int i = 0; i < 32; i++){
		buf[i] = Wire.read();
	}

	//Read last 32 bytes
	Wire.beginTransmission(address);
	Wire.write(38);
	status = Wire.endTransmission();
	if(status != 0){
		buffer[0] = 256;
		return buffer;
	}
	Wire.requestFrom(address, 32);

	for(int i = 32; i < 64; i++){
		buf[i] = Wire.read();
	}

	//Populate array
	for(int i = 0; i < 64; i+=4){
		int lsb = buf[i+2];
		int msb = buf[i+3];
		int reading = (msb*256)+lsb;
		if(i == 0){
			buffer[i] = reading;
		}else{
			buffer[i/4] = reading;
		}

	}

	return buffer;
}


