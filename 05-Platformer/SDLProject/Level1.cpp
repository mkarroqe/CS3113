#include "Level1.hpp"

#define LEVEL1_WIDTH 18
#define LEVEL1_HEIGHT 17

#define LEVEL1_ENEMY_COUNT 2

unsigned int level1_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 2, 0, 3,
    3, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 3, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 3, 3, 3, 0, 3,
    3, 0, 0, 0, 2, 2, 0, 0, 2, 2, 3, 0, 0, 0, 0, 0, 0, 3,
    3, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 3, 0, 0, 3,
    3, 0, 0, 0, 3, 3, 0, 0, 2, 0, 0, 0, 2, 3, 0, 0, 0, 3,
    3, 3, 2, 2, 3, 0, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 0, 3
};

Level1::Level1(int _lives) {
    state.player_lives = _lives;
}

void Level1::Initialize() {
    
    state.nextScene = -10;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    
    state.player->position = glm::vec3(6, 0, 0);
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
    
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    state.enemies[0].position = glm::vec3(2.5, -14, 0);
    state.enemies[0].speed = 1;
    
    state.enemies[0].height = 0.8f;
    state.enemies[0].width = 0.35f;
    
    state.enemies[0].aiType = WALKER;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = Util::LoadTexture("ai.png");
    
    state.enemies[0].isActive = true;
}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    
    std::cout << "Enemy: (" << state.enemies[0].position.x << ", ";
    std::cout << state.enemies[0].position.y << ") \n";
    
    std::cout << "Lives: " << state.player_lives << "\n";
    
    if(state.player->CheckCollision(&state.enemies[0])) {
        std::cout << "ah!\n";
        loseLife();
        state.nextScene = 1;
    }
    if(state.enemies[0].collidedTop) {
        std::cout << "yuh\n";
        state.enemies[0].isActive = false;
    }
    
    if (state.player->position.x >= 15.85) {
        state.nextScene = 2;
    }
    // falling into pit
    else if (state.player->position.y < -15.5) {
        if (state.player->lives == 0) {
            state.nextScene = 5;
        }
        else {
            loseLife();
            state.nextScene = 1;
        }
    }
}

void Level1::Render(ShaderProgram *program) {
    GLuint fontTextureID = Util::LoadTexture("pixel_font.png");
    Util::DrawText(program, fontTextureID, "Level 1", 0.8f, 0.1f, glm::vec3(2, -13, 0));
    
    Util::DrawText(program, fontTextureID, "Up!", 0.2f, 0.05f, glm::vec3(8.7, -10, 0));
    Util::DrawText(program, fontTextureID, "Up!", 0.2f, 0.05f, glm::vec3(8.7, -11, 0));
    Util::DrawText(program, fontTextureID, "Up!", 0.2f, 0.05f, glm::vec3(8.7, -12, 0));
    
    Util::DrawText(program, fontTextureID, "Climb!", 0.2f, 0.05f, glm::vec3(2.0, -4.8, 0));
    
    Util::DrawText(program, fontTextureID, "Finish..?", 0.2f, 0.1f, glm::vec3(13, 0.5, 0));
    
    // formatting lives
    std::string lives_str = std::to_string(state.player_lives);
    
    // thanks https://stackoverflow.com/a/58972804 for tip
    std::string rounded = lives_str.substr(0, lives_str.find(".")+0);
    
    std::string lives_left = "Lives: " + rounded;
    
    Util::DrawText(program, fontTextureID, lives_left, 0.2f, 0.1f, glm::vec3(3, 1.0, 0));
    
    state.map->Render(program);
    state.player->Render(program);
}
