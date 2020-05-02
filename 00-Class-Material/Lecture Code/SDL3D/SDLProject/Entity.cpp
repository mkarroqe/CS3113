#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    rotation = glm::vec3(0);
    
    modelMatrix = glm::mat4(1.0f);
    
    speed = 0.0f;
}

void Entity::Update(float deltaTime)
{
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    if (entityType == CUBE) {
        rotation.y += 45 * deltaTime;
        rotation.z += 45 * deltaTime;
    }
    else if (entityType == ENEMY) {
        rotation.y += 30 * deltaTime;
    }
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Entity::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    mesh->Render(program);
}

