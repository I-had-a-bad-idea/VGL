#include "VGL/object.h"

Object::Object(Mesh* mesh, Material* material, glm::vec3 position, glm::vec3 rotation)
    : mesh(mesh), material(material), position(position), rotation(rotation), selected(0) {}

glm::mat4 Object::model_matrix() const {
    return model_matrix_from_pos_and_rot(position, rotation);
}