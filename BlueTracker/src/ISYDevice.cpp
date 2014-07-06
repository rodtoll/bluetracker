/*
 * ISYDevice.cpp
 *
 *  Created on: Jan 19, 2014
 *      Author: rodtoll
 */

#include "common.h"
#include "ISYDevice.h"

ISYDevice::ISYDevice(): _isyAddress(""), _curlHandle(NULL)
{
	// TODO Auto-generated constructor stub

}

ISYDevice::~ISYDevice()
{
	if(_curlHandle != NULL)
	{
		curl_easy_cleanup(_curlHandle);
		_curlHandle = NULL;
	}
}

int ISYDevice::Initialize(string isyAddress, string userName, string password)
{
	_isyAddress = isyAddress;
	_userName = userName;
	_password = password;

	_curlHandle = curl_easy_init();

	if(_curlHandle == NULL)
	{
		BOOST_LOG_TRIVIAL(error) << "Unable to initialize the easy library" << endl;
		return -1;
	}
	else
	{
		return 0;
	}
}

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{
    for (size_t c = 0; c<size*nmemb; c++)
    {
        //cout << buf[c];
    }
    return size*nmemb; //tell curl how many bytes we handled
}

int ISYDevice::SetVariable(int variableType, int variableId, int variableValue)
{
	string urlToUse = "http://";
	urlToUse+= _isyAddress;
	urlToUse+= "/rest/vars/set/";
	urlToUse+= boost::lexical_cast<std::string>(variableType);
	urlToUse+= "/";
	urlToUse+= boost::lexical_cast<std::string>(variableId);
	urlToUse+= "/";
	urlToUse+= boost::lexical_cast<std::string>(variableValue);
	const char *urlToUseCStr = urlToUse.c_str();

	string userCredentials = _userName;
	userCredentials += ":";
	userCredentials += _password;
	const char *userCredentialsCStr = userCredentials.c_str();

	//BOOST_LOG_TRIVIAL(debug) << "Going to use URL: " << urlToUse << endl;
	//curl_easy_setopt(_curlHandle, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(_curlHandle, CURLOPT_URL, urlToUseCStr );
	curl_easy_setopt(_curlHandle, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(_curlHandle, CURLOPT_USERPWD, userCredentialsCStr );
	curl_easy_setopt(_curlHandle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

	curl_easy_perform(_curlHandle);

	return 0;
}


