#include "Application.h"
#include "Physics/Force.h"
#include "Physics/CollisionDetector.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    Particle* a = new Particle(Box(200,200), Graphics::Width() / 2, Graphics::Height() / 2, 1.0, 0xFFFF00FF);
    Particle* b = new Particle(Box(200, 200), Graphics::Width() / 2, Graphics::Height() / 2, 1.0, 0xFFFF00FF);

    a->angularVelocity = 0.9;
    b->angularVelocity = 0.9;

    particles.push_back(a);
    particles.push_back(b);

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
            case SDL_MOUSEBUTTONDOWN:
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

                particles[1]->position.x = mouse.x;
                particles[1]->position.y = mouse.y;
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    Graphics::ClearScreen(0xFF0F0721);

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

        particle->AddForce(Force::GenerateFrictionForce(*particle, 250));
    }

    for (auto particle : particles) {
        particle->UpdateBody(deltaTime);
    }

    // Collision detection
    for (int i = 0; i <= particles.size() - 1; i++) {
        for (int j = i + 1; j < particles.size(); j++) {
            Contract contract;
           
            bool hasCollision = CollisionDetector::IsColliding(particles[i], particles[j], contract);

            particles[i]->hasCollision = hasCollision;
            particles[j]->hasCollision = hasCollision;

            if (hasCollision) {
                //contract.ResolveCollision();
            }
        }
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

    for (auto particle : particles) {

        particle->color = particle->hasCollision ? 0xFF0000FF : 0xFFFFFFFF ;

        if (particle->shape->GetType() == CIRCLE) {
            Circle* circle = (Circle*)particle->shape;
            Graphics::DrawFillCircle(particle->position.x, particle->position.y, circle->radius, particle->color);
        }

        if (particle->shape->GetType() == BOX) {
            Box* box = (Box*)particle->shape;
            Graphics::DrawPolygon(particle->position.x, particle->position.y, box->worldSpace, particle->color);
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