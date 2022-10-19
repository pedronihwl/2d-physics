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

    anchor = Vec2(Graphics::Width() / 2, 30);

    for (int i = 0; i < 5;i++) {
        Particle* holded = new Particle(anchor.x, anchor.y + (i * restLength), 2.0);
        particles.push_back(holded);
    }
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
                    Vec2 impulseDirection = (particles[4]->position - mouse).UnitVector();
                    float impulseMagnitude = (particles[4]->position - mouse).Magnitude() * 5.0;
                    particles[4]->velocity = impulseDirection * impulseMagnitude;
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

        particle->AddForce(Force::GenerateDragForce(*particle, 0.005));

        // Weight
        particle->AddForce(Vec2(0.0, particle->mass * 9.8 * 50));
    }

    //Spring forces
    particles[0]->AddForce(Force::GenerateSpringForce(*particles[0], anchor, restLength, k));

    for (int i = 1;i < 5;i++) {
        Vec2 springForce = Force::GenerateSpringForce(*particles[i], *particles[i - 1], restLength, k);

        particles[i]->AddForce(springForce);
        particles[i - 1]->AddForce(-springForce);
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
    Graphics::ClearScreen(0xFF0F0721);

    if (leftMouseButtonDown) {
        Graphics::DrawLine(particles[4]->position.x, particles[4]->position.y, mouse.x, mouse.y, 0xFF0000FF);
    }

    Graphics::DrawFillCircle(anchor.x, anchor.y, 4, 0xFF001155);

    //Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFF313131);
    //Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->mass * 4, 0xFFAA3300);

    Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFF313131);
    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->mass * 4, 0xFFAA3300);

    for (int i = 1; i < 5;i++) {
        Graphics::DrawLine(particles[i - 1]->position.x, particles[i - 1]->position.y, particles[i]->position.x, particles[i]->position.y, 0xFF313131);
        Graphics::DrawFillCircle(particles[i]->position.x, particles[i]->position.y, particles[i]->mass * 4, 0xFFAA3300);
    }

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