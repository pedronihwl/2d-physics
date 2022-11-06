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

    //Particle* particle = new Particle(Circle(50), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);

    Particle* particle = new Particle(Box(300.0,200.0), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 3.0);
    particles.push_back(particle);

   
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

    for (auto particle : particles) {
        //particle->AddForce(Vec2(0.0, 9.8 * particle->mass * 50));
        //particle->AddTorque(6000);

        particle->rotation += 0.03;
    }


    for (auto particle : particles) {
        particle->UpdateBody(deltaTime);
    }

    // Bordas
    for (auto particle : particles) {

        if (particle->shape->GetType() == CIRCLE) {
            Circle* circle = (Circle*)particle->shape;

            if (particle->position.x - circle->radius <= 0) {
                particle->position.x = circle->radius;
                particle->velocity.x *= -1.0;
            }
            else if (particle->position.x + circle->radius >= Graphics::Width()) {
                particle->position.x = Graphics::Width() - circle->radius;
                particle->velocity.x *= -1.0;
            }

            if (particle->position.y - circle->radius <= 0) {
                particle->position.y = circle->radius;
                particle->velocity.y *= -1.0;
            }
            else if (particle->position.y + circle->radius >= Graphics::Height()) {
                particle->position.y = Graphics::Height() - circle->radius;
                particle->velocity.y *= -1.0;
            }

        }

    }    
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF0F0721);

    for (auto particle : particles) {

        if (particle->shape->GetType() == CIRCLE) {
            Circle* circle = (Circle*)particle->shape;
            Graphics::DrawCircle(particle->position.x, particle->position.y, circle->radius, particle->rotation, 0xFFFFFFFF);
        }

        if (particle->shape->GetType() == BOX) {
            Box* box = (Box*)particle->shape;
            Graphics::DrawPolygon(particle->position.x, particle->position.y, box->worldSpace, 0xFFFFFFFF);
        }
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