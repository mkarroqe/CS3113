#include "Level3.hpp"

#define LEVEL3_WIDTH 18
#define LEVEL3_HEIGHT 17

#define LEVEL3_ENEMY_COUNT 1

unsigned int level3_data[] =
{
    3, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 3,
    3, 0, 0, 2, 0, 0, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 2, 3,
    3, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3,
    3, 0, 2, 3, 3, 3, 0, 0, 0, 3, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 2, 2, 2, 3, 2, 0, 2, 0, 0, 2, 0, 3,
    3, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 3,
    3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 3, 3, 0, 3,
    3, 3, 2, 1, 0, 0, 1, 1, 0, 0, 1, 1, 3, 3, 3, 3, 0, 3,
    3, 0, 2, 2, 2, 2, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 3,
    3, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 3,
    3, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 2, 3, 3, 3, 3,
    3, 0, 0, 3, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 3, 3, 3, 3,
    3, 3, 0, 0, 3, 3, 0, 0, 2, 3, 3, 0, 0, 3, 2, 3, 3, 3,
    3, 3, 3, 0, 0, 0, 0, 3, 2, 0, 0, 0, 2, 2, 2, 3, 3, 3,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3
};


void Level3::Initialize() {
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    
    state.player->position = glm::vec3(14.8, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.5f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    state.player->height = 0.8f;
    state.player->width = 0.35f;
    
    state.player->jumpPower = 5.0f;
    
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(4, -2.25, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].isActive = false;
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    
    if ((state.player->position.x >= 10.2) && (state.player->position.y >= 0.85)) {
        state.nextScene = 4;
    }
    // falling into pit
    else if (state.player->position.y < -15.5) {
        if (state.player->lives == 0) {
            state.nextScene = 5;
        }
        else {
            state.player->lives -= 1;
        }
    }
}

void Level3::Render(ShaderProgram *program) {
    GLuint fontTextureID = Util::LoadTexture("pixel_font.png");
    Util::DrawText(program, fontTextureID, "Level", 0.8f, 0.1f, glm::vec3(12.05, 1.0, 0));
    Util::DrawText(program, fontTextureID, "3", 0.8f, 0.1f, glm::vec3(13.75, -0.1, 0));
    
    Util::DrawText(program, fontTextureID, "End!", 0.2f, 0.05f, glm::vec3(9.85, 1.05, 0));
    
    state.map->Render(program);
    state.player->Render(program);
}
