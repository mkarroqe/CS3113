#include "Entity.hpp"
#include <vector>

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other) {
    if (isActive == false || other->isActive == false) return false;
    
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0);
    
    if (xdist < 0 && ydist < 0) return true;
    
    return false;
}

bool Entity::CheckCollisionGrass(Entity* grassTile) {
    if (CheckCollision(grassTile)) {
        return true;
    }
    return false;
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity *object = &objects[i];

        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity *object = &objects[i];

        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true; // something here for future assingment
            }
        }
    }
}

void Entity::Update(float deltaTime, Entity *platforms, int platformCount)
{
    if (isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    collidedGrass = false;
    
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime;           // Move on Y
    CheckCollisionsY(platforms, platformCount);     // Fix if needed

    position.x += velocity.x * deltaTime;           // Move on X
    CheckCollisionsX(platforms, platformCount);     // Fix if needed
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

//void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
//{
////    float u = (float)(index % animCols) / (float)animCols;
////    float v = (float)(index / animCols) / (float)animRows;
////
////    float width = 1.0f / (float)animCols;
////    float height = 1.0f / (float)animRows;
//    
//    float u = 1.0f;
//    float v = 1.0f;
//    float width = 1.0f;
//    float height = 1.0f;
//    
//    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
//        u, v + height, u + width, v, u, v};
//    
//    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
//    
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    
//    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
//    glEnableVertexAttribArray(program->positionAttribute);
//    
//    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
//    glEnableVertexAttribArray(program->texCoordAttribute);
//    
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    
//    glDisableVertexAttribArray(program->positionAttribute);
//    glDisableVertexAttribArray(program->texCoordAttribute);
//}



void Entity::Render(ShaderProgram *program) {
    if (isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
