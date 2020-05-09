#include "Menu.h"

#define OBJECT_COUNT 1

void Menu::Initialize() {
    state.nextScene = -10;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.5f, 0);
//    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.75f;
    
    // ------------- OBJECTS ----------------
    state.objects = new Entity[OBJECT_COUNT];
    // -------------- GROUND ---------------
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
    
}

void Menu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
    
    if (state.next) {
        state.nextScene = 1;
    }
}

void Menu::Render(ShaderProgram *program) {
    state.player->Render(program);
}
