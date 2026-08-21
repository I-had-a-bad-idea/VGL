CC = gcc
CXX = g++

VULKAN_SDK = C:/VulkanSDK/1.4.357.0

LIB_TARGET = VulkanGraphicsLib.a
TEST_TARGET = test.exe

CFLAGS = \
-Iexternal/ktx/include \
-Iexternal/ktx/other_include \
-I$(VULKAN_SDK)/Include

CXXFLAGS = \
-std=c++20 \
-Iinclude \
-isystem external \
-isystem external/ktx/include \
-isystem external/ktx/other_include \
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
external/ktx/lib/texture.c \
external/ktx/lib/hashlist.c \
external/ktx/lib/checkheader.c \
external/ktx/lib/swap.c \
external/ktx/lib/memstream.c \
external/ktx/lib/filestream.c \
external/ktx/lib/vkloader.c

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