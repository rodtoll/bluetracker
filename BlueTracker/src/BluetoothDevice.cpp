/*
 * BluetoothDevice.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: rodtoll
 */

#include "common.h"
#include "BluetoothDevice.h"
#include "StickNFindDevice.h"

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
			this->_devicePresent = false;
			return true;
		}
	}
	else
	{
		if(deltaFromLastUpdate.total_seconds() < this->GetTimeBeforeMissingMs().total_seconds())
		{
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
		BluetoothDevice::DeviceType deviceType,
		int isyVariableId)
{
	_deviceType = deviceType;
	_updateFrequencyMs = updateFrequencyMs;
	_timeBeforeMissingMs = timeBeforeMissingMs;
	_deviceAddress = address;
	_friendlyName = friendlyName;
	_isyVariableId = isyVariableId;
	return 0;
}


BluetoothDevice* BluetoothDevice::CreateDevice(
		string address, string friendlyName,
		posix_time::time_duration updateFrequencyMs,
		posix_time::time_duration timeBeforeMissingMs,
		BluetoothDevice::DeviceType deviceType,
		int isyVariableId)
{
	if(deviceType == BluetoothDevice::BDT_SticknFindTag)
	{
		StickNFindDevice* newDevice = new StickNFindDevice();
		newDevice->Initialize(address, friendlyName, updateFrequencyMs, timeBeforeMissingMs, deviceType, isyVariableId);
		return newDevice;
	}
	return NULL;
}

