#include "Win.h"

#define OBJECT_COUNT 1

void Win::Initialize() {
    state.nextScene = -1;
    state.transitioning = false;
}

void Win::Update(float deltaTime) {}

void Win::Render(ShaderProgram *program, ShaderProgram *programUI) {
    GLuint fontTextureID = Util::LoadTexture("small_blocky.png");

    Util::DrawText(programUI, fontTextureID, "You Win!", 0.5, 0.0f, glm::vec3(-1.65, 1.5, 0));
    
    Util::DrawText(programUI, fontTextureID, "The snails thank you :)", 0.2, 0.0f, glm::vec3(-1.95, 0.5, 0));
}
