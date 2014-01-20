/*
 * ISYDevice.h
 *
 *  Created on: Jan 19, 2014
 *      Author: rodtoll
 */

#ifndef ISYDEVICE_H_
#define ISYDEVICE_H_

#include <curl/curl.h>

class ISYDevice
{
public:
	ISYDevice();
	virtual ~ISYDevice();

	int Initialize(string isyAddress, string userName, string password);
	int SetVariable(int variableType, int variableId, int variableValue);
protected:
	string _isyAddress;
	string _userName;
	string _password;
	CURL* _curlHandle;
};

#endif /* ISYDEVICE_H_ */
