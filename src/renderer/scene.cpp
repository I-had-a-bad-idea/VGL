#include "renderer.h"


void Scene::add_object_to_scene(Object* object) {
    objects.push_back(object);
    objects_by_mesh_by_shader[object->material->shader][object->mesh].push_back(object);
}