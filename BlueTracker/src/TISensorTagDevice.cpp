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
	this->_sensorApp.Initialize();
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
	if(this->IsDevicePresent())
	{
		posix_time::ptime currentTime = boost::posix_time::microsec_clock::universal_time();
		posix_time::time_duration deltaFromLastUpdate = currentTime - _lastSensorCheck;
		_lastSensorCheck = currentTime;

		// Time to check the sensors!
		if(deltaFromLastUpdate.total_seconds() > _timeBeforeSensorPollMs.total_seconds())
		{
			BOOST_LOG_TRIVIAL(debug) << "Poll time exceeded. Time to get sensor data" << endl;
			RequestSensorUpdate();
		}
	}
	return false;
}

void TISensorTagDevice::UpdateDeviceDetected(int rssi)
{
	if(!this->IsDevicePresent())
	{
		this->_devicePresent = true;
		BOOST_LOG_TRIVIAL(debug) << "Device became present, connecting to it for sensor readings" << endl;
		int result = this->_sensorApp.Connect(this->_deviceAddress);
		if(result == 0)
		{
			BOOST_LOG_TRIVIAL(debug) << "Device connection established" << endl;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "Device failed to connect even though it broadcast. Weird" << endl;
		}
	}
}

void TISensorTagDevice::RequestSensorUpdate()
{
	bool readSuccesful = false;
	string resultText;
	int result = this->_sensorApp.WriteValue("0x29", "01");
	if(result == 0)
	{
		boost::this_thread::sleep_for(boost::chrono::seconds(3));
		result = this->_sensorApp.ReadValue("0x25", resultText);

		BOOST_LOG_TRIVIAL(debug) << "### SENSOR READING: [" + resultText + "]" << endl;
		if(result == 0)
		{
			result = this->_sensorApp.WriteValue("0x29", "00");
			if(result == 0)
			{
				BOOST_LOG_TRIVIAL(debug) << "### Sensor shutdown" << endl;
				readSuccesful = true;
			}
			else
			{
				BOOST_LOG_TRIVIAL(error) << "### Sensor shutdown error" << endl;
 			}
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "### Sensor read error" << endl;
		}
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "### Sensor write to enable failed" << endl;
	}

	if(!readSuccesful)
	{
		BOOST_LOG_TRIVIAL(error) << "### Sensor failed the sensor sequence. Disconnecting" << endl;
		this->_sensorApp.Disconnect();
		this->_devicePresent = false;
	}

}


