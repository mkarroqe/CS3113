#include "Menu.h"

#define OBJECT_COUNT 1

void Menu::Initialize() {
    state.nextScene = -1;
    
//    state.player = new Entity();
//    state.player->entityType = PLAYER;
//    state.player->position = glm::vec3(0, 0.5f, 0);
//    state.player->speed = 1.75f;
    
//    // ------------- OBJECTS ----------------
//    state.objects = new Entity[OBJECT_COUNT];
//    // -------------- GROUND ---------------
//    GLuint floorTextureID = Util::LoadTexture("gravel2.jpg");
//    Mesh *cubeMesh = new Mesh();
//    cubeMesh->LoadOBJ("cube.obj", 20);
//
//    state.objects[0].textureID = floorTextureID;
//    state.objects[0].mesh = cubeMesh;
//    state.objects[0].position = glm::vec3(0, -0.25f, 0);
//    state.objects[0].rotation = glm::vec3(0, 0, 0);
//    state.objects[0].acceleration = glm::vec3(0, 0, 0);
//    state.objects[0].scale = glm::vec3(20, 0.75f, 20);
//    state.objects[0].entityType = FLOOR;
}

void Menu::Update(float deltaTime) {
//    state.player->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
//
//    for (int i = 0; i < OBJECT_COUNT; i++) {
//        state.objects[i].Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
//    }
    
//    if (state.next) {
//        state.nextScene = 1;
//    }
}

void Menu::Render(ShaderProgram *program, ShaderProgram *programUI) {
    GLuint fontTextureID = Util::LoadTexture("small_blocky.png");

    Util::DrawText(programUI, fontTextureID, "Save Roomba", 0.3, 0.0f, glm::vec3(-1.65, 1.5, 0));
    
    Util::DrawText(programUI, fontTextureID, "(the snail)", 0.15, 0.0f, glm::vec3(-0.85, 1.0, 0));
    
    Util::DrawText(programUI, fontTextureID, "Press Enter to Start", 0.2, 0.0f, glm::vec3(-2.05, 0.5, 0));
}
