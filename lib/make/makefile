### Defines ####################################################################
UNAME := ${shell uname}

CXX       := g++
CXXFLAGS  := -Wall -std=c++0x -v $(CXXDEFS)

ARCH      := $(shell uname -m)
TARGET    :=  ledLib$(ARCH).so
PROJ_ROOT := $(realpath ..)
VPATH     := $(PROJ_ROOT)/src
INCLUDES  := `pkg-config opencv --cflags` \
	     -I $(PROJ_ROOT)/common
SRCS      := led.cpp
LIBS      := -lpthread \
	     -ldl \
	     `pkg-config opencv --libs` \
	     -lboost_serialization \
	     -lboost_system \
	     -lm
OBJS      := $(SRCS:.cpp=.o)

### Rules ######################################################################

.PHONY: all
all: $(TARGET)

.PHONY: make
make: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -shared -v -o $@ $(OBJS) $(LIBS)



.PHONY: clean
clean:
	$(RM) -f *so *.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<