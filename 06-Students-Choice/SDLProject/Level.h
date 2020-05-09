#include "Scene.h"

class Level : public Scene {
    public:
        Level(int _lives);
        void Initialize() override;
        void Update(float deltaTime) override;
        void Render(ShaderProgram *program) override;
};
