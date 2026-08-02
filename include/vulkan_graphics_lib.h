#include <vulkan/vulkan.h>

#include <string>

#include "instance.h"
#include "device.h"
#include "queues.h"
#include "handle.h"
#include "vma_setup.h"


class Renderer {};

Renderer create_renderer(std::string name);