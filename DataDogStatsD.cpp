#include "DataDogStatsD.h"

using namespace std;

/// <summary>Initialise the DogStatsD with the API and app key, defaulting to host localhost and port 8125</summary>
/// <param name="api_key">Your API Key</param>
/// <param name="app_key">Your app key</param>

DataDogStatsD::DataDogStatsD()
{
	this->host = "127.0.0.1";
	this->port = 8125;
}

DataDogStatsD::DataDogStatsD(std::string host, unsigned int port)
{
	this->host = host;
	this->port = port;
}

DataDogStatsD::DataDogStatsD(string api_key, string app_key)
{
this->api_key = api_key;
this->app_key = app_key;
this->host = "127.0.0.1";
this->port = 8125;
}

/// <summary>Initialise the DogStatsD with the API and app key, customised hostname and default port of 8125</summary>
/// <param name="api_key">Your API Key</param>
/// <param name="app_key">Your app key</param>
/// <param name="host">The hostname to send to DogStatsD
DataDogStatsD::DataDogStatsD(string api_key, string app_key, string host)
{
	this->api_key = api_key;
	this->app_key = app_key;
	this->host = host;
	this->port = 8125;
}

/// <summary>Initialise the DogStatsD with the API and app key, customised port number defaults to host localhost</summary>
/// <param name="api_key">Your API Key</param>
/// <param name="app_key">Your app key</param>
/// <param name="port">The port number to send to DogStatsD</param>
DataDogStatsD::DataDogStatsD(string api_key, string app_key, unsigned int port)
{
	this->api_key = api_key;
	this->app_key = app_key;
	this->host = "127.0.0.1";
	this->port = port;
}

/// <summary>Initialise the DogStatsD with the API and app key, with customised hostname and port</summary>
/// <param name="api_key">Your API Key</param>
/// <param name="app_key">Your app key</param>
/// <param name="host">Hostname to send to DogStatsD</param>
/// <param name="port">Port number to send to DogStatsD</param>
DataDogStatsD::DataDogStatsD(string api_key, string app_key, string host, unsigned int port)
{
	this->api_key = api_key;
	this->app_key = app_key;
	this->host = host;
	this->port = port;
}

void DataDogStatsD::increment(std::string stats)
{
	std::vector<std::string> statsArray;
	statsArray.push_back(stats);
	this->updateStats(statsArray, 1, 1.0, "");
}

void DataDogStatsD::increment(std::string stats, std::string tags)
{
	std::vector<std::string> statsArray;
	statsArray.push_back(stats);
	this->updateStats(statsArray, 1, 1.0, tags);
}

void DataDogStatsD::increment(std::vector<std::string> stats)
{
	this->updateStats(stats, 1, 1.0, "");
}

void DataDogStatsD::increment(std::vector<std::string> stats, std::string tags)
{
	this->updateStats(stats, 1, 1.0, tags);
}

void DataDogStatsD::decrement(std::string stats)
{
	std::vector<std::string> statsArray;
	statsArray.push_back(stats);
	this->updateStats(statsArray, -1, 1.0, "");
}

void DataDogStatsD::decrement(std::string stats, std::string tags)
{
	std::vector<std::string> statsArray;
	statsArray.push_back(stats);
	this->updateStats(statsArray, -1, 1.0, tags);
}

void DataDogStatsD::decrement(std::vector<std::string> stats)
{
	this->updateStats(stats, -1, 1.0, "");
}

void DataDogStatsD::decrement(std::vector<std::string> stats, string tags)
{
	this->updateStats(stats, -1, 1.0, tags);
}

void DataDogStatsD::timing(std::string stats, float timeInSeconds)
{
	stringstream valueStream;
	valueStream << (timeInSeconds) << "|ms";
	std::map<std::string, std::string> data;
	data[stats] = valueStream.str();
	this->send(data, 1.0);
}

void DataDogStatsD::timing(std::string stats, float timeInSeconds, std::string tags)
{
	stringstream valueStream;
	valueStream << (timeInSeconds) << "|ms";
	std::map<std::string, std::string> data;
	data[stats] = valueStream.str();
	this->send(data, 1.0, tags);
}



void DataDogStatsD::gauge(std::string stats, float value)
{
	stringstream valueStream;
	valueStream << (value) << "|g";

	std::map<std::string, std::string> data;
	data[stats] = valueStream.str();
	this->send(data, 1.0);
}

void DataDogStatsD::gauge(std::string stats, float value, std::string tags)
{
	stringstream valueStream;
	valueStream << (value) << "|g";

	std::map<std::string, std::string> data;
	data[stats] = valueStream.str();
	this->send(data, 1.0, tags);
}

void DataDogStatsD::histogram(std::string stats, float value)
{
	stringstream valueStream;
	valueStream << (value) << "|h";

	std::map<std::string, std::string> data;
	data[stats] = valueStream.str();
	this->send(data, 1.0);
}

void DataDogStatsD::histogram(std::string stats, float value, std::string tags)
{
	stringstream valueStream;
	valueStream << (value) << "|h";

	std::map<std::string, std::string> data;
	data[stats] = valueStream.str();
	this->send(data, 1.0, tags);
}

void DataDogStatsD::set(std::string stats, float value)
{
	stringstream valueStream;
	valueStream << (value) << "|s";

	std::map<std::string, std::string> data;
	data[stats] = valueStream.str();
	this->send(data, 1.0);
}

void DataDogStatsD::set(std::string stats, float value, std::string tags)
{
	stringstream valueStream;
	valueStream << (value) << "|s";

	std::map<std::string, std::string> data;
	data[stats] = valueStream.str();
	this->send(data, 1.0, tags);
}

void DataDogStatsD::updateStats(std::vector<std::string> stats, int delta, float sampleRate, string tags)
{
	std::map<string, string> data;

	//Loop over the stats and create the multi dimension array of stat => value
	for (std::vector<std::string>::iterator it = stats.begin(); it != stats.end(); ++it)
	{
		data[*it] = "1|c";
	}

	this->send(data, sampleRate, tags);

}

void DataDogStatsD::send(std::map<string, string> data, float sampleRate, string tags)
{
	if (data.size() == 0)
	{
		return;
	}

	std::map<string, string> sampledData;
	if (sampleRate < 1)
	{
		for (std::map<string, string>::iterator it = data.begin(); it != data.end(); ++it)
		{
			if ((Helpers::rand() / Helpers::rand_mx()) <= sampleRate)
			{
				stringstream valueStream;
				valueStream << it->second << "|@" << sampleRate;
				string value = valueStream.str();
				sampledData[it->first] = value;
			}
		}
	}
	else
	{
		sampledData = data;
	}

	if (sampledData.size() == 0)
	{
		return;
	}

	for (std::map<string, string>::iterator it = sampledData.begin(); it != sampledData.end(); ++it)
	{
		stringstream output;

		output << it->first << ":" << it->second << tags;
		string udp_message = output.str();
		this->flush(udp_message);
	}
}

void DataDogStatsD::flush(string& udp_message)
{
#ifndef _WIN32

	int udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	/*struct sockaddr_in sin;

	sin.sin_family = AF_INET;
	inet_pton(AF_INET, this->host.c_str(), &sin.sin_addr);
//	sin.sin_addr.s_addr = htonl(this->host.c_str());
	sin.sin_port = htons(this->port);*/

	struct sockaddr_in serv_addr;
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = htonl(this->host.c_str());
	inet_pton(AF_INET, this->host.c_str(), &serv_addr.sin_addr);
	serv_addr.sin_port = htons(this->port);

	/*if (!bind(udp_socket, (struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		cout << "Failed to bind UDP port" << endl;
	}*/

	connect(udp_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	write(udp_socket, udp_message.c_str(), udp_message.length());
	

	close(udp_socket);
#else
	cout << "Sending " << udp_message << endl;
#endif
}

string DataDogStatsD::returnSerializedTagsString(std::string tags)
{
	stringstream valueStream;
	valueStream << "|#" << tags;
	return valueStream.str();
}

string DataDogStatsD::returnSerializedTagsString(std::vector<std::string> tags)
{
	if (tags.size() == 0)
	{
		return string("");
	}

	stringstream valueStream;
	valueStream << "|#";
	int i = 0;
	for (std::vector<string>::iterator it = tags.begin(); it != tags.end(); ++it)
	{
		valueStream << i << ":" << *it << ",";
		i++;
	}
	string value = valueStream.str();
	value.pop_back();
	return value;
}
	
string DataDogStatsD::returnSerializedTagsString(std::map<std::string, std::string> tags)
{
	stringstream valueStream;
	valueStream << "|#";
	int i = 0;
	for (std::map<string, string>::iterator it = tags.begin(); it != tags.end(); ++it)
	{
		valueStream << it->first << ":" << it->second << ",";
		i++;
	}
	string value =  valueStream.str();
	value.pop_back();
	return value;
}

long DataDogStatsD::getTimeInMicroSeconds()
{
#ifndef _WIN32
	struct timeval time;
	gettimeofday(&time, NULL);
	long microsec = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
	return microsec;
#else
	return 0;
#endif
}