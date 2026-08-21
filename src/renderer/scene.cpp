#include "VGL/renderer.h"


void Scene::add_object_to_scene(Object* object) {
    objects.push_back(object);
    objects_by_mesh_by_shader[object->material->shader][object->mesh].push_back(object);
}

glm::mat4 Scene::view_matrix() const {
    glm::mat4 camera_transform = glm::translate(
        glm::mat4(1.0f),
        cam_pos
    ) * glm::mat4_cast(cam_orientation);

    return glm::inverse(camera_transform);
}