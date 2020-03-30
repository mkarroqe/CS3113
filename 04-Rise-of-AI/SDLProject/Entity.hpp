#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

enum EntityType {PLAYER, PLATFORM, ENEMY};

enum AIType { WALKER, WAITANDGO, PATROL, JUMP };
enum AIState { IDLE, ACTIVE };

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float speed;
    
    float width = 1;
    float height = 1;
    
    float jump = false;
    float jumpPower = 0;
        
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    bool isActive = true;
    
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    bool defeatedEnemies = false;
    bool wasDefeated = false;
    
    Entity();
    
    bool CheckCollision(Entity *other);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckEnemyCollision(Entity *enemies, int enemyCount);
    
    void Update(float deltaTime, Entity *player, Entity *platforms, int platformCount, Entity *enemies, int enemyCount);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    
    void AI(Entity *player);
    void AIWalker();
    void AIWaitAndGo(Entity *player);
    void AIPatrol();
    void AIJump(Entity *player);
};
