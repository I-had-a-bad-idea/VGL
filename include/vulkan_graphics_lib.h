#pragma once

#include "volk.hpp"
#include <volk/volk.h>
#include <vma/vk_mem_alloc.h>

#include <string>

#include "sdl.hpp"
#include "instance.h"
#include "device.h"
#include "queues.h"
#include "handle.h"
#include "vma_setup.h"
#include "window.h"

class Renderer {};

Renderer create_renderer(std::string name, int w, int h);