#include "Scene.hpp"

class Level3 : public Scene {
    public:
        Level3(int _lives);
        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(ShaderProgram *program) override;
};
