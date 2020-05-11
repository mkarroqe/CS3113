#include "Scene.h"

class Win : public Scene {
    public:
        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(ShaderProgram *program, ShaderProgram *programUI) override;
};
