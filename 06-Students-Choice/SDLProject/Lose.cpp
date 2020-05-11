#include "Lose.h"

#define OBJECT_COUNT 1

void Lose::Initialize() {
    state.nextScene = -1;
    state.transitioning = false;
}

void Lose::Update(float deltaTime) {}

void Lose::Render(ShaderProgram *program, ShaderProgram *programUI) {
    GLuint fontTextureID = Util::LoadTexture("small_blocky.png");

    Util::DrawText(programUI, fontTextureID, "You Lose", 0.3, 0.0f, glm::vec3(-1.65, 1.5, 0));
    
    Util::DrawText(programUI, fontTextureID, ":(", 0.15, 0.0f, glm::vec3(-1.5, 1.0, 0));
    
    
    Util::DrawText(programUI, fontTextureID, "All hail the tyrant betta", 0.2, 0.0f, glm::vec3(-2.05, 0.5, 0));
}
