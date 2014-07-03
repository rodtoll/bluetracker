/*
 * BluetoothDeviceFactory.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: rodtoll
 */

#include "common.h"
#include "BluetoothDeviceFactory.h"
#include <fstream>
#include <boost/algorithm/string.hpp>

typedef vector< string > split_vector_type;

int BluetoothLoadDeviceMap(string fileName, boost::container::map<string,BluetoothDevice* >& deviceMap, int &deviceId)
{
	string line;
	ifstream deviceFile(fileName.c_str());

	// Read header so ignore line
	getline(deviceFile,line);

	// Read in device id
	getline(deviceFile,line);
	deviceId = atoi(line.c_str());

	// Skip header comments
	getline(deviceFile,line);

	// Read in devices 1 by 1
	while(getline(deviceFile,line)) {
	    split_vector_type splitVec;
	    split( splitVec, line, is_any_of(","), token_compress_on );

	    BluetoothDevice* newDevice;

	    newDevice = BluetoothDevice::CreateDevice(
	    		splitVec[1],
	    		splitVec[0],
	    		posix_time::seconds(atoi(splitVec[2].c_str())),
	    		posix_time::seconds(atoi(splitVec[3].c_str())),
	    		posix_time::seconds(atoi(splitVec[4].c_str())),
	    		(BluetoothDevice::DeviceType) atoi(splitVec[5].c_str()),
	    		atoi(splitVec[6].c_str()),
	    		deviceId);

	    deviceMap[splitVec[1]] = newDevice;
	}
	return 0;
	/*
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

	deviceName = "00:07:80:71:E9:29";
	newDevice = BluetoothDevice::CreateDevice(
			deviceName,
			string("Radius iBeacon"),
			posix_time::seconds(3),
			posix_time::seconds(60),
			posix_time::hours(1),
			BluetoothDevice::BDT_IBeacon,
			26);

	deviceMap[deviceName] = newDevice;

	deviceName = "90:59:AF:0B:84:DE";
	newDevice = BluetoothDevice::CreateDevice(
			deviceName,
			string("SensorTag-Pink"),
			posix_time::seconds(3),
			posix_time::seconds(60),
			posix_time::seconds(10),
			BluetoothDevice::BDT_SensorTag,
			26);

	deviceMap[deviceName] = newDevice;*/

	return 0;
}

