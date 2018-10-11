### Defines ####################################################################
UNAME := ${shell uname}

CXX       := g++
CXXFLAGS  := -Wall -std=c++0x -v $(CXXDEFS)

ARCH      := $(shell uname -m)
TARGET    :=  demo
PROJ_ROOT := $(realpath .)
VPATH     := $(PROJ_ROOT)
INCLUDES  := `pkg-config opencv --cflags`
SRCS      := led.cpp main.cpp
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
	$(CXX) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) -f $(TARGET) *.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<