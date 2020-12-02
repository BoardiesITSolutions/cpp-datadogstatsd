
<a href="https://ci.boardiesitsolutions.com/viewType.html?buildTypeId=CppDatadogstatsd_Build&guest=1">
<img src="https://ci.boardiesitsolutions.com/app/rest/builds/buildType:(id:CppDatadogstatsd_Build)/statusIcon"/>
</a>

# C++ DataDog StatsD Client

This is a simple library to allow you to submit custom metrics from your C++ apps in 
to your [DataDog](https://datadoghq.com) account. 

The source code for this library was ported from the official [PHP DataDog StatsD client 
library](https://github.com/DataDog/php-datadogstatsd).


The library supports Linux, any Linux distro should work but we've tested against
CentOS 7.4. It also supports Windows, tested on Windows 10. 

## Installation
There is a simple makefile to allow you to build a so library file that you can
link against. The library requires [CURL](https://curl.haxx.se/libcurl/) and [RapidJSON](https://github.com/Tencent/rapidjson)

How you link to the library is up to you, but for our installation we 
did the following:



```
mkdir /usr/lib64/BoardiesITSolutions
cp libDataDogStatsD.so.x.x.x.x /usr/lib64/BoardiesITSolutions
```

Then add `/usr/lib64/BoardiesITSolutions` to `/etc/ld.so.conf`. 

Run `ldconfig` and if you check the directory above you should then see a symlink
that points `libDataDogStatsD.so.x -> libDataDogStatsD.so.x.x.x.x`. 

Store your header files somewhere as your C++ application will need to use them 
during its build process

In your C++ applications make file update your CFLAGS to include 
`-I</path/to/headers/`

Update your LDFLAGS to include `-L/usr/lib64/BoardiesITSolutions -l:libDataDogStatsD.so.1`.

## Sending Metrics to DataDog

Sending metrics couldn't be simpler. 
The first thing to do is to create an instance of the library as follows:
```
DataDogStatsD dataDogStatsD;
```
You can configure the agent via envionment variables. If you instantiate your the library
where the host and the port number is not set, then it checks if `DD_AGENT_HOST` and/or 
`DD_DOGSTATSD_PORT` and sets them appropriately. If there is no environment variable found, then
it defaults to 127.0.0.1 and port 8125. 

You can also set an environment variable called DD_ENTITY_ID. If this is provided then whenever a metric
is submitted to datadog then the tag `dd.internal.entity_id` is applied. This is only applied though
if the agent is running from inside a container. 

### General Notes for all Metrics
All methods available to send metrics to your DataDog account have an optional value
of sending tags. The tags can be in the form of a standard string, a list of strings
(vector) or a key/value list of tags (map); In order for you to successfully send
metrics with tags, they need to be seralized correctly. 

The return value in the 3 examples of returnSerializedTagsString is then
used as the tags parameter for all the metric submission methods

#### Serializing a string tag
```
std::string myTag = dogStatsd.returnSerializedTagsString("MyTag");
```

#### Serializing a string array (vector)
```
std::vector<std::string> myTags;
myTags.push_back("item_1");
myTags.push_back("item_2");
std::string tags = dogStatsd.returnSerializedTagsString(myTags);
```

#### Serializing a key/value array (map)
```

std::map<std::string, std::string> myTags;
myTags["item_1"] = 1;
myTags["item_2"] = 2;
myTags["item_3"] = 3;
std::string tags = dogStatsd.returnSerializedTagsString(myTags);
```

### Increment
To increment how many times something happens a second, e.g. page views
```
DataDogStatsD dataDogStatsD;
//No Tags
dogStatsD.increment("app.TestApp");
//Using tags
dogStatsD.increment("app.TestApp", myTags);
```

### Decrement
Decrement the counter for how many times an event has happened
```
DataDogStatsD dataDogStatsD;
//No Tags
dogStatsD.decrement("app.TestApp");
//Using tags
dogStatsD.increment("app.TestApp", myTags);
```

### Gauges
Track the ebb and flow of a particular metric over time - e.g. the number of active users on a site
```
DataDogStatsD dataDogStatsD;
//No tags
dogStatsD.gauge("app.TestApp.Gauge", value);
//With tags
dogStatsD.gauge("app.TestApp.Gauge", value, myTags);
```

### Sets
Count the number of unique elements in a group, such as unique visitors
```
DataDogStatsD dataDogStatsD;
//No tags
dogStatsD.set("app.TestApp.Set", value);
//With tags
dogStatsD.set("app.TestApp.Set", value, myTags);
```

### Timers
Timers measures the amount of time something happens, e.g. how long a particular 
function takes to run.
There's a helper function here in order to return the correct time format. 
To get the time use `dogStatsD.getTimeInMicroSeconds` as shown below
```
DataDogStats dataDogStatsD;
long startTime = dogStatsD.getTimeInMicroSeconds();
...Run some function...
//No tags
dogStatsD.timing("app.TestApp.Timing", dogStatsD.getTimeInMicroSeconds() - startTime);
//With tags
dogStatsD.timing("app.TestApp.Timing", dogStatsD.getTimeInMicroSeconds() - startTime, myTags);
```

### Histograms
Histograms calculate the statistical distribution of any kind of value
```
DataDogStatsD dataDogStatsd;#
//No tags
dogStatsD.histogram("app.TestApp.Histogram", value);
//With tags
dogStatsD.histogram("app.TestApp.Histogram", value);
```

# Events
There are two ways to send events. Over UDP, which uses the agent installed on the server to forward the event to your
Datadog account. This has no guarantees that it will succeed as it is over UDP, and there is no way from your 
application whether or not it was successful (chances are if the agent is running and your 
server is successfully sending data to datadog, it will most likely send the event). 

If on the other hand, you want/need to know whether the event did successfully get submitted to datadog, the library
also supports sending over HTTP (TCP). This can be done in blocking and non blocking mode. In blocking mode your 
applications thread will stop, while the event submission is being executed and then continue once done. You'll receive
true on success or false on failure, if you want more details, such as why the event submission failed, you can pass in 
an optional call back to receive the error response. 

If you send the event in non blocking mode, a new thread will be created to perform the HTTP event. This will allow your app
to continue running its own code, while in the background the library is sending the event. If in non blocking mode, you
must pass in a call back function so that you can receive the success/failure result.

## Sending Events
No matter how you decide to send the event, you will always need to do the same thing, create the event object. 
Add the header `DDEvent.h` to your application. 

You then need to create an instance of the DDEvent class. To do this you need to pass in two parameters to the 
constructor. The event title (or the header) and the event text (or the description), e.g. `DDEvent ddEvent("My Event", "Here is my event description");`

There are then multiple methods that you can use to set the rest of the event data if required. Below is a list 
of the methods and what parameters they take.


* setDateHappened(size_t epochTime)
* setPriority(enum Priority)
* setHost(string)
* setAlertType(enum AlertType)
* setAggregationKey(string)
* setTags(string)
* setTags(vector<string>)
* setTags(map<string, string)

You may have noticed, that there are 3 different setTags functions. This allows you to send the event with a single string,
multiple strings, or key/value pairs. When using HTTP events do not set the tag string to be what is returned from 
`returnSerializedTagsString()` as this method is only required for sending metrics, NOT events. 

Now that you have created your event object, you can now use to send an event to DataDog. 

## Sending Event over UDP
If you are sending an event to DataDog over UDP, then you need to create an instance of DataDogStatsD with no 
parameters (it doesn't matter if you do use parameters, but they're not needed). Then call the event method within
the DataDogStatsD class with only the 1 parameter, the parameter being your DDEvent object as below.

```
DataDogStatsD dataDogStatsD;
DDEvent myDDEvent("Here is my event", "Here is the event description");
dataDogStatsD.event(myDDEvent);
```

That's it your event should have been submitted. 

## Sending Event over HTTP (TCP)
When sending an event over TCP you need to create the instance of the DataDogStatsD class slightly differently. You need
to pass 2 parameters to the constructor, one being your API key, and the second being your app key. For more information
on creating/finding your API and App key visit [DataDog API Integration](https://app.datadoghq.com/account/settings#api).

To send the event, you need to call the event function, but for it to send it over HTTP you need to pass in some
extra parameters (if you just pass in the DDEvent object it will send it over UDP). 

The event method for sending over HTTP takes the following parameters:

* **DDEvent:** Your DDEvent class object that contains the event details
* **bool:** Whether or not to send the HTTP event in blocking or non blocking mode
* **void(eventCallBack)(bool, string):** This is an optional (only optional in blocking mode) function pointer to a 
callback method where the library will return the result of the HTTP event submission, and in the event of an error,
the response from DataDog explaining why it failed. 

### Sending in Blocking Mode
As mentioned earlier, sending in blocking mode will result in your applications thread to stop, while waiting for the 
response to be returned from DataDog. Doing this too often, or if there is a network issue somewhere, could cause 
unexpected delays in your application. 

If you want to send the event in blocking mode, then the second parameter should be false, with the callback parameter being
optional. If you don't want to use the callback method then you can pass nullptr as the 3rd parameter. 
If you do want to the callback method, then pass your callback function as the 3rd parameter (creating the callback function
is explained below). 

Below is an example on how to send the event in blocking mode.

```
DataDogStatsD dataDogStatsD("<your api key>", "<your app key>");
DDEvent myDDEvent("My Event", "Here is the description of my event");
dataDogStatsD.event(myDDEvent, false, nullptr); //Send without using the callback method
dataDogStatsD.event(myDDEvent, false, callbackFunc); //Send using the callback method
```

When in blocking mode, regardless of whether the callback method was used, the event method will return
true on success, or false on failure. When using the callback method, the event method will still return true
or false, but your callback function will also be called with true on success, or false on failure. If a failure has occurred
then the string in the second parameter of your callback method will contain the response from DataDog. 

### Sending in Non Blocking Mode
If you send the HTTP event in non blocking mode, the library will create a new thread in order to perform the event
submission, this will allow your application to continue running while the event is being submitted. You can only 
send 1 event at a time, so if there is already an event submission in progress when you try sending another, you will
get false returned from the event function. 

To send in non blocking mode the second parameter will be true, the callback method has to be provided, sending a nullptr for 
this parameter will result in false being returned from the event method and your event will not be submitted. 

Below is an example of sending the event in non blocking mode.

```
DataDogStatsD dataDogStatsD("<your api key>", "<your app key>");
DDEvent myDDEvent("My Event", "Here is the description of my event");
dataDogStatsD.event(myDDEvent, true, callbackFunc); //Send using the callback method
```

Although the event call is required for non blocking mode, it may also be useful to check the return value 
of the event method. If the event submission thread is already running, then the event method will return false, and your 
event callback response will contain the message `HTTP Event thread is already sending an event, wait until the current event has been finished`.

If the event thread wasn't already running, then the event method will return true. Returning true does not mean that the event
was successfully submitted, only that the thread was successfully started, you will receive the event submission result within your callback.

### Creating the Event Submission Callback Method
First of all, in your header file, create the prototype, for your callback method (Note that the example below 
was done from main.cpp, if you are doing it from a class, then the class will likely needed to prefixed in your actual 
callback method code, e.g. MyClass:myCallback()).

```
void(eventCallback)(bool, std::string);`
```

Now in your cpp file add the code for your callback method, below is an example callback method. 

```
void eventCallback(bool result, string error)
{
	    if (result)
	    {
		    cout << "HTTP Event Executed Successfully" << endl;
    	}
	    else
	    {
		    cout << "HTTP Event Failed. Error: " << error << endl;
	    }
}
```

Whatever you call your call back method is what goes into the 3rd parameter for the event method. 

If you have any issues or questions, then please either submit them via the GitHub issue tracker
or via our [support portal](https://support.boardiesitsolutions.com). 

If using our support portal, then, for general help, please submit a support ticket at 
https://support.boardiesitsolutions.com/support/ or if you are reporting a bug/issue then please
submit it via our issue tracker at https://support.boardiesitsolutions.com/issues/index.php. 

# Contributions
If you wish to contribute to this project, then great, check our [contribution guidelines](CONTRIBUTING.md)

# Contributors
anderssjoblom-vertex - https://github.com/anderssjoblom-vertex
niamster - https://github.com/niamster
justinkamerman - https://github.com/justinkamerman
