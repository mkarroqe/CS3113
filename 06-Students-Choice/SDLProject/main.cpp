#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Scene.h"

#include "Level.h"
#include "Menu.h"

#define OBJECT_COUNT 11
#define ENEMY_COUNT 1

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

glm::mat4 uiViewMatrix, uiProjectionMatrix;
GLuint fontTextureID;
GLuint heartTextureID;

Scene *currentScene;
Scene *sceneList[2];

void SwitchToScene(int _nextScene, int _lives=3) {
    if (_nextScene == 1) {
        currentScene = new Level(_lives);
    }
    
    // if win (4) or lose (5)
    else {
        currentScene = sceneList[_nextScene];
    }
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Save Roomba", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
//    fontTextureID = Util::LoadTexture("small_blocky.png");
////    heartTextureID = Util::LoadTexture("gravel2.jpg");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 100.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(0.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    
    sceneList[0] = new Menu();
    sceneList[1] = new Level(3);
    SwitchToScene(0);
    
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        // Some sort of action
                        break;
                        
                }
                break;
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    // ----------------- START -------------------
    if (keys[SDL_SCANCODE_RETURN]) {
        currentScene->state.player_lives = 3;
        currentScene->state.next = true;
        SwitchToScene(1);
    }
    
    // -------------- PLAYER VIEW ----------------
    if (keys[SDL_SCANCODE_A]) {
        currentScene->state.player->rotation.y += 1.0f;
    }
    else if (keys[SDL_SCANCODE_D]) {
        currentScene->state.player->rotation.y -= 1.0f;
    }
    
    // test
    if (currentScene == sceneList[1]) {
        currentScene->state.player->velocity.x = 0;
        currentScene->state.player->velocity.z = 0;
    }
    
    if (keys[SDL_SCANCODE_W]) {
        currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * -2.0f;
        currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * -2.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * 2.0f;
        currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * 2.0f;
    }
    // --------------- SNAIL MOVES ----------------
    int snail_num = OBJECT_COUNT - 1;
    
    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.objects[snail_num].position.x -= 0.01f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.objects[snail_num].position.x += 0.01f;
    }
    
    if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.objects[snail_num].rotation.y -= 2.75;
        currentScene->state.objects[snail_num].position.z -= 0.01f;
        
//        // roll
//        state.objects[snail_num].rotation.x -= 25;
//        state.objects[snail_num].position.z -= 0.01f;
    
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.objects[snail_num].rotation.y += 2.75;
        currentScene->state.objects[snail_num].position.z += 0.01f;
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
        currentScene->state.player->Update(FIXED_TIMESTEP, currentScene->state.player, currentScene->state.objects, OBJECT_COUNT);
        
//        for (int i = 0; i < OBJECT_COUNT; i++) {
//            currentScene->state.objects[i].Update(FIXED_TIMESTEP, currentScene->state.player, currentScene->state.objects, OBJECT_COUNT);
//        }
//
//        for (int i = 0; i < ENEMY_COUNT; i++) {
//            currentScene->state.enemies[i].Update(FIXED_TIMESTEP, currentScene->state.player, currentScene->state.objects, OBJECT_COUNT);
//        }
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix,
    glm::radians(currentScene->state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::translate(viewMatrix, -currentScene->state.player->position);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
//    currentScene->state.player->Render(&program);
    
//    for (int i = 0; i < OBJECT_COUNT; i++) {
//        currentScene->state.objects[i].Render(&program);
//    }
//
//    for (int i = 0; i < ENEMY_COUNT; i++) {
//        currentScene->state.enemies[i].Render(&program);
//    }
    
    // Once we are done drawing 3D objects...switch!
//    program.SetProjectionMatrix(uiProjectionMatrix);
//    program.SetViewMatrix(uiViewMatrix);
    
//    currentScene->Render(&program);

//    Util::DrawText(&program, fontTextureID, "Lives: 3", 0.25, 0.0f, glm::vec3(-6, 3.2, 0));
//    
//    for (int i = 0; i < 3; i++)
//    {
//        // These icons are small, so just move 0.5 to the right for each one.
//        Util::DrawIcon(&program, heartTextureID, glm::vec3(5 + (i * 0.5f), 3.2, 0));
//    }
    
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
        
        if (currentScene->state.nextScene >= 0) {
//            if (currentScene->state.player_lives == 0) {
//                SwitchToScene(5);
//            }
//            // if next is less than curr initialize w less lives?
//            else {
                SwitchToScene(currentScene->state.nextScene, currentScene->state.player_lives);
//            }
        }
        
        Render();
    }
    
    Shutdown();
    return 0;
}
