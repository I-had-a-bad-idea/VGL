CC = gcc
CXX = g++

VULKAN_SDK = C:/VulkanSDK/1.4.357.0

LIB_TARGET = VGL.a
TEST_TARGET = test.exe

CFLAGS = \
-Iinclude/ktx/include \
-I$(VULKAN_SDK)/Include

CXXFLAGS = \
-std=c++20 \
-Iinclude \
-isystem include/ktx/include \
-DVK_NO_PROTOTYPES

LDFLAGS = \
-L$(VULKAN_SDK)/Lib \
-lvulkan-1 \
-lSDL3 \
-lslang

CPP_SOURCES := $(wildcard src/*.cpp src/object/*.cpp src/renderer/*.cpp)

TEST_SOURCES = test.cpp
TEST_OBJECTS = ${TEST_SOURCES:.cpp=.o}

KTX_SOURCES := \
include/ktx/lib/texture.c \
include/ktx/lib/hashlist.c \
include/ktx/lib/checkheader.c \
include/ktx/lib/swap.c \
include/ktx/lib/memstream.c \
include/ktx/lib/filestream.c \
include/ktx/lib/vkloader.c

CPP_OBJECTS := $(CPP_SOURCES:.cpp=.o)
KTX_OBJECTS := $(KTX_SOURCES:.c=.o)

all: ${LIB_TARGET}

${LIB_TARGET}: ${CPP_OBJECTS} ${KTX_OBJECTS}
	ar rcs $@ $^

${TEST_TARGET}: $(TEST_OBJECTS) $(LIB_TARGET)
	$(CXX) $(TEST_OBJECTS) $(LIB_TARGET) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(subst /,\,$(CPP_OBJECTS)) $(subst /,\,$(KTX_OBJECTS)) ${LIB_TARGET} ${TEST_TARGET} 2>nul || exit 0

compile-and-run-test: ${TEST_TARGET}
	./${TEST_TARGET}


.PHONY: all clean ${LIB_TARGET}