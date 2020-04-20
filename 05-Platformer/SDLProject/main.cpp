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

#include "Entity.hpp"
#include "Map.hpp"
#include "Util.hpp"
#include "Scene.hpp"

#include "Menu.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include "Level3.hpp"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene *currentScene;
Scene *sceneList[4];

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Underground", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    
    glClearColor(0.18f, 0.31f, 0.31f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    SwitchToScene(sceneList[0]);
}

void ProcessInput() {
    
    currentScene->state.player->movement = glm::vec3(0);
    
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
                    
                    case SDLK_RETURN:
                        SwitchToScene(sceneList[1]);
                        break;
                        
                    case SDLK_SPACE:
                        if (currentScene->state.player->collidedBottom) {
                            currentScene->state.player->jump = true;
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_RETURN]) {
        SwitchToScene(sceneList[1]);
    }
    
    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
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
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
    
    // ----------------- VIEW MATRIX -----------------
    viewMatrix = glm::mat4(1.0f);
    
    if (currentScene->state.player->position.y > -12.5) {
        if (currentScene->state.player->position.x > 5) {
            if (currentScene->state.player->position.x > 12) {
                if (currentScene->state.player->position.y > -2) {
                    viewMatrix = glm::translate(viewMatrix, glm::vec3(-12, 2.1, 0));
                }
                else {
                    viewMatrix = glm::translate(viewMatrix, glm::vec3(-12, -currentScene->state.player->position.y, 0));
                }
            }
            else {
                if (currentScene->state.player->position.y > -2) {
                    viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 2.1, 0));
                }
                else {
                    viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, -currentScene->state.player->position.y, 0));
                }
            }
        }
        else {
            if (currentScene->state.player->position.y > -2) {
                if (currentScene->state.player->position.x < 5) {
                    viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 2.1, 0));
                }
                else {
                    viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 2.1, 0));
                }
            }
            else {
                viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, -currentScene->state.player->position.y, 0));
            }
        }
    }
    else {
        if (currentScene->state.player->position.x < 5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 12.75, 0));
        }
        else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 12.75, 0));
        }
    }
    // ---------------- /VIEW MATRIX -----------------
    
    std::cout << "(" << currentScene->state.player->position.x
        << ", " << currentScene->state.player->position.y;
    std::cout << ")\n";
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
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
            SwitchToScene(sceneList[currentScene->state.nextScene]);
        }
        
        Render();
    }
    
    Shutdown();
    return 0;
}
