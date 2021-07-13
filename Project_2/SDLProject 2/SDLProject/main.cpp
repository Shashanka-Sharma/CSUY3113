#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// initalizes display window pointer
SDL_Window* displayWindow;
bool gameIsRunning = true;

// initalizes shader program
ShaderProgram program;
glm::mat4 viewMatrix, ballMatrix, projectionMatrix, platformAMatrix, platformBMatrix;

// initalizes texture ID's
GLuint platformATextureID;
GLuint platformBTextureID;
GLuint ballTextureID;

Mix_Music* music;
Mix_Chunk* bounce;

//sets the players positions and speed
glm::vec3 playerA_position = glm::vec3(2.6,0,0); // 3.25 - ycoord
glm::vec3 playerA_movement = glm::vec3(0,0,0);
glm::vec3 playerB_position = glm::vec3(-4.8,0,0);
glm::vec3 playerB_movement = glm::vec3(0,0,0);
glm::vec3 ball_position = glm::vec3(-1.0,0,0);
glm::vec3 ball_movement = glm::vec3(1.0f,1.0f,0.0f);
float playerA_speed = 2.0f;
float playerB_speed = 2.0f;
float ball_speed = 0.0f;
float lastTicks = 0.0f;


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
void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Pong Game!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    // this is how you play music
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    
    bounce = Mix_LoadWAV("bounce.wav");
    
    // standard matrices for display
    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    // initializing the matrices for the textures
    ballMatrix = glm::mat4(1.0f);
    platformAMatrix = glm::mat4(1.0f);
    platformBMatrix = glm::mat4(1.0f);
    
    // creates the display matrix (standard)
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(2.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    
    // good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // loads textures from file path
    platformATextureID = LoadTexture("download.jpg");
    platformBTextureID = LoadTexture("download.jpg");
    ballTextureID = LoadTexture("square-64.png");
    
    
    
    
}
void collison() {
    float x_dist = fabs(playerA_position.x - ball_position.x) -
    ((1.0f + 0.4f) / 2.0f);
    float y_dist = fabs(playerA_position.y - ball_position.y) -
    ((1.0f + 0.4f) / 2.0f);
    
    float second_xdist = fabs(playerB_position.x - ball_position.x) -
    ((1.0f + 0.4f) / 2.0f);
    float second_ydist = fabs(playerB_position.y - ball_position.y) -
    ((1.0f + 0.4f) / 2.0f);
    
    if ((x_dist < 0 && y_dist < 0) || (second_xdist < 0 &&
                                       second_ydist < 0)) {
        // Player Collision
        if(ball_movement.x == 1.0f){
            ball_movement.x = -ball_movement.x;
            Mix_PlayChannel(-1,bounce,0);
        }
        else {
            ball_movement.x = -ball_movement.x;
            Mix_PlayChannel(-1,bounce,0);
        }
    }
    
    if (ball_position.y >= 3.5f) {
        ball_movement.y = -ball_movement.y;
        Mix_PlayChannel(-1,bounce,0);
    }
    else if (ball_position.y <= -3.5f){
        ball_movement.y = -ball_movement.y;
        Mix_PlayChannel(-1,bounce,0);
    }
    if (ball_position.x >= 2.6f) {
        ball_speed = 0.0f;
        playerA_speed = 0.0f;
    }
    else if (ball_position.x <= -4.8f) {
        ball_speed = 0.0f;
        playerB_speed = 0.0f;
    }
}

void ProcessInput() {
    playerA_movement = glm::vec3(0);
    playerB_movement = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_SPACE:
                        ball_speed = 3.1f;
                        
                }
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_UP] && playerA_position.y < 3.2f) {
        playerA_movement.y = 5.5f;
        
    }
    else if (keys[SDL_SCANCODE_DOWN] && playerA_position.y > -3.2f){
        playerA_movement.y = -5.5f;
    }
    
    if (keys[SDL_SCANCODE_W] && playerB_position.y < 3.2f) {
        playerB_movement.y = 5.5f;
    }
    else if (keys[SDL_SCANCODE_S] && playerB_position.y > -3.25f) {
        playerB_movement.y = -5.5f;
    }
   
    
    if (glm::length(playerA_movement) > 1.0f) {
        playerA_movement = glm::normalize(playerA_movement);
    }
    if (glm::length(playerB_movement) > 1.0f) {
        playerB_movement = glm::normalize(playerB_movement);
    }
    if (glm::length(ball_movement) > 1.0f) {
        ball_movement =
        glm::normalize(ball_movement);
    }


 
}


void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    // Add (direction * units per second * elapsed time)
    playerA_position += playerA_movement * playerA_speed * deltaTime;
    playerB_position += playerB_movement * playerB_speed * deltaTime;
    ball_position += ball_movement * ball_speed * deltaTime;
    
    ballMatrix = glm::mat4(1.0f);
    collison();

    
    ballMatrix = glm::translate(ballMatrix, ball_position);
    
    platformAMatrix = glm::mat4(1.0f);
    platformAMatrix = glm::translate(platformAMatrix, playerA_position);

    platformBMatrix = glm::mat4(1.0f);
    platformBMatrix = glm::translate(platformBMatrix, playerB_position);
}

void Render() {
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    float ball_vertices[] = {-0.2,-0.2,0.2,-0.2,0.2,0.2,
        -0.2,-0.2,0.2,0.2,-0.2,0.2};
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    // draws player
    program.SetModelMatrix(platformAMatrix);
    glBindTexture(GL_TEXTURE_2D, platformATextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // draws player
    program.SetModelMatrix(platformBMatrix);
    glBindTexture(GL_TEXTURE_2D, platformBTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // draws the ball
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball_vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    
    program.SetModelMatrix(ballMatrix);
    glBindTexture(GL_TEXTURE_2D, ballTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
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
