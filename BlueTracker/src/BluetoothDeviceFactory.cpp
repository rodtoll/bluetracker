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
	deviceFile.close();
	return 0;
}

