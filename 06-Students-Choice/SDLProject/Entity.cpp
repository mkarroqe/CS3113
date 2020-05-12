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
    if (xdist < -0.75 && ydist < -0.75 && zdist < -0.75) return true; // TODO: check bc this is some fudging

    return false;
}

void Entity::Update(float deltaTime, Entity *snail, Entity *player, Entity *objects, int objectCount)
{
    glm::vec3 previousPosition = position;

    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    if (entityType == SNAIL) {
        // Collisions w plants
        for (int i = 0; i < objectCount; i++) {
            // Ignore collisions with the floor
            if (objects[i].entityType == FLOOR) continue;

            if ((CheckCollision(&objects[i])) && (objects[i].entityType == PLANT)) {
                std::cout << "plant collision\n";
                
                // sliding snail behavior that adds a "time limit" on how long it is safe from betta
                position.z = previousPosition.z + 0.001;
                position.x = previousPosition.x + 0.001;
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
//        rotation.z -= 90 * deltaTime;
        
        // BETTA MOVEMENT BASED ON IF SNAIL IS NEAR A PLANT
        for (int i = 0; i < objectCount - 1; i++) {
            // if snail is not near plant, follow
            if (!(objects[i].CheckCollision(snail))) {
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
            }
            // if snail is near a plant, go the opposite way TODO: check this makes sense
            else {
                if (position.x < snail->position.x) {
                    position.x -= 1 * deltaTime;
                }
                else {
                    position.x += 1 * deltaTime;
                }
                
                if (position.z < snail->position.x) {
                    position.z -= 1 * deltaTime;
                }
                else {
                    position.z += 1 * deltaTime;
                }
            }
        }
        
        
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

