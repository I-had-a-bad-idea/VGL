#include "object.h"


Material::Material(Texture* texture, Shader* shader) {
    albedo = texture;
    shader = shader;
}