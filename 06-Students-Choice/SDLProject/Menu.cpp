#include "Menu.h"

#define OBJECT_COUNT 1

void Menu::Initialize() {
    state.nextScene = -1;
    state.transitioning = false;
}

void Menu::Update(float deltaTime) {}

void Menu::Render(ShaderProgram *program, ShaderProgram *programUI) {
    GLuint fontTextureID = Util::LoadTexture("small_blocky.png");

    Util::DrawText(programUI, fontTextureID, "Save Roomba", 0.3, 0.0f, glm::vec3(-1.65, 1.5, 0));
    
    Util::DrawText(programUI, fontTextureID, "(the snail)", 0.15, 0.0f, glm::vec3(-0.85, 1.0, 0));
    
    if (state.transitioning) {
        Util::DrawText(programUI, fontTextureID, "Please be patient, snails load slowly", 0.2, 0.0f, glm::vec3(-3.55, 0.5, 0));
    }
    else {
        Util::DrawText(programUI, fontTextureID, "Press Enter to Start", 0.2, 0.0f, glm::vec3(-2.05, 0.5, 0));
    }
}
