/*
 * BluetoothAdapter.cpp
 *
 *  Created on: Jan 18, 2014
 *      Author: rodtoll
 */

#include "common.h"

#include "BluetoothAdapter.h"

BluetoothAdapter::BluetoothAdapter()
{
	_broadcastHandler = NULL;
	_deviceId = -1;
	_adapterState = BlueUnInitialized;
}

BluetoothAdapter::~BluetoothAdapter()
{
	Cleanup();
}

int BluetoothAdapter::GetDeviceId()
{
	return _deviceId;
}

BluetoothAdapter::AdapterState BluetoothAdapter::GetState()
{
	return _adapterState;
}

int BluetoothAdapter::Initialize(int deviceId)
{
	if(this->GetState() != BlueUnInitialized)
	{
		BOOST_LOG_TRIVIAL(error) << "Device is already open. Cannot initialize." << endl;
		return -2;
	}

	int result = hci_open_dev(deviceId);

	if(result < 0)
	{
		BOOST_LOG_TRIVIAL(error) << "Initialize failed on open device id" << endl;
		return -1;
	}
	else
	{
		_deviceId = result;
		SetState(BlueInitialized);
		return 0;
	}
}

int BluetoothAdapter::StartScan(HandleDeviceBroadcastMessage* broadcastHandler)
{
	if(this->GetState() != BlueInitialized)
	{
		BOOST_LOG_TRIVIAL(error) << "Cannot start scan unless adapter is in the initialized state" << endl;
		return -2;
	}

	if(broadcastHandler == NULL)
	{
		BOOST_LOG_TRIVIAL(error) << "Cannot do a scan without a callback" << endl;
		return -3;
	}

	int res;

	uint8_t own_type = 0x00;
	uint8_t scan_type = 0x01;
	uint8_t filter_policy = 0x00;
	uint16_t interval = htobs(0x0010);
	uint16_t window = htobs(0x0010);

	res = hci_le_set_scan_parameters(_deviceId, scan_type, interval, window,
						own_type, filter_policy, 1000);

	if(res == 0)
	{
		res = hci_le_set_scan_enable(_deviceId, 0x01, 0, 1000);
		if(res < 0)
		{
			BOOST_LOG_TRIVIAL(error) << "hci_le_set_scan_enable failed" << errno << endl;
		}
		else
		{
			_broadcastHandler = broadcastHandler;
			this->SetState(BlueScanning);
			ExecuteScan();
		}
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "hci_le_set_scan_parameters" << errno << endl;
		perror("hci_le_set_scan_parameters");
	}

	return res;
}

static volatile int signal_received = 0;

static void sigint_handler(int sig)
{
	signal_received = sig;
}

void BluetoothAdapter::ExecuteScan()
{
	unsigned char buf[HCI_MAX_EVENT_SIZE], *ptr;
	struct hci_filter nf, of;
	socklen_t olen;
	struct sigaction sa;
	int len;

	olen = sizeof(of);
	if (getsockopt(_deviceId, SOL_HCI, HCI_FILTER, &of, &olen) < 0) {
		BOOST_LOG_TRIVIAL(error) << "could not get socket options" << endl;
		return;
	}

	hci_filter_clear(&nf);
	hci_filter_set_ptype(HCI_EVENT_PKT, &nf);
	hci_filter_set_event(EVT_LE_META_EVENT, &nf);

	if (setsockopt(_deviceId, SOL_HCI, HCI_FILTER, &nf, sizeof(nf)) < 0) {
		BOOST_LOG_TRIVIAL(error) << "could not set socket options" << endl;
		return;
	}

	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_NOCLDSTOP;
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);

	BOOST_LOG_TRIVIAL(debug) << "starting scan loop" << endl;

	while (1) {
		evt_le_meta_event *meta;
		le_advertising_info *info;
		char addr[18];

		while ((len = read(_deviceId, buf, sizeof(buf))) < 0) {
			if (errno == EINTR && signal_received == SIGINT) {
				len = 0;
				BOOST_LOG_TRIVIAL(debug) << "exiting loop because of SIGINT" << endl;
				goto done;
			}

			if (errno == EAGAIN || errno == EINTR)
				continue;
			goto done;
		}

		ptr = buf + (1 + HCI_EVENT_HDR_SIZE);
		len -= (1 + HCI_EVENT_HDR_SIZE);

		meta = (evt_le_meta_event *) ptr;

		if (meta->subevent != 0x02)
		{
			BOOST_LOG_TRIVIAL(debug) << "Got an unexpected event so looping again" << endl;
			continue;
		}

		/* Ignoring multiple reports */
		info = (le_advertising_info *) (meta->data + 1);
		ba2str(&info->bdaddr, addr);

		(*_broadcastHandler)(addr,ptr,len);
	}

done:
	BOOST_LOG_TRIVIAL(debug) << "exiting scan loop" << endl;

	setsockopt(_deviceId, SOL_HCI, HCI_FILTER, &of, sizeof(of));

	if (len < 0)
		return;

	return;
}

void BluetoothAdapter::SetState(AdapterState adapterState)
{
	BOOST_LOG_TRIVIAL(debug) << "Bluetooth Adapter state transition from " << _adapterState << " to " << adapterState << endl;

	_adapterState = adapterState;
}

int BluetoothAdapter::StopScan()
{
	if(this->GetState() != BlueScanning)
	{
		BOOST_LOG_TRIVIAL(error) << "Cannot stop a scan because no scan is running" << endl;
		return -2;
	}

	int res = hci_le_set_scan_enable(_deviceId, 0x00, 0, 1000);

	if(res < 0)
	{
		BOOST_LOG_TRIVIAL(error) << "Disabling scan failed" << errno << endl;
		return -3;
	}
	else
	{
		this->SetState(BlueInitialized);
		BOOST_LOG_TRIVIAL(debug) << "Scan is disabled" << endl;
	}

	return 0;
}

void BluetoothAdapter::Cleanup()
{
	if(this->GetState() == BlueScanning)
	{
		StopScan();
	}
	if(this->GetState() == BlueInitialized)
	{
		hci_close_dev(_deviceId);
		this->SetState(BlueUnInitialized);
		_deviceId = -1;
	}
}


