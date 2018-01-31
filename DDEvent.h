#pragma once
#ifndef DDEVENT_H
#define DDEVENT_H

#include <string>
#include <time.h>
#include <sstream>
#include <iostream>
#include <algorithm>

class DDEvent
{
public:
	enum class Priority {NORMAL, LOW, NOT_SET};
	enum class AlertType {ERROR, WARNING, INFO, SUCCESS, NOT_SET};
	DDEvent(std::string title, std::string text);
	void setTitle(std::string title);
	void setText(std::string text);
	void setDateHappened(size_t dateHappened);
	void setPriority(Priority priority);
	void setHost(std::string host);
	void setAlertType(AlertType alertType);
	void setAggregationKey(std::string aggregationKey);
	void setTags(std::string tags);
	std::string returnDDEventUDPString();

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
};

#endif

