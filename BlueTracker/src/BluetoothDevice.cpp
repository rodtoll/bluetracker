/*
 * BluetoothDevice.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: rodtoll
 */

#include "common.h"
#include "BluetoothDevice.h"
#include "StickNFindDevice.h"
#include "FitbitDevice.h"
#include "TISensorTagDevice.h"
#include "IBeaconDevice.h"

BluetoothDevice::BluetoothDevice(
		):_lastRssi(0),_devicePresent(false), _deviceType(BDT_Undefined), _isyVariableId(0)
{
}

BluetoothDevice::~BluetoothDevice()
{
}

BluetoothDevice::DeviceType BluetoothDevice::GetDeviceType()
{
	return _deviceType;
}

bool BluetoothDevice::TickAndCheckForStateChange()
{
	posix_time::ptime currentTime = boost::posix_time::microsec_clock::universal_time();
	posix_time::time_duration deltaFromLastUpdate = currentTime - this->GetLastSeen();

	if(this->IsDevicePresent())
	{
		if(deltaFromLastUpdate.total_seconds() > this->GetTimeBeforeMissingMs().total_seconds())
		{
			BOOST_LOG_TRIVIAL(debug) << "Changing from present to not because no update for " << deltaFromLastUpdate.total_seconds() << endl;
			this->_devicePresent = false;
			return true;
		}
	}
	else
	{
		if(deltaFromLastUpdate.total_seconds() < this->GetTimeBeforeMissingMs().total_seconds())
		{
			BOOST_LOG_TRIVIAL(debug) << "Changing from not present to present as update was received" << endl;
			this->_devicePresent = true;
			return true;
		}
	}
	return false;
}

void BluetoothDevice::UpdateDeviceDetected(int rssi)
{
	_lastSeenTime = boost::posix_time::microsec_clock::universal_time();
	this->SetLastRSSI(rssi);
}

bool BluetoothDevice::IsDevicePresent()
{
	return _devicePresent;
}

posix_time::ptime BluetoothDevice::GetLastSeen()
{
	return _lastSeenTime;
}

void BluetoothDevice::SetLastRSSI(int rssi)
{
	_lastRssi = rssi;
}

int BluetoothDevice::GetLastRSSI()
{
	return _lastRssi;
}

posix_time::time_duration BluetoothDevice::GetUpdateFrequencyMs()
{
	return _updateFrequencyMs;
}

posix_time::time_duration BluetoothDevice::GetTimeBeforeMissingMs()
{
	return _timeBeforeMissingMs;
}


const char *BluetoothDevice::GetFriendlyName()
{
	return _friendlyName.c_str();
}

const char *BluetoothDevice::GetDeviceAddress()
{
	return _deviceAddress.c_str();
}

int BluetoothDevice::GetISYVariableId()
{
	return _isyVariableId;
}

int BluetoothDevice::Initialize(string address, string friendlyName,
		posix_time::time_duration updateFrequencyMs,
		posix_time::time_duration timeBeforeMissingMs,
		posix_time::time_duration timeBeforeSensorPollMs,
		BluetoothDevice::DeviceType deviceType,
		int isyVariableId,
		int bleAdapterIndex)
{
	_deviceType = deviceType;
	_updateFrequencyMs = updateFrequencyMs;
	_timeBeforeMissingMs = timeBeforeMissingMs;
	_timeBeforeSensorPollMs = timeBeforeSensorPollMs;
	_deviceAddress = address;
	_friendlyName = friendlyName;
	_isyVariableId = isyVariableId;
	_bleAdapterIndex = bleAdapterIndex;
	return 0;
}


BluetoothDevice* BluetoothDevice::CreateDevice(
		string address, string friendlyName,
		posix_time::time_duration updateFrequencyMs,
		posix_time::time_duration timeBeforeMissingMs,
		posix_time::time_duration timeBeforeSensorPollMs,
		BluetoothDevice::DeviceType deviceType,
		int isyVariableId,
		int bleAdapterIndex)
{
	if(deviceType == BluetoothDevice::BDT_SticknFindTag)
	{
		StickNFindDevice* newDevice = new StickNFindDevice();
		newDevice->Initialize(address, friendlyName, updateFrequencyMs, timeBeforeMissingMs, timeBeforeSensorPollMs, deviceType, isyVariableId, bleAdapterIndex);
		return newDevice;
	}
	else if(deviceType == BluetoothDevice::BDT_FitBit)
	{
		FitbitDevice* newDevice = new FitbitDevice();
		newDevice->Initialize(address, friendlyName, updateFrequencyMs, timeBeforeMissingMs, timeBeforeSensorPollMs, deviceType, isyVariableId, bleAdapterIndex);
		return newDevice;
	}
	else if(deviceType == BluetoothDevice::BDT_SensorTag)
	{
		TISensorTagDevice* newDevice = new TISensorTagDevice(bleAdapterIndex);
		newDevice->Initialize(address, friendlyName, updateFrequencyMs, timeBeforeMissingMs, timeBeforeSensorPollMs, deviceType, isyVariableId, bleAdapterIndex);
		return newDevice;
	}
	else if(deviceType == BluetoothDevice::BDT_IBeacon)
	{
		IBeaconDevice* newDevice = new IBeaconDevice();
		newDevice->Initialize(address, friendlyName, updateFrequencyMs, timeBeforeMissingMs, timeBeforeSensorPollMs, deviceType, isyVariableId, bleAdapterIndex);
		return newDevice;
	}
	return NULL;
}

