#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <string>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Entity.h"
#define OBSTACLE_COUNT 31
#define FIXED_TIMESTEP 0.0166666f


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;

float lastTicks = 0.0f;
float accumulator = 0.0f;

enum GameStatus { ACTIVE, WIN , LOSE};

struct GameState {
    Entity* player;
//    Entity* launchPad;
    Entity* obstacles;
    GameStatus gameStatus;
    GLuint fontTextureID;
};

GameState state;

GLuint LoadTexture(const char* filePath) { // we do not have the filepath for the image yet
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
}
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}

void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text,float size, float spacing, glm::vec3 position)
{
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;
    
    std::vector<float> vertices;
    std::vector<float> texCoords;
    
    for(int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        
        vertices.insert(vertices.end(), {
                offset + (-0.5f * size), 0.5f * size,
                offset + (-0.5f * size), -0.5f * size,
                offset + (0.5f * size), 0.5f * size,
                offset + (0.5f * size), -0.5f * size,
                offset + (0.5f * size), 0.5f * size,
                offset + (-0.5f * size), -0.5f * size,
        });
            texCoords.insert(texCoords.end(), {
                u, v,
                u, v + height,
                u + width, v,
                u + width, v + height,
                u + width, v,
                u, v + height,
            });
        } // end of for loop
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        program->SetModelMatrix(modelMatrix);
        glUseProgram(program->programID);
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
        glEnableVertexAttribArray(program->positionAttribute);
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
        glEnableVertexAttribArray(program->texCoordAttribute);
        glBindTexture(GL_TEXTURE_2D, fontTextureID);
        glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Lunar Lander!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // good setting for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // the loaded textures
    GLuint playerTextureID = LoadTexture("enemyBlack4.png");
    GLuint padTextureID = LoadTexture("buttonBlue.png");
    GLuint obstacleTextureID = LoadTexture("platformPack_tile041.png");
    // initialize player entity
    state.player = new Entity();
    state.player->textureID = playerTextureID;
    state.player->position = glm::vec3(0.0f,3.5f,0);
    state.player->acceleration = glm::vec3(0,-0.5f,0);
    state.player->speed = 1.5f;
    state.player->entityType = PLAYER;

    state.obstacles = new Entity[OBSTACLE_COUNT];
    for (size_t i = 0; i < OBSTACLE_COUNT-1; i++) {
        state.obstacles[i].textureID = obstacleTextureID;
        state.obstacles[i].entityType = DEADTILE;
    }
    // set these positions later
    state.obstacles[0].position = glm::vec3(-4.4,-3.25f,0);
    state.obstacles[1].position = glm::vec3(-3.4,-3.25f,0);
    state.obstacles[2].position = glm::vec3(-2.4,-3.25f,0);
    state.obstacles[3].position = glm::vec3(-1.4,-3.25f,0);
    state.obstacles[4].position = glm::vec3(-0.4,-3.25f,0);
    state.obstacles[5].position = glm::vec3(0.6,-3.25f,0);
    state.obstacles[6].position = glm::vec3(1.6,-3.25f,0);
    
    state.obstacles[7].position = glm::vec3(3.6,-3.25f,0);
    state.obstacles[8].position = glm::vec3(4.6,-3.25f,0);
    state.obstacles[9].position = glm::vec3(-4.4,-2.25f,0);
    state.obstacles[10].position = glm::vec3(-4.4,-1.25f,0);
    state.obstacles[11].position = glm::vec3(-4.4,-0.25f,0);
    state.obstacles[12].position = glm::vec3(-4.4,0.75f,0);
    state.obstacles[13].position = glm::vec3(-4.4,1.75f,0);
    state.obstacles[14].position = glm::vec3(-4.4,2.75f,0);
    state.obstacles[15].position = glm::vec3(-4.4,3.75f,0);
    state.obstacles[16].position = glm::vec3(4.6,-2.25,0);
    state.obstacles[17].position = glm::vec3(4.6,-1.25,0);
    state.obstacles[18].position = glm::vec3(4.6,-0.25,0);
    state.obstacles[19].position = glm::vec3(4.6,0.75,0);
    state.obstacles[20].position = glm::vec3(4.6,1.75,0);
    state.obstacles[21].position = glm::vec3(4.6,2.75,0);
    state.obstacles[22].position = glm::vec3(4.6,3.75,0);
    state.obstacles[23].position = glm::vec3(4.6,0.5,0);
    state.obstacles[24].position = glm::vec3(3.6,0.5,0);
    state.obstacles[25].position = glm::vec3(2.6,0.5,0);
    state.obstacles[26].position = glm::vec3(1.6,0.5,0);
    state.obstacles[27].position = glm::vec3(-3.4,2.75,0);
    state.obstacles[28].position = glm::vec3(-2.4,2.75,0);
    state.obstacles[29].position = glm::vec3(-1.4,2.75,0);
    state.obstacles[30].entityType = WINTILE;
    state.obstacles[30].textureID = padTextureID;
    state.obstacles[30].position = glm::vec3(2.6f,-3.05f,0);
    //state.obstacles[30].entityType = WINTILE;
    
    for (size_t i = 0; i < OBSTACLE_COUNT; i++) {
        state.obstacles[i].Update(0, NULL, 0);
    }
    
    state.gameStatus = ACTIVE;
    state.fontTextureID = LoadTexture("font2.png");
}

void ProcessInput() {
    
    state.player->movement = glm::vec3(0);

    
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
        if (state.player->isActive){
        state.player->acceleration.x -= 3.0f;
        }
    }
    
    else if (keys[SDL_SCANCODE_RIGHT]) {
        if (state.player->isActive){
        state.player->acceleration.x += 3.0f;
        }
    }
    
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
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
       // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
       if (state.gameStatus == ACTIVE ){
           state.player->Update(FIXED_TIMESTEP, state.obstacles, OBSTACLE_COUNT);
       }
       deltaTime -= FIXED_TIMESTEP;
   }
   accumulator = deltaTime;
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    state.player->Render(&program);
//    state.launchPad->Render(&program);
    
    for (size_t i = 0; i < OBSTACLE_COUNT; i++) {
        state.obstacles[i].Render(&program);
    }
    if (state.player->isSuccess){
            //if (state.player->isSuccess == true){
                state.gameStatus = WIN;
                DrawText(&program, state.fontTextureID, "MISSION SUCCESS!", 0.5f, -0.05f, glm::vec3(-2.95f,1.25f,0));
    }
    else if (state.player->isActive == false){
        //if (state.player->isSuccess == false){
            state.gameStatus = LOSE;
            DrawText(&program, state.fontTextureID, "MISSION FAILED!", 0.5f, -0.05f, glm::vec3(-2.95f,1.25f,0));
        //}
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
        Render();
    }
    
    Shutdown();
    return 0;
}
