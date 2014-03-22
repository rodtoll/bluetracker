/*
 * IBeaconDevice.h
 *
 *  Created on: Mar 22, 2014
 *      Author: rodtoll
 */

#ifndef IBEACONDEVICE_H_
#define IBEACONDEVICE_H_

#include "BluetoothDevice.h"

class IBeaconDevice: public BluetoothDevice
{
public:
	IBeaconDevice();
	virtual ~IBeaconDevice();

	virtual const char *GetDeviceTypeName();
};


#endif /* IBEACONDEVICE_H_ */
