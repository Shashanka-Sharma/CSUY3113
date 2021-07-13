
// Copyrighted Music, link to source found below
//Killing Time by Kevin MacLeod
//Link: https://incompetech.filmmusic.io/song/3953-killing-time
//License: https://filmmusic.io/standard-license

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
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Entity.h"
#define PLATFORM_COUNT 27
#define FIXED_TIMESTEP 0.0166666f
#define ENEMY_COUNT 3

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;

float lastTicks = 0.0f;
float accumulator = 0.0f;


struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* enemies;
    GLuint fontTextureID;
    int counter = 0;
};

GameState state;

Mix_Music* music;



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
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Rise of The AI!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("killing-time-by-kevin-macleod-from-filmmusic-io.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    
    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // good setting for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    state.fontTextureID = LoadTexture("font2.png");
    GLuint platformTextureID = LoadTexture("tile_0073.png");
    GLuint characterTextureID = LoadTexture("character_0001.png");
    GLuint enemyTextureID = LoadTexture("character_0016.png");
    
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-3.25,-3.0,0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-3.81f,0);
    state.player->speed = 2;
    state.player->jumpPower = 3.50f;
    state.player->textureID = characterTextureID;
    state.player->height = 1.0f;
    state.player->width = 1.0f;
    
    
    
    state.platforms = new Entity[PLATFORM_COUNT];
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].textureID = platformTextureID;
        state.platforms->entityType = PLATFORM;
    }
    
    state.platforms[0].position = glm::vec3(-4.45,-4,0);
    state.platforms[1].position = glm::vec3(-3.45,-4,0);
    state.platforms[2].position = glm::vec3(-2.45,-4,0);
    state.platforms[3].position = glm::vec3(-1.45,-4,0);
    state.platforms[4].position = glm::vec3(-0.45,-4,0);
    state.platforms[5].position = glm::vec3(0.55,-4,0);
    state.platforms[6].position = glm::vec3(1.55,-4,0);
    state.platforms[7].position = glm::vec3(2.55,-4,0);
    state.platforms[8].position = glm::vec3(2.55,-4,0);
    state.platforms[9].position = glm::vec3(3.55,-4,0);
    state.platforms[10].position = glm::vec3(4.55,-4,0);
    
    state.platforms[11].position = glm::vec3(4.55,-0.35,0);
    state.platforms[12].position = glm::vec3(3.55,-0.35,0);
    state.platforms[13].position = glm::vec3(2.55,-0.35,0);
    state.platforms[14].position = glm::vec3(1.55,-0.35,0);
    state.platforms[15].position = glm::vec3(0.55,-0.35,0);
    state.platforms[16].position = glm::vec3(-0.45,-0.35,0);
    
    state.platforms[17].position = glm::vec3(-1.45,-1.35,0);
    state.platforms[18].position = glm::vec3(-4.45,-2.5,0);
    
    state.platforms[19].position = glm::vec3(-4.45,2.0,0);
    state.platforms[20].position = glm::vec3(-3.45,2.0,0);
    state.platforms[21].position = glm::vec3(-2.45,2.0,0);
    state.platforms[22].position = glm::vec3(-1.45,2.0,0);
    state.platforms[23].position = glm::vec3(-0.45,2.0,0);
    state.platforms[24].position = glm::vec3(0.55,2.0,0);
    
    state.platforms[25].position = glm::vec3(4.5,0.65,0);
    state.platforms[26].position = glm::vec3(1.55,2,0);
    
    
    state.enemies = new Entity[ENEMY_COUNT];
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].height = 0.75f;
        state.enemies[i].width = 0.75f;
    }
    state.enemies[0].position = glm::vec3(3.55,-3.0,0);
    state.enemies[1].position = glm::vec3(2.55,0.65,0);
    state.enemies[2].position = glm::vec3(-4.45,3.0,0);
    
    state.enemies[0].aiType = ATTACKER;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].acceleration = glm::vec3(0,-3,0);
    state.enemies[0].speed = 1.0f;
    state.enemies[0].jumpPower = 3.0f;
    
    state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = WALKING;
    state.enemies[1].acceleration = glm::vec3(0,-3,0);
    state.enemies[1].speed = 1.0f;
    state.enemies[1].jumpPower = 3.0f;
    
    state.enemies[2].aiType = WALKER;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].acceleration = glm::vec3(0,-3.0,0);
    state.enemies[2].speed = 1.0f;
    state.enemies[2].jumpPower = 3.0f;
    
    
    
    
    
    for (size_t i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, NULL,NULL, 0, NULL, NULL);
    }

}

void ProcessInput() {
    
    state.player->movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                        
                    case SDLK_SPACE:
                        if (state.player->collidedBottom) {
                        state.player->jump = true;
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
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
       state.player->Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies,
                            ENEMY_COUNT);
       
       for (int i = 0; i < ENEMY_COUNT; i++) {
           state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.platforms, PLATFORM_COUNT, state.enemies,ENEMY_COUNT);
       }
       deltaTime -= FIXED_TIMESTEP;
   }
   accumulator = deltaTime;
    
    
//    for (int i = 0; i < ENEMY_COUNT; i++) {
//        if (state.enemies[i].isSuccess == false) {
//            state.player->counter += 1;
//        }
//    }
//
//    if (state.player->counter == ENEMY_COUNT) {
//        state.player->isSuccess = true;
//    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    state.player->Render(&program);
    
    for (int i = 0; i < ENEMY_COUNT ; i++) {
        state.enemies[i].Render(&program);
    }
    
    for (size_t i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    
    int counter = 0;
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (state.enemies[i].isSuccess == false) {
            counter += 1;
            }
    }
    
    if (counter == ENEMY_COUNT) {
        state.player->isSuccess = true;
    }
    
    if (state.player->isSuccess){
                    DrawText(&program, state.fontTextureID, "YOU WIN!", 0.5f, -0.05f, glm::vec3(-1.5f,1.0f,0));
        }
        else if (state.player->isActive == false){
                DrawText(&program, state.fontTextureID, "YOU LOSE!", 0.5f, -0.05f, glm::vec3(-1.5f,1.0f,0));
            
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
