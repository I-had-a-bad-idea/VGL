#include "VGL/math.h"

glm::mat4 model_matrix_from_pos_and_rot(glm::vec3 pos, glm::vec3 rot) {
    glm::mat4 model = glm::mat4(1.0f);

    // Translation
    model = glm::translate(model, pos);

    // Rotation (assuming rotation stores Euler angles in radians)
    model = glm::rotate(model, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

    return model;
}