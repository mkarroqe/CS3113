#include "Entity.hpp"

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

void Entity::CheckEnemyCollision(Entity *enemies, int enemyCount) {
    for (int i = 0; i < enemyCount; i++)
    {
        Entity *enemy = &enemies[i];

        if (CheckCollision(enemy))
        {
            std::cout << "u hit an enemy " << i << " dawg\n";
            wasDefeated = true;
        }
    }
}

void Entity::AI(Entity *player) {
    switch(aiType) {
        case WALKER:
            AIWalker();
            break;
            
        case WAITANDGO:
            AIWaitAndGo(player);
            break;
            
        case PATROL:
            AIPatrol();
            break;
            
        case JUMP:
            AIJump(player);
            break;
            
    }
}

void Entity::AIWalker() {
    movement = glm::vec3(-1, 0, 0);
}

void Entity::AIWaitAndGo(Entity *player) {
    switch(aiState) {
        case IDLE:
            if (glm::distance(position, player->position) < 3.0f) {
                aiState = ACTIVE;
            }
            break;
            
        case ACTIVE:
            if (player->position.x < position.x) {
                movement = glm::vec3(-1, 0, 0);
            } else {
                movement = glm::vec3(1, 0, 0);
            }
            defeatedEnemies = true; // TODO: delete this is just a test of text

            break;
    }
}

// TODO: bounce back?
void Entity::AIPatrol() {
    switch(aiState) {
        case IDLE:
            break;
            
        case ACTIVE:
            if (position.x < 0.5) {
                movement = glm::vec3(1, 0, 0);
            }
            else {
                position.x = -4.5f;
                movement = glm::vec3(1, 0, 0);
            }
            movement = glm::vec3(1, 0, 0);
            break;
    }
}

void Entity::AIJump(Entity *player) {
    // jumps in place 
    switch(aiState) {
        case IDLE:
            velocity.y = 0;
            break;
            
        case ACTIVE:
            velocity.y += 2.5; // TODO: stopped working?
            aiState = IDLE;
            
            break;
    }
}

void Entity::Update(float deltaTime, Entity *player, Entity *platforms, int platformCount, Entity *enemies, int enemyCount)
{
    if (isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    CheckEnemyCollision(enemies, enemyCount);
    
    if (entityType == ENEMY) {
        AI(player);
    }
    
    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }
    
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime;           // Move on Y
    CheckCollisionsY(platforms, platformCount);     // Fix if needed

    position.x += velocity.x * deltaTime;           // Move on X
    CheckCollisionsX(platforms, platformCount);     // Fix if needed
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = 0;
    float v = 0;

    float width = 1.0f;
    float height = 1.0f;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

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
