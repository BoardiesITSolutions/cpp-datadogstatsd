SOURCES = DataDogStatsD.cpp DDEvent.cpp Helpers.cpp

lib_name = libDataDogStatsD.so.1.1.0.4

curl_include = /usr/include/curl
rapidjson_inc_path = /usr/include/rapidjson

OBJECTS = $(SOURCES:.cpp=.o)
CFLAGS = -fpic -c $(SOURCES) -Wall -g -Iincludes -std=c++11 -I/usr/include -I$(curl_include) -I$(rapidjson_inc_path)
CC = g++
LDFLAGS = -lpthread -pthread -lm -L/usr/lib64/

.PHONY: clean

default:
	$(CC) -shared -Wl,-soname,libDataDogStatsD.so.1 -o $(lib_name) $(OBJECTS) $(LDFLAGS)
	ln -sf $(lib_name) libDataDogStatsD.so

clean:
	rm -vf $(OBJECTS) depend $(lib_name)

depend: $(SOURCES)
	$(CC) $(CFLAGS) > depend

-include depend
