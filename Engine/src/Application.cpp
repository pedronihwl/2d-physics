#include "Application.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    particles.push_back(new Particle(50, 100, 1.0));
    particles.push_back(new Particle(100, 100, 3.0));

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

                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = -50 * 50;
                if (event.key.keysym.sym == SDLK_RIGHT) 
                    pushForce.x = 50 * 50;
                if (event.key.keysym.sym == SDLK_DOWN) 
                    pushForce.y = 50 * 50;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -50 * 50;

                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
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

    if (deltaTime > 0.016) {
        deltaTime = 0.016;
    }

    previousFrameTime = SDL_GetTicks();

    // Adicionando vento para a particula
    for (auto particle : particles) {
        particle->AddForce(Vec2(2.0 * 50, 0.0 * 50));
    }

    // Push force
    for (auto particle : particles) {
        particle->AddForce(pushForce);
    }

    // Adicionando peso para a particula
    for (auto particle : particles) {
        particle->AddForce(Vec2(0.0 * 50, particle->mass * 9.8 * 50));
    }

    for (auto particle : particles) {
        particle->Integrate(deltaTime);
    }

    for (auto particle : particles) {
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
        else if (particle->position.y + 4 >= Graphics::Height()) {
            particle->position.y = Graphics::Height() - 4;
            particle->velocity.y *= -1.0;
        }

    }

    
    
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    
    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->mass * 4, 0xFFFFFFFF);
    Graphics::DrawFillCircle(particles[1]->position.x, particles[1]->position.y, particles[1]->mass * 4, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    // TODO: destroy all objects in the scene
    for (auto particle : particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}