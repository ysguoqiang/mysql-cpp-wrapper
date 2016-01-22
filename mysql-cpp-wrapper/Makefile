CC:= g++
CFLAGS:= -Wall -O2 -c
LDFLAGS:= $(shell mysql_config --libs)
OBJSs:= $(wildcard *.cpp)
OBJS:= $(patsubst %.cpp, %.o, $(OBJSs))
OUT:= mysql_test

.PHONY:clean

$(OUT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $^
	
clean:
	-rm $(OBJS) $(OUT)
