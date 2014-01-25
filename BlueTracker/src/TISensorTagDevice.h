/*
 * TISensorTag.h
 *
 *  Created on: Jan 20, 2014
 *      Author: rodtoll
 */

#ifndef TISENSORTAG_H_
#define TISENSORTAG_H_

#include "BluetoothDevice.h"
#include "GattTool.h"

class TISensorTagDevice: public BluetoothDevice {
public:
	TISensorTagDevice();
	virtual ~TISensorTagDevice();

	virtual const char *GetDeviceTypeName();

	virtual bool TickAndCheckForStateChange();
	virtual void UpdateDeviceDetected(int rssi);

protected:
	void RequestSensorUpdate();

	posix_time::ptime _lastSensorCheck;
	GattTool _sensorApp;
};

#endif /* TISENSORTAG_H_ */
