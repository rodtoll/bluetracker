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
			posix_time::seconds(120),
			BluetoothDevice::BDT_SticknFindTag,
			20);

	deviceMap[deviceName] = newDevice;

	deviceName = "F0:B2:48:5A:8A:49";
	newDevice = BluetoothDevice::CreateDevice(
			deviceName,
			string("Stick-Pink"),
			posix_time::seconds(4),
			posix_time::seconds(60),
			posix_time::seconds(120),
			BluetoothDevice::BDT_SticknFindTag,
			21);
	deviceMap[deviceName] = newDevice;

	deviceName = "D4:AD:89:B6:D5:C9";
	newDevice = BluetoothDevice::CreateDevice(
			deviceName,
			string("Rod Fitbit"),
			posix_time::seconds(3),
			posix_time::seconds(60),
			posix_time::hours(1),
			BluetoothDevice::BDT_FitBit,
			26);

	deviceMap[deviceName] = newDevice;

	deviceName = "90:59:AF:0B:84:DE";
	newDevice = BluetoothDevice::CreateDevice(
			deviceName,
			string("SensorTag-Pink"),
			posix_time::seconds(3),
			posix_time::seconds(60),
			posix_time::hours(1),
			BluetoothDevice::BDT_SensorTag,
			26);

	deviceMap[deviceName] = newDevice;

	return 0;
}

