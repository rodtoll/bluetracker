/*
 * BluetoothDeviceFactory.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: rodtoll
 */

#include "common.h"
#include "BluetoothDeviceFactory.h"

int BluetoothLoadDeviceMap(string fileName, boost::container::map<string,BluetoothDevice* >& deviceMap)
{
	string deviceName;
	BluetoothDevice* newDevice;

	deviceName = "E6:18:EB:4A:FE:D1";
	newDevice = BluetoothDevice::CreateDevice(
			deviceName,
			string("Stick-Orange"),
			posix_time::seconds(4),
			posix_time::seconds(60),
			BluetoothDevice::BDT_SticknFindTag,
			20);

	deviceMap[deviceName] = newDevice;

	deviceName = "F0:B2:48:5A:8A:49";
	newDevice = BluetoothDevice::CreateDevice(
			deviceName,
			string("Stick-Pink"),
			posix_time::seconds(4),
			posix_time::seconds(60),
			BluetoothDevice::BDT_SticknFindTag,
			21);

	deviceMap[deviceName] = newDevice;

	return 0;
}

