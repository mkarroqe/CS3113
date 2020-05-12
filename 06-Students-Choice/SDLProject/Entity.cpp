#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    rotation = glm::vec3(0);
    scale = glm::vec3(1);
    
    modelMatrix = glm::mat4(1.0f);
    
    speed = 0.0f;
    
    billboard = false;
    
    width = 1.0f;
    height = 1.0f;
    depth = 1.0f;
}

bool Entity::CheckCollision(Entity *other)
{
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    float zdist = fabs(position.z - other->position.z) - ((depth + other->depth) / 2.0f);
    if (xdist < 0 && ydist < 0 && zdist < 0) return true;

    return false;
}

void Entity::Update(float deltaTime, Entity *snail, Entity *player, Entity *objects, int objectCount)
{
    glm::vec3 previousPosition = position;
    
    if (billboard) {
        float directionX = position.x - player->position.x;
        float directionZ = position.z - player->position.z;
        rotation.y = glm::degrees(atan2f(directionX, directionZ));
        
        // come towards us
        velocity.z = cos(glm::radians(rotation.y)) * -1.0f;
        velocity.x = sin(glm::radians(rotation.y)) * -1.0f;
    }

    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    if (entityType == SNAIL) {
        // Collisions w plants
        for (int i = 0; i < objectCount; i++) {
            // Ignore collisions with the floor
            if (objects[i].entityType == FLOOR) continue;

            if (CheckCollision(&objects[i])) {
                position = previousPosition;
                break;
            }
        }
    }
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
    if (entityType == ENEMY) {
        rotation.z -= 90 * deltaTime;
        
        if (position.x < snail->position.x) {
            position.x += 1 * deltaTime;
        }
        else {
            position.x -= 1 * deltaTime;
        }
        
        if (position.z < snail->position.x) {
            position.z += 1 * deltaTime;
        }
        else {
            position.z -= 1 * deltaTime;
        }
//        if (position.x < 1) {
//            position.x += 0.01 * deltaTime;
//            position.z += 0.01 * deltaTime;
//        }
//        else {
//            position.x -= 0.01 * deltaTime;
//            position.z -= 0.01 * deltaTime;
//        }
        
//        modelMatrix = glm::translate(modelMatrix, glm::vec3(sin(0.5 * position.y), 0.1, 0.1));
    }
}

void Entity::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
        
    glBindTexture(GL_TEXTURE_2D, textureID);

    mesh->Render(program);
}

void Entity::DrawBillboard(ShaderProgram *program) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

