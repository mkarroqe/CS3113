// Pong!
// by: Mary Karroqe
// Code adapted from CS3113 Class Template + Examples

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

// ------------------- INITIALIZATION OF ONSCREEN ITEMS -------------------
// Initialize Matricies for paddles + ball
glm::mat4 viewMatrix, projectionMatrix;
glm::mat4 modelMatrix_p1, modelMatrix_p2, modelMatrix_ball;

// Ball Initalization
glm::vec3 ball_position = glm::vec3(0, 0, 0); // Start at 0, 0, 0
glm::vec3 ball_movement = glm::vec3(0, 0, 0); // Don't go anywhere (yet)
glm::vec3 ball_scale = glm::vec3(0.35f, 0.35f, 1.0f);
float ball_speed = 3.0f;
float ball_rotate = 1.0f;
bool ball_path_reversed = false;

// Paddle 1 Initalization
glm::vec3 p1_position = glm::vec3(0, 0, 0); // Start at 0, 0, 0
glm::vec3 p1_movement = glm::vec3(0, 0, 0); // Don't go anywhere (yet)
float p1_speed = 3.0f;

// Paddle 2 Initalization
glm::vec3 p2_position = glm::vec3(0, 0, 0); // Start at 0, 0, 0
glm::vec3 p2_movement = glm::vec3(0, 0, 0); // Don't go anywhere (yet)
float p2_speed = 3.0f;

// Both Paddles
glm::vec3 p_scale = glm::vec3(0.35f, 2.55f, 1.0f);

// ------------------ /INITIALIZATION OF ONSCREEN ITEMS -------------------

// ------------------------------- TEXTURES -------------------------------
GLuint paddleTextureID;
GLuint ballTextureID;

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
// ------------------------------ /TEXTURES -------------------------------

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix_ball = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    // black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    paddleTextureID = LoadTexture("paddle.png");
    ballTextureID = LoadTexture("ball.png");
}

void ProcessInput() {
    p1_movement = glm::vec3(0); // so that if nothing is pressed, we don't go anywhere
    p2_movement = glm::vec3(0);
    
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
                        // Some sort of action
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    // Ball
    if (keys[SDL_SCANCODE_SPACE]) {
        ball_movement.x = 2.5f;
        ball_movement.y = 1.0f;
    }
    if (glm::length(ball_movement) > 1.0f) {
        ball_movement = glm::normalize(ball_movement);
    }
    
    // Paddle 1
    if (keys[SDL_SCANCODE_W]) {
        p1_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        p1_movement.y = -1.0f;
    }
    if (glm::length(p1_movement) > 1.0f) {
        p1_movement = glm::normalize(p1_movement);
    }
    
    // Paddle 2
    if (keys[SDL_SCANCODE_UP]) {
        p2_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        p2_movement.y = -1.0f;
    }
    if (glm::length(p2_movement) > 1.0f) {
        p2_movement = glm::normalize(p2_movement);
    }
}

bool touchedTop(glm::vec3 ball_position) {
    float top = 3.7;
    return (ball_position[1] > top);
}

bool touchedBottom(glm::vec3 ball_position) {
    float bottom = -3.7;
    return (ball_position[1] < bottom);
}

bool isPastPaddles(glm::vec3 ball_position) {
    float right = 3.2f;
    float left = -3.2f;
    
    if (ball_position[1] > right) {
        cout << "\n==============================\n";
        cout << "Player 1 wins!\n";
        return true;
    }
    else if (ball_position[1] < left) {
        cout << "\n==============================\n";
        cout << "Player 2 wins!\n";
        return true;
    }
    
    return false;
}

bool areColliding(glm::vec3 ball_position, glm::vec3 p_position) {
    // Ball Info
    float x1 = ball_position.x;
    float y1 = ball_position.y;
    float w1 = ball_scale.x;
    float h1 = ball_scale.y;
    
    // Paddle Info
    float x2 = p_position.x;
    float y2 = p_position.y;
    float w2 = p_scale.x;
    float h2 = p_scale.y;
    
    float x_diff = fabs(x2 - x1);
    float y_diff = fabs(y2 - y1);
    
    float x_dist = x_diff - ((w1 + w2) / 2);
    float y_dist = y_diff - ((h1 + h2) / 2);
    
    if ((x_dist < 0) && (y_dist < 0))
        cout << "x_dist: " << x_dist << "\n";
        cout << "y_dist: " << y_dist << "\n";
        
        return true;
    
    return false;
}

void updateBall(float deltaTime) {
    // ---------------------------- ball ----------------------------
        modelMatrix_ball = glm::mat4(1.0f);
        
        if (ball_path_reversed) {
            ball_movement.y *= -1.0;
            ball_position -= ball_movement * ball_speed * deltaTime;
        }
        else {
            ball_movement.x *= -1.0;
            ball_position += ball_movement * ball_speed * deltaTime;
        }
    
        if (touchedTop(ball_position)) {
            cout << "WALL TOP IS TOUCHED\n";
            ball_path_reversed = true;
        }
        else if (touchedBottom(ball_position)) {
            cout << "WALL BOTTOM IS TOUCHED\n";
            ball_path_reversed = false;
        }

        else if (areColliding(ball_position, p1_position)) {
//            ball_path_reversed = true;
            ball_position += (ball_movement * ball_speed * deltaTime);
            cout << "PADDLE ONE IS TOUCHED\n";
    //        modelMatrix_ball = glm::translate(modelMatrix_ball, -1.5f * ball_position);
        }

        else if (areColliding(ball_position, p2_position)) {
//            ball_path_reversed = true;
            ball_position += ball_movement * ball_speed * deltaTime;
            cout << "PADDLE TWO IS TOUCHED\n";
    //        modelMatrix_ball = glm::translate(modelMatrix_ball, ball_position);
        }
        
        modelMatrix_ball = glm::translate(modelMatrix_ball, ball_position);
        modelMatrix_ball = glm::scale(modelMatrix_ball, ball_scale);
    // --------------------------- /ball ----------------------------
}

void updateP1(float deltaTime) {
    // ---------------------------- pad1 ----------------------------
    p1_position += p1_movement * p1_speed * deltaTime;
    modelMatrix_p1 = glm::mat4(1.0f);
    modelMatrix_p1 = glm::translate(modelMatrix_p1, glm::vec3(-4.5f, 1.0f, 0.0f));
    modelMatrix_p1 = glm::translate(modelMatrix_p1, p1_position);
    modelMatrix_p1 = glm::scale(modelMatrix_p1, p_scale);
    // --------------------------- /pad1 ----------------------------
}

void updateP2(float deltaTime) {
    // ---------------------------- pad2 ----------------------------
    p2_position += p2_movement * p2_speed * deltaTime;
    modelMatrix_p2 = glm::mat4(1.0f);
    modelMatrix_p2 = glm::translate(modelMatrix_p2, glm::vec3(4.5f, 0.0f, 0.0f));
    modelMatrix_p2 = glm::translate(modelMatrix_p2, p2_position);
    modelMatrix_p2 = glm::scale(modelMatrix_p2, p_scale);
    // --------------------------- /pad2 ----------------------------
}

float lastTicks = 0.0f;

void Update() {
    if (isPastPaddles(ball_position))
        gameIsRunning = false;
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    updateBall(deltaTime);
    updateP1(deltaTime);
    updateP2(deltaTime);
}

void render_object(glm::mat4 modelMatrix, GLuint textureID) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    program.SetModelMatrix(modelMatrix);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    render_object(modelMatrix_ball, ballTextureID);
    render_object(modelMatrix_p1, paddleTextureID);
    render_object(modelMatrix_p2, paddleTextureID);
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    cout << "Thanks for playing, game over!\n";
    cout << "==============================\n\n";
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
