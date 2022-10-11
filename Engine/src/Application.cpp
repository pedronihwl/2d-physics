#include "Application.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
    particle = new Particle(0, 0, 1.0);

    // TODO: setup objects in the scene
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    // Limitar aplicação a 60 frames por segundo
    int timeTowait = (1000 / 60) - (SDL_GetTicks() - previousFrameTime);
    if (timeTowait > 0) {
        SDL_Delay(timeTowait);
    }

    // Quantos pixels nós queremos mover por segundo e não por frame
    float deltaTime = (SDL_GetTicks() - previousFrameTime) / 1000.0;

    previousFrameTime = SDL_GetTicks();

    particle->acceleration.x = 2.0 * 50;
    particle->acceleration.y = 9.8 * 50;

    particle->velocity += particle->acceleration * deltaTime;
    particle->position += particle->velocity * deltaTime;

    if (particle->position.x - 4 <= 0) {
        particle->position.x = 4;
        particle->velocity.x *= -1.0;
    }
    else if (particle->position.x + 4 >= Graphics::Width()) {
        particle->position.x = Graphics::Width() - 4;
        particle->velocity.x *= -1.0;
    }

    if (particle->position.y - 4 <= 0) {
        particle->position.y = 4;
        particle->velocity.y *= -1.0;
    }
    else if (particle->position.y + 4 >= Graphics::Height() ) {
        particle->position.y = Graphics::Height() - 4;
        particle->velocity.y *= -1.0;
    }
    
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle(particle->position.x, particle->position.y, 5, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    // TODO: destroy all objects in the scene
    delete particle;
    Graphics::CloseWindow();
}