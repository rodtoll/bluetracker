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

