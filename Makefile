CXX=g++

CXXFLAGS=-std=c++20 \
-Iinclude \
-isystem external \
-IC:/VulkanSDK/1.4.357.0/include \
-DVK_NO_PROTOTYPES 

LDFLAGS= \
-LC:/VulkanSDK/1.4.357.0/Lib \
-lvulkan-1 \
-lSDL3

SOURCES := $(wildcard src/*.cpp src/object/*.cpp src/renderer/*.cpp) test.cpp

test: ${SOURCES}
	$(CXX) $(CXXFLAGS) ${SOURCES} $(LDFLAGS) -o test


compile-and-run-test: test
	./test