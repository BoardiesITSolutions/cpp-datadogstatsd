#include "DDEvent.h"

using namespace std;

DDEvent::DDEvent(string title, string text)
{
	this->title = title;
	this->text = text;

	//Set up the defaults
	priority = Priority::NOT_SET;
	alertType = AlertType::NOT_SET;
}

/// <summary>Update the title</summary>
/// <param name="title">The title text that should be sent to datadog (limited to 100 characters</param>
void DDEvent::setTitle(string title)
{
	this->title = title;
}

/// <summary>Update the event text</summary>
/// <param name="text">The event text that should be sent to datadog (limited to 4000 characters)</param>
void DDEvent::setText(string text)
{
	this->text = text;
}

/// <summary>Update the epoch time for the event (if this is not used then the current time is used at the point of the constructor instantiation</summary>
/// <param name="dateHappend">size_t representation of the epoch time</param>
void DDEvent::setDateHappened(size_t dateHappened)
{
	stringstream dateStream;
	dateStream << dateHappened;
	this->dateHappened = dateStream.str();
}

/// <summary>The priority of the event</summary>
/// <param name="priority">An enum of priority Low or Normal</param>
void DDEvent::setPriority(DDEvent::Priority priority)
{
	this->priority = priority;
}

/// <summary>Set the host string</summary>
/// <param name="host">The host name to send to datadog</param>
void DDEvent::setHost(string host)
{
	this->host = host;
}

/// <summary>Set the alert type, if not used defaults to Info</summary>
/// <param name="alertType">An AlertType enum value</param>
void DDEvent::setAlertType(DDEvent::AlertType alertType)
{
	this->alertType = alertType;
}

/// <summary>Set the aggregation key</summary>
/// <param name="aggegationKey">The aggregation key value to send to datadog</param>
void DDEvent::setAggregationKey(string aggregationKey)
{
	this->aggregationKey = aggregationKey;
}

void DDEvent::setTags(string tags)
{
	this->tags = tags;
}

/// <summary>Return the DDEvent object in the encoded format required for DataDog UDP Event Submissions</summary>
string DDEvent::returnDDEventUDPString()
{
	stringstream fields;

	fields << this->title;
	if (!this->text.empty())
	{
		string tempString = this->text;
		
		this->stringReplace(tempString, "\n", "\\n");
		fields << "|" << tempString;
	}
	else
	{
		fields << "|";
	}
	
	if (!this->dateHappened.empty())
	{
		fields << "|d:" << this->dateHappened;
	}
	
	if (!this->host.empty())
	{
		fields << "|h:" << this->host;
	}

	if (!this->aggregationKey.empty())
	{
		fields << "|k:" << this->aggregationKey;
	}

	if (this->priority != DDEvent::Priority::NOT_SET)
	{
		fields << "|p:" << this->getPriorityStringFromEnum(this->priority);
	}
	
	if (this->alertType != DDEvent::AlertType::NOT_SET)
	{
		fields << "|t:" << this->getAlertTypeStringFromEnum(this->alertType);
	}

	if (!this->tags.empty())
	{
		fields << this->tags;
	}

	int titleLength = this->title.length();
	int textLength = this->text.length();

	stringstream outputString;
	outputString << "_e{" << titleLength << "," << textLength << "}:" << fields.str();

	return outputString.str();
}

std::string DDEvent::getPriorityStringFromEnum(DDEvent::Priority priority)
{
	switch (priority)
	{
		case Priority::LOW:
			return "low";
		case Priority::NORMAL:
			return "normal";
		default:
			return "";
	}
}

std::string DDEvent::getAlertTypeStringFromEnum(DDEvent::AlertType alertType)
{
	switch (alertType)
	{
		case AlertType::INFO:
			return "info";
		case AlertType::ERROR:
			return "error";
		case AlertType::SUCCESS:
			return "success";
		case AlertType::WARNING:
			return "warning";
		default:
			return "";
	}
}

void DDEvent::stringReplace(string& source, string needle, string replace)
{
	while (source.find(needle) != std::string::npos)
	{
		source.replace(source.find(needle), needle.length(), replace);
	}
}