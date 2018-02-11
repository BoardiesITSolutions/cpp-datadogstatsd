#pragma once

#ifndef DataDogStatsD_H
#define DataDogStatsD_H

#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <string.h>
#include "Helpers.h"
#include <time.h>
#include "DDEvent.h"
#include <thread>

#ifndef _WIN32
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#endif



class DataDogStatsD
{
public:
	DataDogStatsD();
	~DataDogStatsD();
	DataDogStatsD(std::string api_key, std::string app_key);
	DataDogStatsD(std::string host, unsigned int port);
	DataDogStatsD(std::string api_key, std::string app_key, std::string host);
	DataDogStatsD(std::string api_key, std::string app_key, unsigned int port);
	DataDogStatsD(std::string api_key, std::string app_key, std::string host, unsigned int port);

	void increment(std::string stats);
	void increment(std::string stats, std::string tags);
	void increment(std::vector<std::string> stats);
	void increment(std::vector<std::string> stats, std::string tags);

	void decrement(std::string stats);
	void decrement(std::string stats, std::string tags);
	void decrement(std::vector<std::string> stats);
	void decrement(std::vector<std::string> stats, std::string tags);
	
	void timing(std::string stats, float timeInSeconds);
	void timing(std::string stats, float timeInSeconds, std::string tags);

	void gauge(std::string stats, float value);
	void gauge(std::string stats, float value, std::string tags);

	void histogram(std::string stats, float value);
	void histogram(std::string stats, float value, std::string tags);

	void set(std::string stats, float value);
	void set(std::string stats, float value, std::string tags);
	
	std::string returnSerializedTagsString(std::string tags);
	std::string returnSerializedTagsString(std::vector<std::string> tags);
	std::string returnSerializedTagsString(std::map<std::string, std::string> tags);


	void event(DDEvent& ddEvent);
	bool event(DDEvent ddEvent, bool nonBlockingMode, void(*eventCallback)(bool result, std::string error) = nullptr);

	void(*eventCallback)(bool, std::string);
	
	long getTimeInMicroSeconds();
private:
	std::string api_key;
	std::string app_key;
	std::string host;
	unsigned int port;
	void updateStats(std::vector<std::string> stats, int delta = 1, float sampleRate = 1.0, std::string tags = "");
	void send(std::map<std::string, std::string> data, float sampleRate = 1.0, std::string tags = "");
	void flush(std::string& udp_message);
	void sendDDEventinthread(DDEvent ddEvent, void(*eventCallback)(bool result, std::string error));
	static size_t curlResponseWriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
	std::thread *http_event_thread = NULL;
	CURL *initCurl(DDEvent ddEvent, std::string *response, struct curl_slist *list, const char* jsonString);
	bool httpEventThreadStarted = false;
};

#endif
