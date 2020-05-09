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

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

glm::mat4 uiViewMatrix, uiProjectionMatrix;
GLuint fontTextureID;
GLuint heartTextureID;

#define OBJECT_COUNT 5
#define ENEMY_COUNT 0

struct GameState {
    Entity *player;
    Entity *objects;
    Entity *enemies;
};

GameState state;

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
    fontTextureID = Util::LoadTexture("small_blocky.png");
//    heartTextureID = Util::LoadTexture("gravel2.jpg");
    
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
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.5f, 0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.0f;
    
    state.objects = new Entity[OBJECT_COUNT];
    
    GLuint floorTextureID = Util::LoadTexture("gravel2.jpg");
    Mesh *cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj", 20);
    
    state.objects[0].textureID = floorTextureID;
        state.objects[0].mesh = cubeMesh;
        state.objects[0].position = glm::vec3(0, -0.25f, 0);
        state.objects[0].rotation = glm::vec3(0, 0, 0);
        state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].scale = glm::vec3(20, 0.75f, 20);
        state.objects[0].entityType = FLOOR;
    
    // ---------------- FERN ----------------
    GLuint fernTextureID = Util::LoadTexture("fern_diffuse.jpg");
    Mesh *fernMesh = new Mesh();
    fernMesh->LoadOBJ("fern.obj", 1);

    state.objects[1].textureID = fernTextureID;
    state.objects[1].mesh = fernMesh;
    state.objects[1].scale = glm::vec3(0.005, 0.004f, 0.005f);
    state.objects[1].position = glm::vec3(0, 0.5, -2);
    state.objects[1].rotation = glm::vec3(0, 270, -170);
    state.objects[1].entityType = PLANT;

    // ---------------- GRASS ----------------
    GLuint grassTextureID = Util::LoadTexture("Grass.png");
    Mesh *grassMesh = new Mesh();
    grassMesh->LoadOBJ("Low Grass.obj", 1);
    
    state.objects[2].textureID = grassTextureID;
    state.objects[2].mesh = grassMesh;
    state.objects[2].scale = glm::vec3(2.0f, 2.0f, 2.0f);
    state.objects[2].position = glm::vec3(0, 0.15, -2);
    state.objects[2].entityType = PLANT;
    
    // ----------------- PALM ----------------
    GLuint palmTextureID = Util::LoadTexture("palmleaves.png");
    Mesh *palmMesh = new Mesh();
    palmMesh->LoadOBJ("Palm_01.obj", 1);
    
    state.objects[3].textureID = palmTextureID;
    state.objects[3].mesh = palmMesh;
    state.objects[3].scale = glm::vec3(0.05, 0.05f, 0.05f);
    state.objects[3].position = glm::vec3(0.25, 0.15, -2);
    state.objects[3].entityType = PLANT;
    
    // ---------------- SNAIL ----------------
    GLuint snailTextureID = Util::LoadTexture("SNAIL.png");
    Mesh *snailMesh = new Mesh();
    snailMesh->LoadOBJ("SNAIL.OBJ", 1);
    
    state.objects[4].textureID = snailTextureID;
    state.objects[4].mesh = snailMesh;
    state.objects[4].scale = glm::vec3(14.0f, 14.0f, 14.0f);
    state.objects[4].position = glm::vec3(-0.25, 0.15, -2);
    state.objects[4].rotation = glm::vec3(0, 90, 0);
    state.objects[4].entityType = PLANT;


//    state.enemies = new Entity[ENEMY_COUNT];
//
//    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
//
//    for (int i = 0; i < ENEMY_COUNT; i++) {
//        state.enemies[i].billboard = true;
//        state.enemies[i].textureID = enemyTextureID;
//        state.enemies[i].position = glm::vec3(rand() % 20 - 10, 0.5, rand() % 20 - 10);
//        state.enemies[i].rotation = glm::vec3(0, 0, 0);
//        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
//    }
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

    if (keys[SDL_SCANCODE_A]) {
        state.player->rotation.y += 1.0f;
    }
    else if (keys[SDL_SCANCODE_D]) {
        state.player->rotation.y -= 1.0f;
    }
    
    state.player->velocity.x = 0;
    state.player->velocity.z = 0;

    if (keys[SDL_SCANCODE_W]) {
        state.player->velocity.z = cos(glm::radians(state.player->rotation.y)) * -2.0f;
        state.player->velocity.x = sin(glm::radians(state.player->rotation.y)) * -2.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        state.player->velocity.z = cos(glm::radians(state.player->rotation.y)) * 2.0f;
        state.player->velocity.x = sin(glm::radians(state.player->rotation.y)) * 2.0f;
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
        state.player->Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT);
        
        for (int i = 0; i < OBJECT_COUNT; i++) {
            state.objects[i].Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT);
        }
        
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT);
        }
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix,
    glm::radians(state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::translate(viewMatrix, -state.player->position);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
//    state.player->Render(&program);
    
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Render(&program);
    }
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }
    
    // Once we are done drawing 3D objects...switch!
    program.SetProjectionMatrix(uiProjectionMatrix);
    program.SetViewMatrix(uiViewMatrix);

    Util::DrawText(&program, fontTextureID, "Lives: 3", 0.25, 0.0f, glm::vec3(-6, 3.2, 0));
    
    for (int i = 0; i < 3; i++)
    {
        // These icons are small, so just move 0.5 to the right for each one.
        Util::DrawIcon(&program, heartTextureID, glm::vec3(5 + (i * 0.5f), 3.2, 0));
    }
    
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
