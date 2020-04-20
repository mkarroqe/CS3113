#include "Scene.hpp"

class Lose : public Scene {
    public:
        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(ShaderProgram *program) override;
};
