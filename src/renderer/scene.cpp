#include "VGL/renderer.h"


void Scene::add_object_to_scene(Object* object) {
    objects.push_back(object);
    objects_by_mesh_by_shader[object->material->shader][object->mesh].push_back(object);
}

void Scene::remove_object_from_scene(Object* object) {
    auto& objects_for_mesh = objects_by_mesh_by_shader[object->material->shader][object->mesh]; // find object mesh group

    // if it is the only object with this shader, remove the shader group from the scene
    if (objects_by_mesh_by_shader[object->material->shader].size() == 1) {
        objects_by_mesh_by_shader.erase(object->material->shader);
    }

    // else if it is the only object with this mesh, remove the mesh group from the scene
    else if (objects_for_mesh.size() == 1) {
        objects_by_mesh_by_shader[object->material->shader].erase(object->mesh);
    }
    else {
        // else just remove from the group
        auto it = std::find(objects_for_mesh.begin(), objects_for_mesh.end(), object); // Find object in group
        if (it != objects_for_mesh.end()) {
            objects_for_mesh.erase(it); // Delete it
        }
    }


    auto it2 = std::find(objects.begin(), objects.end(), object); // Remove from main object list
    if (it2 != objects.end()) {
        objects.erase(it2);
    }
}

glm::mat4 Scene::view_matrix() const {
    glm::mat4 camera_transform = glm::translate(
        glm::mat4(1.0f),
        cam_pos
    ) * glm::mat4_cast(cam_orientation);

    return glm::inverse(camera_transform);
}