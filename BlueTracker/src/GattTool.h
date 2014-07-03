/*
 * GattTool.h
 *
 *  Created on: Jan 21, 2014
 *      Author: rodtoll
 */

#ifndef GATTTOOL_H_
#define GATTTOOL_H_

#include <pstreams/pstream.h>

using namespace redi;

class GattTool {
public:
	enum ToolState
	{
		UnInitialized = 0,
		Initialized = 1,
		Connected = 2
	};

	GattTool();
	virtual ~GattTool();

	int Initialize(int bleAdapterIndex);
	int Connect(string deviceAddress);
	int Disconnect();
	int WriteValue(string handle, string value);
	int ReadValue(string handle, string& value);

	int ExecuteCommand(string &command, string &result, bool asyncCommand = false);

protected:
	int WaitForPrompt(string &resultText);
	void SetToolState(ToolState state);

	pstream _gattToolProcess;
	ToolState _toolState;
	string _deviceAddress;
	int _bleAdapterIndex;
};

#endif /* GATTTOOL_H_ */
