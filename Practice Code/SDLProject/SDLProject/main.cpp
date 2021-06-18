#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

glm::vec3 player_position = glm::vec3(0,0,0);
glm::vec3 player_movement = glm::vec3(0,0,0);

float player_speed = 1.0f;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Triangle!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ProcessInput() {
    
    player_movement = glm::vec3(0);

    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // move the player left
                        break;
                    case SDLK_RIGHT:
                        // move the player right
                        player_movement.x = 1.0f;
                        break;
                    case SDLK_SPACE:
                        // some sort of action
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT]) {
        player_movement.x = -1.0f;
    }
    
    else if (keys[SDL_SCANCODE_RIGHT]) {
        player_movement.x = 1.0f;
    }
    if (keys[SDL_SCANCODE_UP]) {
        player_movement.y = 1.0f;
    }
    
    else if (keys[SDL_SCANCODE_DOWN]) {
        player_movement.y = -1.0f;
    }
    
    if (glm::length(player_movement) > 1.0f) {
        player_movement = glm::normalize(player_movement);
    }
    
}
float lastTicks = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    // Add (direction * units per second * elapsed time)
    player_position += player_movement * player_speed * deltaTime;
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, player_position);
    
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetModelMatrix(modelMatrix);
    
    float vertices[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(program.positionAttribute);
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}


