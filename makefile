SOURCES = DataDogStatsD.cpp DDEvent.cpp Helpers.cpp

lib_name = libDataDogStatsD.so.1.0.0.0

OBJECTS = $(SOURCES:.cpp=.o)
CFLAGS = -fpic -c $(SOURCES) -Wall -g -Iincludes -std=c++11 -I/usr/include
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

