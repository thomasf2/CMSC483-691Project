CXX=g++
TARGET=thread_rho
CXXFLAGS=-std=c++11 -DLOG_OUTPUT
LDFLAGS=-lgmpxx -lgmp -lpthread

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)
	
.PHONY: clean
clean:
	rm $(TARGET)
