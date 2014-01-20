/*
 * BluetoothDevice.h
 *
 *  Created on: Jan 19, 2014
 *      Author: rodtoll
 */

#ifndef BLUETOOTHDEVICE_H_
#define BLUETOOTHDEVICE_H_

class BluetoothDevice
{
public: // Types
	enum DeviceType
	{
		BDT_Undefined = 0,
		BDT_IBeacon = 1,
		BDT_SticknFindTag = 2,
		BDT_SensorTag = 3
	};

public:
	BluetoothDevice();
	virtual ~BluetoothDevice();

	virtual DeviceType GetDeviceType();
	virtual const char *GetDeviceTypeName() = 0;

	virtual bool TickAndCheckForStateChange();
	virtual void UpdateDeviceDetected(int rssi);

	virtual posix_time::ptime GetLastSeen();

	virtual bool IsDevicePresent();

	virtual void SetLastRSSI(int rssi);
	virtual int GetLastRSSI();
	virtual int GetISYVariableId();

	virtual const char *GetFriendlyName();
	virtual const char *GetDeviceAddress();

	virtual posix_time::time_duration GetUpdateFrequencyMs();
	virtual posix_time::time_duration GetTimeBeforeMissingMs();

	static BluetoothDevice* CreateDevice(
			string address, string friendlyName,
			posix_time::time_duration updateFrequencyMs,
			posix_time::time_duration timeBeforeAway,
			BluetoothDevice::DeviceType deviceType,
			int isyVariableId);

protected:
	virtual int Initialize(string address, string friendlyName,
			posix_time::time_duration updateFrequencyMs,
			posix_time::time_duration timeBeforeMissingMs,
			BluetoothDevice::DeviceType deviceType,
			int isyVariableId);

	posix_time::ptime _lastSeenTime;
	int _lastRssi;
	bool _devicePresent;
	posix_time::time_duration _updateFrequencyMs;
	posix_time::time_duration _timeBeforeMissingMs;
	DeviceType _deviceType;
	string _friendlyName;
	string _deviceAddress;
	int _isyVariableId;
};



#endif /* BLUETOOTHDEVICE_H_ */
