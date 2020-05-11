#include "Scene.h"

class Lose : public Scene {
    public:
        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(ShaderProgram *program, ShaderProgram *programUI) override;
};
