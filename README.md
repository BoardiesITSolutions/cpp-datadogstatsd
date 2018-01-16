# C++ DataDog StatsD Client

This is a simple library to allow you to submit custom metrics from your C++ apps in 
to your [DataDog](https://datadoghq.com) account. 

The source code for this library was ported from the official [PHP DataDog StatsD client 
library](https://github.com/DataDog/php-datadogstatsd).


The library supports Linux, any Linux distro should work but we've tested against
CentOS 7.4. 

## Installation
There is a simple makefile to allow you to build a so library file that you can
link against. At this time, there should be any dependencies required in order
to build and run the library, although this will likely change in the future
(check what's next for more info). If you build successfully you should get
libDataDogStats.so.x.x.x.x (x.x.x.x being the version number) 

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

# What's Next?
The next stage is for us to implement sending events to DataDog, over UDP and TCP.
We mentioned at the start that in the future the library will likely require
dependencies and this is the reason why.
