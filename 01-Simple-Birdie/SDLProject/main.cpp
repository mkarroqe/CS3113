// Simple Birdie: A Simple 2D Scene
// by: Mary Karroqe
// Code adapted from CS3113 Class Template + Triangle Example

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

#include <iostream>
using namespace std;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
// mama birdie
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

// baby birdies
glm::mat4 modelMatrix1, modelMatrix2, modelMatrix3;

float player_x = 0;
float player_y = 0;
float player_s = 0;
float player_rotate = 0;

GLuint birdieTextureID;
GLuint babyTextureID;

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Birdie Begs Bouncing Babies to Follow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    glClearColor(0.878f, 1.0f, 1.0f, 1.0f);
    
    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    birdieTextureID = LoadTexture("birdie.png");
    babyTextureID = LoadTexture("baby_birdie.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

// helper function used for debugging!
void printMatrix(glm::mat4 aMatrix) {
    cout << aMatrix[0][0] << ' ' << aMatrix[0][1] << ' ' << aMatrix[0][2] << ' ' << aMatrix[0][3] << '\n';
    cout << aMatrix[1][0] << ' ' << aMatrix[1][1] << ' ' << aMatrix[1][2] << ' ' << aMatrix[1][3] << '\n';
    cout << aMatrix[2][0] << ' ' << aMatrix[2][1] << ' ' << aMatrix[2][2] << ' ' << aMatrix[2][3] << '\n';
    cout << aMatrix[3][0] << ' ' << aMatrix[3][1] << ' ' << aMatrix[3][2] << ' ' << aMatrix[3][3] << '\n';
    cout << '\n';
}

// helper function: considers fmod result == 0 at thousandth place
bool isModZero(float x, float y) {
    float mod = fmod(x, y);
    return (mod * 1000) < 0;
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    player_x += 0.5f * deltaTime;
    player_y += 0.5f * deltaTime;
    player_rotate += 90.0f * deltaTime;
    player_s += 1.01 * deltaTime;
    
    // ---------------------- mama birdie ----------------------
    // initial position
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 4, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(sin(1.4*player_x), 0.0f, 0.0f));
    
//    // spin to enter animation!
//    if (player_s < 4) {
//        modelMatrix = glm::scale(modelMatrix, glm::vec3(player_s, player_s, 1.0f));
//    } else {
//        modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 4, 1.0f));
//        modelMatrix = glm::translate(modelMatrix, glm::vec3(sin(1.4*player_x), 0.0f, 0.0f));
//    }
//    modelMatrix = glm::rotate(modelMatrix, glm::radians(player_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // --------------------- baby birdies ----------------------
    modelMatrix1 = glm::mat4(1.0f);
    modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(2.0f, -1.25f, 0.0f));
    modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(2.0f, sin(20*player_x), 0.0f));
    
    modelMatrix2 = glm::mat4(1.0f);
    modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(1.0f, -1.0f, 0.0f));
    modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(1.0f, sin(15*player_x), 0.0f));
    
    modelMatrix3 = glm::mat4(1.0f);
    modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(1.5f, -1.25f, 0.0f));
    modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(1.5f, sin(10*player_x), 0.0f));
    modelMatrix3 = glm::rotate(modelMatrix3, glm::radians(player_rotate), glm::vec3(0.0f, 0.0f, 1.0f));

}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    // ---------------------- mama birdie ----------------------
    program.SetModelMatrix(modelMatrix); // mama birdie
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, birdieTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    // --------------------- /mama birdie ----------------------
        
    // -------------------- baby birdie 1 ----------------------
    program.SetModelMatrix(modelMatrix1);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, babyTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    // ------------------- /baby birdie 1 ----------------------
    
    // -------------------- baby birdie 2 ----------------------
    program.SetModelMatrix(modelMatrix2);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, babyTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    // ------------------- /baby birdie 2 ----------------------
    
    // -------------------- baby birdie 3 ----------------------
    program.SetModelMatrix(modelMatrix3);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, babyTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    // ------------------- /baby birdie 3 ----------------------
    
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
