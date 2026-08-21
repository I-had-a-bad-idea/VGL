#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

glm::mat4 model_matrix_from_pos_and_rot(glm::vec3 pos, glm::vec3 rot);

