/*
 * BluetoothDeviceFactory.h
 *
 *  Created on: Jan 19, 2014
 *      Author: rodtoll
 */

#ifndef BLUETOOTHDEVICEFACTORY_H_
#define BLUETOOTHDEVICEFACTORY_H_

#include "BluetoothDevice.h"

#include <boost/algorithm/string.hpp>
#include <boost/container/map.hpp>

using namespace boost;

int BluetoothLoadDeviceMap(string fileName, boost::container::map<string,BluetoothDevice*> &deviceMap, int &deviceId, string &adapterName);

#endif /* BLUETOOTHDEVICEFACTORY_H_ */
