#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "Physics/Particle.h"

class Application {
    private:
        bool running = false;
        int previousFrameTime;

        std::vector<Particle*> particles;
        Vec2 pushForce = Vec2();

        // liquid in your screen
        SDL_Rect liquid;

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif