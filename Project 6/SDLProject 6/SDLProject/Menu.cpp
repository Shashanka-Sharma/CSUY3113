#include "Menu.h"

void Menu::Initialize() {
    
    Mix_Music* music;
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("Werq.mp3");
    Mix_PlayMusic(music, 0);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    
    state.nextScene = -1;
    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    state.fontTextureID = fontTextureID;
    
    
    state.player = new Entity();
   
}
void Menu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, 0, NULL, 0);
}
void Menu::Render(ShaderProgram *program) {
    
    
    Util::DrawText(program, state.fontTextureID,"The Labyrinth of Time", 0.5f, -0.25f, glm::vec3(-2.5f,1,-6));
    Util::DrawText(program, state.fontTextureID, "Press Enter To Start", 0.5f, -0.25, glm::vec3(-2.5f,0,-6));
    Util::DrawText(program, state.fontTextureID, "By: Shashanka Sharma", 0.5f, -0.25f, glm::vec3(-2.5f,-1,-6));
}

