/*
 * StickNFindDevice.h
 *
 *  Created on: Jan 19, 2014
 *      Author: rodtoll
 */

#ifndef STICKNFINDDEVICE_H_
#define STICKNFINDDEVICE_H_

#include "BluetoothDevice.h"

class StickNFindDevice: public BluetoothDevice
{
public:
	StickNFindDevice();
	virtual ~StickNFindDevice();

	virtual const char *GetDeviceTypeName();
};

#endif /* STICKNFINDDEVICE_H_ */
