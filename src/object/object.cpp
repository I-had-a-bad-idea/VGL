#include "object.h"

Object::Object(Mesh* mesh, Material* material)
    : mesh(mesh), material(material), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), selected(0) {}

glm::mat4 Object::model_matrix() const {
    glm::mat4 model = glm::mat4(1.0f);

    // Translation
    model = glm::translate(model, position);

    // Rotation (assuming rotation stores Euler angles in radians)
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    return model;
}