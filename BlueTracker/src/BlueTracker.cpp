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
#include "GattTool.h"

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

void worker(int deviceIndex)
{
	cout << "Launching worker argument=" << deviceIndex <<endl;
	BluetoothAdapter adapter;
	int res = adapter.Initialize(deviceIndex);
	if(res == 0)
	{
		BOOST_LOG_TRIVIAL(debug) << "Adapter Initialized" << endl;
		res = adapter.StartScan(&HandleDeviceBroadcast);
		if( res == 0)
		{
			BOOST_LOG_TRIVIAL(debug) << "Scan started!" << endl;
			boost::this_thread::sleep_for(boost::chrono::seconds(10));
			adapter.StopScan();
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "Scan failed to start" << endl;
		}
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "Adapter Failed to Initialize" << endl;
	}
}

int main()
{
	ISYDevice isyDevice;
	if(isyDevice.Initialize("10.0.1.19", "admin", "ErgoFlat91") == 0)
	{
		BOOST_LOG_TRIVIAL(debug) << "ISY Initialize successful" << endl;
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "ISY Initialize failed" << endl;
		return -1;
	}

	boost::filesystem::path full_path( boost::filesystem::current_path() );
	std::cout << "Current path is : " << full_path << std::endl;

	int result;
	int deviceId = 0;

	result = BluetoothLoadDeviceMap("./devices.cfg",s_devicesList,deviceId);

	if(result != 0)
	{
		BOOST_LOG_TRIVIAL(error) << "Error loading device map" << endl;
		return -1;
	}

	BOOST_LOG_TRIVIAL(debug) << "Launching worker" << endl;
	boost::thread worker_thread(worker,deviceId);

	BOOST_LOG_TRIVIAL(debug) << "Worker Launched" << endl;

	// Ensure upon startup we send the actual state
	bool sentInitialState = false;

	while(1)
	{
		if(worker_thread.try_join_for(boost::chrono::seconds(0)))
		{
			BOOST_LOG_TRIVIAL(debug) << "Worker is shutting down..." << endl;
			break;
		}

		boost::container::map<string,BluetoothDevice*>::iterator i;

		for(i = s_devicesList.begin(); i != s_devicesList.end(); i++)
		{
			BOOST_LOG_TRIVIAL(debug) << "Checking for state changes..." << endl;

			BluetoothDevice *device = i->second;

			BOOST_LOG_TRIVIAL(debug) << "Checking device: " << device->GetFriendlyName() << endl;

			if(device->TickAndCheckForStateChange() || !sentInitialState)
			{
				BOOST_LOG_TRIVIAL(info) << "> State changed to" << device->IsDevicePresent() << endl;
				isyDevice.SetVariable(2, device->GetISYVariableId(), (device->IsDevicePresent()) ? 1 : 0 );
			}
			else
			{
				BOOST_LOG_TRIVIAL(debug) << "> No state change. Current state:" << device->IsDevicePresent() << endl;
			}
		}

		sentInitialState = true;

		boost::this_thread::sleep_for(boost::chrono::seconds(30));
	}
	BOOST_LOG_TRIVIAL(debug) << "Exiting worker - Waiting for join" << endl;
	worker_thread.join();
	BOOST_LOG_TRIVIAL(debug) << "Worker joined" << endl;
}
