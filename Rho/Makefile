CXX=g++
TARGET=thread_rho
CXXFLAGS=--std=c++11
LDFLAGS=-lgmpxx -lgmp -lpthread

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)
clean:
	rm $(TARGET)
