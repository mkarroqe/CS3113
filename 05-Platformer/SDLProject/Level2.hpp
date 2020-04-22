#include "Scene.hpp"

class Level2 : public Scene {
    public:
        Level2(int _lives);
        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(ShaderProgram *program) override;
};
