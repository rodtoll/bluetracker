//============================================================================
// Name        : BlueTracker.cpp
// Author      : Rod Toll
// Version     :
// Copyright   : (C) 2014 Rod Toll, All Rights Reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "common.h"

#include "BluetoothAdapter.h"
#include "BluetoothDeviceFactory.h"
#include "ISYDevice.h"

boost::container::map<string,BluetoothDevice*> s_devicesList;

void HandleDeviceBroadcast(string deviceAddress, void *data, int dataLength)
{
	int8_t* rssi = (int8_t*) data;
	rssi += dataLength-1;
	int rssiAsInt = *rssi;

	if(s_devicesList.find(deviceAddress) == s_devicesList.end())
	{
		cout << deviceAddress << " - rssiAsInt - " << rssiAsInt << " -- Unknown device" << endl;
	}
	else
	{
		BluetoothDevice* device = s_devicesList[deviceAddress];
		device->UpdateDeviceDetected((int)(*rssi));
		cout << deviceAddress << " - rssiAsInt - " << rssiAsInt << " -- " << device->GetFriendlyName() << " -- " << device->GetDeviceAddress() << " -- " << device->GetDeviceTypeName() << endl;
	}
}

void worker(int foo)
{
	cout << "Launching worker argument=" << foo <<endl;
	BluetoothAdapter adapter;
	int res = adapter.Initialize(1);
	if(res == 0)
	{
		cout << "Adapter Initialized" << endl;
		res = adapter.StartScan(&HandleDeviceBroadcast);
		if( res == 0)
		{
			cout << "Scan started!" << endl;
			boost::this_thread::sleep_for(boost::chrono::seconds(10));
			adapter.StopScan();
		}
		else
		{
			cout << "Scan failed to start" << endl;
		}
	}
	else
	{
		cout << "Adapter Failed to Initialize" << endl;
	}
}

int main()
{
	ISYDevice isyDevice;
	if(isyDevice.Initialize("10.0.1.19", "admin", "ErgoFlat91") == 0)
	{
		cout << "ISY Initialize successful" << endl;
	}
	else
	{
		cout << "ISY Initialize failed" << endl;
		return -1;
	}

	int result;

	result = BluetoothLoadDeviceMap("devices.cfg",s_devicesList);

	if(result != 0)
	{
		cout << "Error loading device map" << endl;
		return -1;
	}

	cout << "Launching worker" << endl;
	boost::thread worker_thread(worker,3);

	cout << "Worker Launched" << endl;

	// Ensure upon startup we send the actual state
	bool sentInitialState = false;

	while(1)
	{
		if(worker_thread.try_join_for(boost::chrono::seconds(0)))
		{
			cout << "Worker is shutting down..." << endl;
			break;
		}

		boost::container::map<string,BluetoothDevice*>::iterator i;

		for(i = s_devicesList.begin(); i != s_devicesList.end(); i++)
		{
			cout << "Checking for state changes..." << endl;

			BluetoothDevice *device = i->second;

			cout << "Checking device: " << device->GetFriendlyName() << endl;

			if(device->TickAndCheckForStateChange() || !sentInitialState)
			{
				cout << "> State changed to" << device->IsDevicePresent() << endl;
				isyDevice.SetVariable(2, device->GetISYVariableId(), (device->IsDevicePresent()) ? 1 : 0 );
			}
			else
			{
				cout << "> No state change. Current state:" << device->IsDevicePresent() << endl;
			}
		}

		sentInitialState = true;

		boost::this_thread::sleep_for(boost::chrono::seconds(30));
	}

	worker_thread.join();
	cout << "Worker joined" << endl;
}
