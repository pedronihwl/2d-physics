#include "Application.h"
#include "Physics/Force.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    // TODO: setup objects in the scene
    particles.push_back(new Particle(200, 200, 1.0));
    particles.push_back(new Particle(400, 300, 20.0));

    liquid.x = 0;
    liquid.w = Graphics::Width();
    liquid.y = Graphics::Height() / 2;
    liquid.h = Graphics::Height() / 2;
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

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    particles.push_back(new Particle(x,y,2));
                }

                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouse.x = x;
                    mouse.y = y;
                }
                break;

            case SDL_MOUSEMOTION:
                mouse.x = event.motion.x;
                mouse.y = event.motion.y;
                break;


            case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    Vec2 impulseDirection = (particles[0]->position - mouse).UnitVector();
                    float impulseMagnitude = (particles[0]->position - mouse).Magnitude() * 5.0;
                    particles[0]->velocity = impulseDirection * impulseMagnitude;
                }
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
        particle->AddForce(pushForce);
        particle->AddForce(Force::GenerateFrictionForce(*particle, 5));
    }

    Vec2 attraction = Force::GenerateGravitionalForce(*particles[0], *particles[1], 500.0, 5.0, 100.0);
    particles[0]->AddForce(attraction);
    particles[1]->AddForce(-attraction);

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
    Graphics::ClearScreen(0xFF0F0721);

    //Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF6E3713);
    if (leftMouseButtonDown) {
        Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, mouse.x, mouse.y, 0xFF0000FF);
    }

    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->mass * 4, 0xFFAA3300);
    Graphics::DrawFillCircle(particles[1]->position.x, particles[1]->position.y, particles[1]->mass * 1, 0xFF00FFFF);

    //for (auto particle : particles) {
        //Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->mass * 4, 0xFFFFFFFF);
    //}

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