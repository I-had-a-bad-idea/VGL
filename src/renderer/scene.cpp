#include "VGL/renderer.h"


void Scene::add_object_to_scene(Object* object) {
    objects.push_back(object);
    objects_by_mesh_by_shader[object->material->shader][object->mesh].push_back(object);
}

glm::mat4 Scene::view_matrix() const {
    return model_matrix_from_pos_and_rot(cam_pos, cam_rot);
}