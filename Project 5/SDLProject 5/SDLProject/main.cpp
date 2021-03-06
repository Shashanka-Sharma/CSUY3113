#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>


#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Menu.h"
#include "Finale.h"

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
glm::mat4 uiViewMatrix, uiProjectionMatrix;
GLuint fontTextureID;

Mix_Music* music;

Scene *currentScene;
Scene *sceneList[5];

int lives = 3;

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Platformer!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("The Entertainer.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    
    uiViewMatrix = glm::mat4(1.0f);
    uiProjectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new Finale();

//    if (currentScene == sceneList[4]) {
//        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
//        music = Mix_LoadMUS("death-and-axes-by-kevin-macleod-from-filmmusic-io.mp3");
//        Mix_PlayMusic(music, -1);
//        Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
//    }
    SwitchToScene(sceneList[0]);

    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    currentScene->state.fontTextureID = fontTextureID;
}

void ProcessInput() {
    
    currentScene->state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_RETURN:
                        if (currentScene == sceneList[0]) {
                            SwitchToScene(sceneList[1]);
                        }
                        break;
                        
                    case SDLK_SPACE:
                        if (currentScene->state.player->collidedBottom) {
                            currentScene->state.player->jump = true;
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}

void Update() {
   float ticks = (float)SDL_GetTicks() / 1000.0f;
   float deltaTime = ticks - lastTicks;
    
   lastTicks = ticks;
   deltaTime += accumulator;
    
   if (deltaTime < FIXED_TIMESTEP) {
       accumulator = deltaTime;
       return;
   }
   while (deltaTime >= FIXED_TIMESTEP) {
       
       currentScene->Update(FIXED_TIMESTEP);
       deltaTime -= FIXED_TIMESTEP;
   }
   accumulator = deltaTime;
    
    if (currentScene->state.player->lifeLoss) {
        lives -=1;
        currentScene->state.player->lifeLoss = false;
        if (lives <= 0) {
            currentScene->state.player->isActive = false;
        }
        else {
            currentScene->state.player->position =
            currentScene->state.player->positionReset;
        }
    }
    
    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 5) {
        viewMatrix = glm::translate(viewMatrix,
             glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    } else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
    
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
    program.SetViewMatrix(uiViewMatrix);
    program.SetProjectionMatrix(uiProjectionMatrix);
    
    if (currentScene->state.player->isActive == false) {
            Util::DrawText(&program, currentScene->state.fontTextureID, "You Lose!", 0.5f, -0.25f, glm::vec3(-1, 2,0));
    }
    
    if (currentScene != sceneList[0] && currentScene != sceneList[4] )
    {
        Util::DrawText(&program, currentScene->state.fontTextureID, "Lives: " + std::to_string(lives), 0.5f, -0.25f, glm::vec3 (-4.5f,3.0f,0));
    }
    
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
        
        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        Render();
    }
    
    Shutdown();
    return 0;
}
