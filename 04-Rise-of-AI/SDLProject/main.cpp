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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.hpp"

#define PLATFORM_COUNT 25
#define ENEMY_COUNT 3

struct GameState {
    Entity *player;
    Entity *platforms;
    Entity *enemies;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("An Unlikely Hero", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // -----------------------------------------------------------------
    //                      Initialize Game Objects
    // --------------------------- PLAYER ------------------------------
    
    state.player = new Entity();
    state.player->entityType = PLAYER;

    GLuint playerTextureID = LoadTexture("tp.png");
    state.player->textureID = playerTextureID;
    state.player->height = 1.0f;
    state.player->width = 1.0f;

    state.player->position = glm::vec3(-4, -1, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.85f;
    state.player->jumpPower = 5.2f;
    
    // --------------------------- ENEMIES -----------------------------
    state.enemies = new Entity[ENEMY_COUNT];
    
    GLuint enemyTextureID;
    for (int i = 0; i < ENEMY_COUNT; i ++) {
        state.enemies[i].entityType = ENEMY;
        
        if (i == 0) {
            state.enemies[i].position = glm::vec3(4, 1.75f, 0);
            state.enemies[i].aiType = WAITANDGO;
            state.enemies[i].aiState = IDLE;
            enemyTextureID = LoadTexture("virus1.png");
        }
        else if (i == 1) {
            state.enemies[i].position = glm::vec3(-2.5, 2.75f, 0);
            state.enemies[i].aiType = PATROL;
            state.enemies[i].aiState = ACTIVE;
            enemyTextureID = LoadTexture("virus3.png");
        }
        else {
            state.enemies[i].position = glm::vec3(-0.5f, -1.25f, 0);
            state.enemies[i].aiType = JUMP;
            state.enemies[i].aiState = IDLE;
            enemyTextureID = LoadTexture("virus2.png");
        }
        
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].height = 1.0f;
        state.enemies[i].width = 1.0f;
        
        state.enemies[i].movement = glm::vec3(0);
        state.enemies[i].acceleration = glm::vec3(0, -9.81f, 0);
        state.enemies[i].speed = 1;
    }
    
    // -------------------------- PLATFORMS ----------------------------
    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("platformPack_tile030.png");
    
    int x = 0;
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platformTextureID;
        
        if (i < 10) {
            state.platforms[i].position = glm::vec3(-4.5 + i, -3.25, 0);
        }
        else if (i < 14) {
            x = i - 4;
            state.platforms[i].position = glm::vec3(-4.5 + x, -2.25, 0);
        }
        else if (i < 17) {
            x = i - 7;
            state.platforms[i].position = glm::vec3(-4.5 + x, -1.25, 0);
        }
        else if (i < 19) {
            x = i - 9;
            state.platforms[i].position = glm::vec3(-4.5 + x, -0.25, 0);
        }
        else {
            x = i - 19;
            state.platforms[i].position = glm::vec3(-4.5 + x, 0.75, 0);
        }
        
        state.platforms[i].Update(0, NULL, NULL, 0);
    }
}

void ProcessInput() {
    state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
                        if (state.player->collidedBottom) {
                            state.player->jump = true;
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
    }
    

    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT);
        
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, ENEMY_COUNT);
        }
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }
    
    state.player->Render(&program);
    
    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
