### Defines ####################################################################
CXX = g++
CXXFLAGS = -Wall -std=c++11 -v

TARGET = 3dLed

PROJ_ROOT = $(realpath ..)

SRCS = $(PROJ_ROOT)/sender/led.cpp \
	$(PROJ_ROOT)/sender/main.cpp

OBJS := $(SRCS:.cpp=.o)

INCLUDES = -I/usr/local/include/opencv \
	-I/usr/local/include/ \
	-I $(PROJ_ROOT)/common \
	-I $(PROJ_ROOT)/sender \
	
LIBS =  -lpthread \
	-ldl \
	-lopencv_core \
	-lopencv_imgproc \
	-lopencv_highgui \
	-lopencv_ml \
	-lboost_serialization \
	-lboost_serialization \
	-lboost_system \
	-lbcm2835 \
	-lm

### Rules ######################################################################

all: $(TARGET)
make: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -v -o $@ $(OBJS) $(LIBS)

clean:
	rm -f $(TARGET) *.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<