CXX = g++
CXXFLAGS = -Wall -std=c++11

SRCDIR = sorce
INCDIR = include
TARGET = campus_nav.exe

SRCS = $(SRCDIR)/main.cpp $(SRCDIR)/data.cpp $(SRCDIR)/graph.cpp $(SRCDIR)/router.cpp $(SRCDIR)/search.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCS) -o $(TARGET)

clean:
	del /f $(TARGET) 2>nul || rm -f $(TARGET)

.PHONY: all clean
