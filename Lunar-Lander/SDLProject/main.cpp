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

#define PLATFORM_COUNT 13
#define OBS_COUNT 3

struct GameState {
    Entity *player;
    Entity *platforms;
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
    displayWindow = SDL_CreateWindow("Bird Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    
    // light cyan
    glClearColor(1.9f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
    // Initialize Game Objects
    
    // Initialize Bird
    state.player = new Entity();
    state.player->position = glm::vec3(0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, - 9.81f, 0);
    state.player->speed = 2.5f;
    state.player->textureID = LoadTexture("bird_lander.png");
    
    state.player->jumpPower = 5.0f;
    
    // Initialize Base Tiles
    state.platforms = new Entity[PLATFORM_COUNT + OBS_COUNT];
    GLuint basePlatformTextureID = LoadTexture("platformPack_tile002.png");
    GLuint grassPlatformTextureID = LoadTexture("platformPack_tile045.png");
    GLuint obstaclePlatformTextureID = LoadTexture("platformPack_tile039.png");
    
    float x_pos_base = -4.5f;
    float x_pos = -2.5f;
    float y_pos = 1.5f;
    
    for (int i = 0; i < PLATFORM_COUNT - OBS_COUNT; i++) {
        if (i > 5 && i < 9) {
            state.platforms[i].textureID = grassPlatformTextureID;
            state.platforms[i].position = glm::vec3(x_pos_base, -3.25, 0);
            x_pos_base += 1;
        }
        else {
            state.platforms[i].textureID = basePlatformTextureID;
            state.platforms[i].position = glm::vec3(x_pos_base, -3.25, 0);
            x_pos_base += 1;
        }
    }
    for (int i = 0; i < OBS_COUNT; i++) {
        state.platforms[i + PLATFORM_COUNT].textureID = obstaclePlatformTextureID;
        state.platforms[i + PLATFORM_COUNT].position = glm::vec3(x_pos, y_pos, 0);
        x_pos += 2.5;
        y_pos -= 1;
    }
    
    // Add Obstacle Tiles
    for (int i = PLATFORM_COUNT; i < (PLATFORM_COUNT + OBS_COUNT); i++) {
        
    }
    
    for (int i = 0; i < (PLATFORM_COUNT + OBS_COUNT); i++ ){
        state.platforms[i].Update(0, NULL, 0);
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
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
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
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
//        state.player->Update(FIXED_TIMESTEP, state.grass_platforms, GRASS_COUNT);
        
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
//    for (int i = 0; i < GRASS_COUNT; i++) {
//        state.grass_platforms[i].Render(&program);
//    }
    
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
