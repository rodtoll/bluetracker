/*
 * TISensorTag.cpp
 *
 *  Created on: Jan 20, 2014
 *      Author: rodtoll
 */

#include "common.h"
#include "BluetoothDevice.h"
#include "TISensorTagDevice.h"

TISensorTagDevice::TISensorTagDevice() {
	// TODO Auto-generated constructor stub

}

TISensorTagDevice::~TISensorTagDevice() {
	// TODO Auto-generated destructor stub
}

const char *TISensorTagDevice::GetDeviceTypeName()
{
	return "TISENSORTAG";
}

bool TISensorTagDevice::TickAndCheckForStateChange()
{
	bool result = BluetoothDevice::TickAndCheckForStateChange();

	posix_time::ptime currentTime = boost::posix_time::microsec_clock::universal_time();
	posix_time::time_duration deltaFromLastUpdate = currentTime - _lastSensorCheck;
	_lastSensorCheck = currentTime;

	// Time to check the sensors!
	if(deltaFromLastUpdate.total_seconds() > _timeBeforeSensorPollMs.total_seconds())
	{
		BOOST_LOG_TRIVIAL(debug) << "Poll time exceeded. Time to get sensor data" << endl;
		RequestSensorUpdate();
	}
	return result;
}

void TISensorTagDevice::RequestSensorUpdate()
{

}


