/*
 * GattTool.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: rodtoll
 */

#include "common.h"
#include "GattTool.h"

GattTool::GattTool(): _toolState(UnInitialized)
{
	// TODO Auto-generated constructor stub

}

GattTool::~GattTool() {
	// TODO Auto-generated destructor stub
}

void GattTool::SetToolState(ToolState state)
{
	BOOST_LOG_TRIVIAL(debug) << "GattTool transitioning from state " << _toolState << " to " << state << endl;
	_toolState = state;
}

int GattTool::WaitForPrompt(string &resultText)
{
	BOOST_LOG_TRIVIAL(debug) << "Waiting for a prompt to appear" << endl;
	getline(_gattToolProcess, resultText, '>');
	BOOST_LOG_TRIVIAL(debug) << "Found prompt. Text: " << resultText << endl;
	return 0;
}

int GattTool::Initialize()
{
	if(_toolState != UnInitialized)
	{
		BOOST_LOG_TRIVIAL(error) << "Unable to initialize gatttool because it is already initialized" << endl;
		return -1;
	}

	string commandResult;

	// Setup so we are input/output
	const pstreams::pmode mode = pstreams::pstdout|pstreams::pstdin;

	// Start the gatttool environment
	_gattToolProcess.open("gatttool -i hci1 -I", mode);

	// Wait for a prompt
	return this->WaitForPrompt(commandResult);
}

int GattTool::ExecuteCommand(string &command, string &result, bool asyncCommand)
{
	int resultCode = 0;

	// Send command to the process
	_gattToolProcess << command << endl;

	if(asyncCommand)
	{
		string tmp;
		resultCode = this->WaitForPrompt(tmp);
	}

	if(resultCode == 0)
	{
		// Get the result of the command
		resultCode = this->WaitForPrompt(result);
	}
	return resultCode;
}


int GattTool::Connect(string deviceAddress)
{
	int resultCode = 0;
	string result;
	string connectCommand = "connect " + deviceAddress;

	resultCode = this->ExecuteCommand(connectCommand,result);

	if(resultCode == 0)
	{
		if(result.find("connect error") != string::npos)
		{
			BOOST_LOG_TRIVIAL(error) << "Error connecting to specified device" << endl;
			return -1;
		}
		else if(result.find("CON") != string::npos)
		{
			BOOST_LOG_TRIVIAL(debug) << "Connection succeeded" << endl;
			this->SetToolState(Connected);
			return 0;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "Unknown error. Output received: " << result << endl;
			return -2;
		}
	}
	return resultCode;
}

int GattTool::Disconnect()
{
	if(this->_toolState != Connected)
	{
		BOOST_LOG_TRIVIAL(error) << "Cannot disconnect when not connected" << endl;
		return -1;
	}
	string disconnectCommand = "disconnect";
	string tmp;
	return this->ExecuteCommand(disconnectCommand, tmp);
}

int GattTool::WriteValue(string handle, string value)
{
	if(this->_toolState != Connected)
	{
		BOOST_LOG_TRIVIAL(error) << "Cannot write a value when not connected" << endl;
		return -1;
	}

	string commandResult;
	string commandToExecute = "char-write-cmd " + handle + " " + value;
	return this->ExecuteCommand(commandToExecute, commandResult);
}

int GattTool::ReadValue(string handle, string& value)
{
	if(this->_toolState != Connected)
	{
		BOOST_LOG_TRIVIAL(error) << "Cannot read a value when not connected" << endl;
		return -1;
	}

	const string valuePrefix = "Characteristic value/descriptor: ";
	string commandResult;
	string commandToExecute = "char-read-hnd " + handle;

	int result = this->ExecuteCommand(commandToExecute, commandResult, true);

	if(result == 0)
	{
		unsigned int index = commandResult.find(valuePrefix);

		if( index == string::npos )
		{
			BOOST_LOG_TRIVIAL(error) << "Could not find the value" << endl;
			return -2;
		}

		int endIndex = commandResult.find("\n", index);

		if( index == string::npos )
		{
			BOOST_LOG_TRIVIAL(error) << "Could not find end of the descriptor" << endl;
			return -3;
		}

		int startOfText = index + valuePrefix.length();
		value = commandResult.substr(startOfText, endIndex-startOfText-1);
	}

	return result;
}

