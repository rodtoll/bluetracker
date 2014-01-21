/*
 * FitbitDevice.h
 *
 *  Created on: Jan 20, 2014
 *      Author: rodtoll
 */

#ifndef FITBITDEVICE_H_
#define FITBITDEVICE_H_

#include "BluetoothDevice.h"

class FitbitDevice: public BluetoothDevice {
public:
	FitbitDevice();
	virtual ~FitbitDevice();

	virtual const char *GetDeviceTypeName();
};

#endif /* FITBITDEVICE_H_ */
