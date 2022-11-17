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


    particles.push_back(new Particle(Circle(25.0), Graphics::Width() - 100, Graphics::Height() / 2, 5.0, 0xFFFFFFFF));

    particles.push_back(new Particle(Circle(25.0), Graphics::Width() / 5, Graphics::Height() / 2 + 125, 5.0, 0xFFAA3300));
    particles.push_back(new Particle(Circle(25.0), Graphics::Width() / 5, Graphics::Height() / 2 + 150, 5.0, 0xFFAA3300));
    particles.push_back(new Particle(Circle(25.0), Graphics::Width() / 5, Graphics::Height() / 2 + 175, 5.0, 0xFFAA3300));
    particles.push_back(new Particle(Circle(25.0), Graphics::Width() / 5, Graphics::Height() / 2 + 200, 5.0, 0xFFAA3300));

    particles.push_back(new Particle(Circle(25.0), Graphics::Width() / 4, Graphics::Height() / 2 + 125, 5.0, 0xFFAA3300));
    particles.push_back(new Particle(Circle(25.0), Graphics::Width() / 4, Graphics::Height() / 2 + 150, 5.0, 0xFFAA3300));
    particles.push_back(new Particle(Circle(25.0), Graphics::Width() / 4, Graphics::Height() / 2 + 175, 5.0, 0xFFAA3300));
    particles.push_back(new Particle(Circle(25.0), Graphics::Width() / 4, Graphics::Height() / 2 + 200, 5.0, 0xFFAA3300));

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
                contract.ResolveCollision();
                //Graphics::DrawFillCircle(contract.start.x, contract.start.y, 3, 0xFFFF00FF);
                //Graphics::DrawFillCircle(contract.end.x, contract.end.y, 3, 0xFFFF00FF);
                //Graphics::DrawLine(contract.start.x, contract.start.y, contract.start.x + contract.normal.x * 15, contract.start.y + contract.normal.y * 15, 0xFFFF00FF);
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

        if (particle->shape->GetType() == CIRCLE) {
            Circle* circle = (Circle*)particle->shape;
            Graphics::DrawFillCircle(particle->position.x, particle->position.y, circle->radius, particle->color);
        }

        if (particle->shape->GetType() == BOX) {
            Box* box = (Box*)particle->shape;
            Graphics::DrawPolygon(particle->position.x, particle->position.y, box->worldSpace, particle->color);
        }
    }

    if (leftMouseButtonDown) {
        Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, mouse.x, mouse.y, 0xFF0000FF);
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