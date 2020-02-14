#pragma once
#ifndef DDEVENT_H
#define DDEVENT_H

#include <string>
#include <time.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <curl/curl.h>
#include <vector>
#include <map>
#ifndef RAPIDJSON_HAS_STDSTRING
#define RAPIDJSON_HAS_STDSTRING 1
#endif
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class DDEvent
{
public:
	enum class Priority {NORMAL, LOW, NOT_SET};
	enum class AlertType {
		DD_ERROR, //Had to name as DD_ERROR as ERROR is defined somewhere within the curl/curl.h header 
		WARNING, INFO, SUCCESS, NOT_SET};
	DDEvent(std::string title, std::string text);
	void setTitle(std::string title);
	void setText(std::string text);
	void setDateHappened(size_t dateHappened);
	void setPriority(Priority priority);
	void setHost(std::string host);
	void setAlertType(AlertType alertType);
	void setAggregationKey(std::string aggregationKey);
	void setTags(std::string tags);
	void setTags(std::vector<std::string> tags);
	void setTags(std::map<std::string, std::string> tags);
	std::string returnDDEventUDPString();
	void getDDEventAsJSONString(std::string *jsonString);

private:
	std::string title; //required
	std::string text; //required
	std::string dateHappened;
	Priority priority;
	std::string host;
	AlertType alertType;
	std::string aggregationKey;
	void stringReplace(std::string& source, std::string needle, std::string replace);
	std::string getPriorityStringFromEnum(Priority priority);
	std::string getAlertTypeStringFromEnum(AlertType alertType);
	std::string tags;
	std::vector<std::string> tagsList;
};

#endif

