#include "Level.h"

#define OBJECT_COUNT 11
#define ENEMY_COUNT 1

Level::Level(int _lives) {
    state.player_lives = _lives;
}

void Level::Initialize() {
    state.nextScene = -1;
    state.next = false;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.5f, 0);
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
    
    // ---------------- FERN ----------------
    GLuint fernTextureID = Util::LoadTexture("fern_diffuse.jpg");
    Mesh *fernMesh = new Mesh();
    fernMesh->LoadOBJ("fern.obj", 1);
    
    int num_ferns = 3;
    for (int i = 0; i < num_ferns; i++) {
        int num = 1 + i;
        state.objects[num].textureID = fernTextureID;
        state.objects[num].mesh = fernMesh;
        state.objects[num].scale = glm::vec3(0.005, 0.004f, 0.005f);
        state.objects[num].position = glm::vec3(rand() % 20 - 10, 0.15, rand() % 20 - 10);//glm::vec3(0, 0.15, -2);
        state.objects[num].rotation = glm::vec3(270, 0, 0);
        state.objects[num].entityType = PLANT;
    }

    // ---------------- GRASS ----------------
    GLuint grassTextureID = Util::LoadTexture("Grass.png");
    Mesh *grassMesh = new Mesh();
    grassMesh->LoadOBJ("Low Grass.obj", 1);

    int num_grass = 3;
    for (int i = 0; i < num_grass; i++) {
        int num = 1 + num_ferns + i;
        state.objects[num].textureID = grassTextureID;
        state.objects[num].mesh = grassMesh;
        state.objects[num].scale = glm::vec3(2.0f, 2.0f, 2.0f);
        state.objects[num].position = glm::vec3(rand() % 20 - 10, 0.15, rand() % 20 - 10);//glm::vec3(0, 0.15, -2);
        state.objects[num].entityType = PLANT;
    }

    // ----------------- PALM ----------------
    GLuint palmTextureID = Util::LoadTexture("palmleaves.png");
    Mesh *palmMesh = new Mesh();
    palmMesh->LoadOBJ("Palm_01.obj", 1);

    int num_palms = 3;
    for (int i = 0; i < num_palms; i++) {
        int num = 1 + num_ferns + num_grass + i;
        state.objects[num].textureID = palmTextureID;
        state.objects[num].mesh = palmMesh;
        state.objects[num].scale = glm::vec3(0.05, 0.05f, 0.05f);
        state.objects[num].position = glm::vec3(rand() % 20 - 10, 0.15, rand() % 20 - 10);//glm::vec3(0.25, 0.15, -2);
        state.objects[num].entityType = PLANT;
    }
    
    // ---------------- SNAIL ----------------
    GLuint snailTextureID = Util::LoadTexture("SNAIL.png");
    Mesh *snailMesh = new Mesh();
    snailMesh->LoadOBJ("SNAIL.OBJ", 1);
    
    int snail_num = num_ferns + num_grass + num_palms + 1;
    state.objects[snail_num].textureID = snailTextureID;
    state.objects[snail_num].mesh = snailMesh;
    state.objects[snail_num].scale = glm::vec3(14.0f, 14.0f, 14.0f);
    state.objects[snail_num].position = glm::vec3(-0.25, 0.19, -2);
    state.objects[snail_num].rotation = glm::vec3(0, 90, 0);
    state.objects[snail_num].entityType = SNAIL;
    
    // --------------- ENEMIES ----------------
    state.enemies = new Entity[ENEMY_COUNT];
    // ---------------- BETTA ----------------
    GLuint bettaTextureID = Util::LoadTexture("betta.png");
    Mesh *bettaMesh = new Mesh();
    bettaMesh->LoadOBJ("betta.obj", 1);
    
    state.enemies[0].textureID = bettaTextureID;
    state.enemies[0].mesh = bettaMesh;
    state.enemies[0].scale = glm::vec3(0.45f, 0.45f, 0.45f);
    state.enemies[0].position = glm::vec3(-0.5, 0.5, -2);
    state.enemies[0].rotation = glm::vec3(270, 0, 0);
    state.enemies[0].entityType = ENEMY;
}

void Level::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
    
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
    }
}

void Level::Render(ShaderProgram *program, ShaderProgram *programUI) {
    // Text
    GLuint fontTextureID = Util::LoadTexture("small_blocky.png");

    Util::DrawText(programUI, fontTextureID, "Lives: 3", 0.25, 0.0f, glm::vec3(-6, 3.2, 0));
}
