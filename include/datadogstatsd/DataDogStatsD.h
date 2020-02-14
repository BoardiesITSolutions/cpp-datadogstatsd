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
#include "datadogstatsd/Helpers.h"
#include <time.h>
#include "datadogstatsd/DDEvent.h"
#include <thread>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <Ws2tcpip.h>
#include <inaddr.h>
#pragma comment (lib, "Ws2_32.lib")

#else
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

	void increment(const std::string& stats, int value=1);
	void increment(const std::string& stats, const std::string& tags, int value=1);
	void increment(const std::vector<std::string>& stats);
	void increment(const std::vector<std::string>& stats, const std::string& tags);

	void decrement(const std::string& stats);
	void decrement(const std::string& stats, const std::string& tags);
	void decrement(const std::vector<std::string>& stats);
	void decrement(const std::vector<std::string>& stats, const std::string& tags);

	void timing(const std::string& stats, float timeInSeconds);
	void timing(const std::string& stats, float timeInSeconds, const std::string& tags);

	void gauge(const std::string& stats, float value);
	void gauge(const std::string& stats, float value, const std::string& tags);

	void histogram(const std::string& stats, float value);
	void histogram(const std::string& stats, float value, const std::string& tags);

	void set(const std::string& stats, float value);
	void set(const std::string& stats, float value, const std::string& tags);

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
	void updateStats(const std::vector<std::string> stats, int delta = 1, float sampleRate = 1.0, const std::string& tags = "");
	void send(std::map<std::string, std::string> data, float sampleRate = 1.0, std::string tags = "");
	void flush(std::string& udp_message);
	void sendDDEventinthread(DDEvent ddEvent, void(*eventCallback)(bool result, std::string error));
	static size_t curlResponseWriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
	std::thread *http_event_thread = NULL;
	CURL *initCurl(DDEvent ddEvent, std::string *response, struct curl_slist *list, const char* jsonString);
	bool httpEventThreadStarted = false;
	const std::string dd_entity_id_key = "dd.internal.entity_id";
	const char * dd_entity_id;
};

#endif
