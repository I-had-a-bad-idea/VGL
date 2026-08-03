CXX=g++

CXXFLAGS=-std=c++20 \
-Iinclude \
-IC:/VulkanSDK/1.4.357.0/include \
-DVK_NO_PROTOTYPES 

LDFLAGS= \
-LC:/VulkanSDK/1.4.357.0/Lib \
-lvulkan-1 \
-lSDL3

test:
	$(CXX) $(CXXFLAGS) src/*.cpp test.cpp $(LDFLAGS) -o test


compile-and-run-test: test
	./test