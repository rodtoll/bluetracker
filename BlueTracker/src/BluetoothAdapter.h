/*
 * BluetoothAdapter.h
 *
 *  Created on: Jan 18, 2014
 *      Author: rodtoll
 */

#ifndef BLUETOOTHADAPTER_H_
#define BLUETOOTHADAPTER_H_

typedef void HandleDeviceBroadcastMessage(string adapterName, string deviceAddress, void *data, int dataLength);

class BluetoothAdapter {
public:
	BluetoothAdapter();
	virtual ~BluetoothAdapter();

	int Initialize(int deviceId, string adapterName);
	int StartScan(HandleDeviceBroadcastMessage* broadcastHandler);
	int StopScan();
	void Cleanup();

	int GetDeviceId();

    enum AdapterState { BlueUnInitialized, BlueInitialized, BlueScanning };

	AdapterState GetState();
	string GetName() { return _adapterName; };

protected:

	void SetState(AdapterState adapterState);
	void DisplayError(const char *msg);
	void ExecuteScan();

	int _deviceId;
	AdapterState _adapterState;
	HandleDeviceBroadcastMessage* _broadcastHandler;
	string _adapterName;
};

#endif /* BLUETOOTHADAPTER_H_ */
