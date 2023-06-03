TARGET_1 = testsink
TARGET_2 = testvent
TARGET_3 = main

CC = g++
VERSION=\"1.0.0\"
CXXFLAGS=-g -O0 -w -Wall -std=gnu++98 -D_DEBUG -DVERSION="$(VERSION)"

INCLUDES = -I /usr/local/include         \
           -I ../sc-common/include       \
		   -I ./include

LIBS += -L/usr/local/lib -L../sc-common/Release
LDLIBS += -lzmq -lpthread -lmycommon

SRCS_1 := $(wildcard ./test/testsink.cpp ./src/itwconfig.cpp)

SRCS_2 := $(wildcard ./test/testvent.cpp)
SRCS_2 += $(wildcard ./src/itwconfig.cpp)

SRCS_3 := $(wildcard ./src/*.cpp)

OBJS_1  := $(SRCS_1:%.cpp=%.o)
OBJS_2  := $(SRCS_2:%.cpp=%.o)
OBJS_3  := $(SRCS_3:%.cpp=%.o)

all:$(TARGET_1) $(TARGET_2) $(TARGET_3)
$(TARGET_1):$(OBJS_1)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LIBS) $(LDLIBS)

$(TARGET_2):$(OBJS_2)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LIBS) $(LDLIBS)

$(TARGET_3):$(OBJS_3)
	$(CC) $(CXXFLAGS) $^ -o $@ $(LIBS) $(LDLIBS) -lhiredis -lcurl

%.o: %.cpp
	@echo "compiling $? ..."
	$(CC) -c $(CXXFLAGS) $? -o $@ $(INCLUDES)

.PHONY:
clean:
	rm -f $(OBJS_1)
	rm -f $(OBJS_2)
	rm -f $(OBJS_3)
	rm -f $(TARGET_1)
	rm -f $(TARGET_2)
	rm -f $(TARGET_3)