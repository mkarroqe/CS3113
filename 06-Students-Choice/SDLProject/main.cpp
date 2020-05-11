#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>
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

ShaderProgram programUI;
glm::mat4 uiViewMatrix, uiProjectionMatrix;
GLuint fontTextureID;

Scene *currentScene;
Scene *sceneList[2];

Mix_Music *music;
Mix_Chunk *sploosh;

void SwitchToScene(int _nextScene, int _lives=3) {
//    if (_nextScene == 1) {
//        currentScene = new Level(_lives);
//    }
//    else {
        currentScene = sceneList[_nextScene];
//    }
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
    
    // ----------------- MUSIC -----------------
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("mary samba loop.mp3");
    Mix_PlayMusic(music, -1);
    
    sploosh = Mix_LoadWAV("mary fish sploosh.wav");
    // ---------------- /MUSIC -----------------
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 100.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(0.0f, 1.0f, 1.0f, 1.0f);
    
    programUI.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
    programUI.SetProjectionMatrix(uiProjectionMatrix);
    programUI.SetViewMatrix(uiViewMatrix);
//    programUI.SetColor(0.0f, 0.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    
    glClearColor(0.0f, 0.8f, 1.0f, 1.0f);
    
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
                    case SDLK_w:
                        break;
                    case SDLK_s:
                        break;
                    case SDLK_a:
                        break;
                    case SDLK_d:
                        break;
                        
                    case SDLK_UP:
                        break;
                    case SDLK_DOWN:
                        break;
                    case SDLK_LEFT:
                        break;
                    case SDLK_RIGHT:
                        break;
                    case SDLK_RSHIFT:
                        break;
                    case SDLK_LSHIFT:
                        break;
                        
                    case SDLK_RETURN:
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    // ----------------- START -------------------
    if (keys[SDL_SCANCODE_RETURN]) {
        if (currentScene == sceneList[0]) {
            currentScene->state.player_lives = 3;
            currentScene->state.transitioning = true;
            
            Mix_PlayChannel(-1, sploosh, 0);
        }
    }
    
    // For level.. not most graceful implementation but it WORKS
    if (currentScene == sceneList[1]) {
        // -------------- PLAYER VIEW ----------------
        if (keys[SDL_SCANCODE_A]) {
            currentScene->state.player->rotation.y += 1.0f;
            std::cout << "you pressed A\n";
        }
        else if (keys[SDL_SCANCODE_D]) {
            currentScene->state.player->rotation.y -= 1.0f;
            std::cout << "you pressed D\n";
        }
        
        currentScene->state.player->velocity.x = 0;
        currentScene->state.player->velocity.z = 0;
        
        if (keys[SDL_SCANCODE_W]) {
            currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * -2.0f;
            
            currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * -2.0f;
            
            std::cout << "you pressed W\n";
        }
        else if (keys[SDL_SCANCODE_S]) {
            
            currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * 2.0f;
            
            currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * 2.0f;
            
            std::cout << "you pressed S\n";
        }
    
        // --------------- SNAIL MOVES ----------------
        int snail_num = OBJECT_COUNT - 1;
        float curr_x_pos = currentScene->state.objects[snail_num].position.x;
        float curr_y_pos = currentScene->state.objects[snail_num].position.y;
        float curr_z_pos = currentScene->state.objects[snail_num].position.z;
        
        std::cout << "pos is (" << curr_x_pos << ", "
                                << curr_y_pos << ", "
                                << curr_z_pos << ")\n";
    
    
        if (keys[SDL_SCANCODE_LEFT]) {
            // TODO: make work while on wall
    //        if (curr_y_pos < -0.4) {
                currentScene->state.objects[snail_num].rotation.y = 270;
                currentScene->state.objects[snail_num].position.x -= 0.01f;
    //        }
    //
    //        // back wall
    //        else {
    //            currentScene->state.objects[snail_num].rotation.y = 270;
    //            currentScene->state.objects[snail_num].position.x -= 0.01f;
    //        }
            
            // front wall
            
            // right wall
            
            // left wall
            
            std::cout << "you pressed LEFT\n";
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            // TODO: make work while on wall
            currentScene->state.objects[snail_num].rotation.y = 90;
            currentScene->state.objects[snail_num].position.x += 0.01f;
            
    //        // back wall
    //        else if (curr_z_pos > 3.5){
    //            currentScene->state.objects[snail_num].rotation.y = 270;
    //            currentScene->state.objects[snail_num].position.x -= 0.01f;
    //        }
    //
    //        // front wall
    //        else if (curr_z_pos < 1.3){
    //            currentScene->state.objects[snail_num].rotation.y = 270;
    //            currentScene->state.objects[snail_num].position.x -= 0.01f;
    //        }
    //
    //        // right wall
    //        else if (curr_x_pos > 1.3){
    //            currentScene->state.objects[snail_num].rotation.y = 270;
    //            currentScene->state.objects[snail_num].position.x -= 0.01f;
    //        }
    //
    //        // left wall
    //        else if (curr_x_pos < -1.3){
    //            currentScene->state.objects[snail_num].rotation.y = 270;
    //            currentScene->state.objects[snail_num].position.x -= 0.01f;
    //        }
            
            std::cout << "you pressed RIGHT\n";
        }
        
        if (keys[SDL_SCANCODE_UP]) {
            if (curr_y_pos < 0) {
                currentScene->state.objects[snail_num].rotation.y = -180;
                currentScene->state.objects[snail_num].position.z -= 0.01f;
            }
            
            // on wall
            else {
                currentScene->state.objects[snail_num].rotation.y = -180;
            }
            
            std::cout << "you pressed UP\n";
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            if (curr_y_pos < 0) {
                currentScene->state.objects[snail_num].rotation.y = -360;
                currentScene->state.objects[snail_num].position.z += 0.01f;
            }
            
            // on wall
            else {
                currentScene->state.objects[snail_num].rotation.y = -360;
            }
            
            std::cout << "you pressed DOWN\n";
        }
    
                                                    
    // --------------- WALL CLIMBING --------------
        if (keys[SDL_SCANCODE_RSHIFT]) {
            if (curr_y_pos >= -0.051) {
                currentScene->state.objects[snail_num].position.y += 0.02f;
                
                // Climb up front wall
                if (curr_z_pos > 1.3) {//> 7) {
                    currentScene->state.objects[snail_num].rotation.x = -90;
                    currentScene->state.objects[snail_num].rotation.y = 360;
                }

                // Climb up back wall
                else if (curr_z_pos < 3.5) {// < -7) {
                    currentScene->state.objects[snail_num].rotation.x = 90;
                    currentScene->state.objects[snail_num].rotation.y = 180;
                }
                
                // Climb up right wall
                else if (curr_x_pos > 1.3) { //> 5) {
                    currentScene->state.objects[snail_num].rotation.x = 90;
                    currentScene->state.objects[snail_num].rotation.y = 180;
                    currentScene->state.objects[snail_num].rotation.z = -90;
                }
                
                // Climb up left wall
                else if (curr_x_pos > -1.3) { //> 5) {
                    currentScene->state.objects[snail_num].rotation.x = 90;
                    currentScene->state.objects[snail_num].rotation.y = 180;
                    currentScene->state.objects[snail_num].rotation.z = 90;
                }
            }
            else {
                currentScene->state.objects[snail_num].position.y = -0.049f;
                currentScene->state.objects[snail_num].rotation = glm::vec3(0, 90, 0);
            }
            
            std::cout << "you pressed RSHIFT at y pos: " << curr_y_pos << "\n";
        }
        else if (keys[SDL_SCANCODE_LSHIFT]) {
            if (curr_y_pos >= -0.051) {
                currentScene->state.objects[snail_num].position.y -= 0.01f;
                
                // Climb down front wall
    //            if (curr_x_pos > 1.3) {
                    currentScene->state.objects[snail_num].rotation.x = -90;
                    currentScene->state.objects[snail_num].rotation.y = -180;
    //            }
                
    //            // Climb down back wall
    //            else if (curr_x_pos > 5) {
    //                currentScene->state.objects[snail_num].rotation.x = 90;
    //                currentScene->state.objects[snail_num].rotation.y = -360;
    //            }
    //
    //            // Climb down right wall
    //            else if (curr_x_pos > 5) {
    //                currentScene->state.objects[snail_num].rotation.x = 90;
    //                currentScene->state.objects[snail_num].rotation.y = -360;
    //                currentScene->state.objects[snail_num].rotation.z = 90;
    //            }
    //
    //            // Climb down left wall
    //            else if (curr_x_pos > 5) {
    //                currentScene->state.objects[snail_num].rotation.x = 90;
    //                currentScene->state.objects[snail_num].rotation.y = -360;
    //                currentScene->state.objects[snail_num].rotation.z = -90;
    //            }
    //        }
    //        else {
    //            currentScene->state.objects[snail_num].position.y = -0.049f;
    //            currentScene->state.objects[snail_num].rotation = glm::vec3(0, 90, 0);
            }
            
            std::cout << "you pressed LSHIFT at y pos: " << curr_y_pos << "\n";
        }
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
    
    if (currentScene == sceneList[1]) {
        while (deltaTime >= FIXED_TIMESTEP) {
            currentScene->state.player->Update(FIXED_TIMESTEP, currentScene->state.player, currentScene->state.objects, OBJECT_COUNT);
            
            for (int i = 0; i < OBJECT_COUNT; i++) {
                currentScene->state.objects[i].Update(FIXED_TIMESTEP, currentScene->state.player, currentScene->state.objects, OBJECT_COUNT);
            }
            
            for (int i = 0; i < ENEMY_COUNT; i++) {
                currentScene->state.enemies[i].Update(FIXED_TIMESTEP, currentScene->state.player, currentScene->state.objects, OBJECT_COUNT);
            }
            
            deltaTime -= FIXED_TIMESTEP;
        }
    
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix,
    glm::radians(currentScene->state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::translate(viewMatrix, -currentScene->state.player->position);
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    currentScene->Render(&program, &programUI);
    
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
        
        // pls
        if (currentScene->state.transitioning) {
            SwitchToScene(1);
        }
        
//        if (currentScene->state.nextScene >= 0) {
//            SwitchToScene(currentScene->state.nextScene, currentScene->state.player_lives);
//        }
    }
    
    Shutdown();
    return 0;
}
