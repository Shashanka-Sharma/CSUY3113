#include "Finale.h"

void Finale::Initialize() {
    
    Mix_Music* music;
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("Fantasia Fantasia.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    
    state.nextScene = -1;
    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    state.fontTextureID = fontTextureID;
    
    state.player = new Entity();
}
void Finale::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, 0, NULL, 0);
}
void Finale::Render(ShaderProgram *program) {
    
    
    Util::DrawText(program, state.fontTextureID,"You Escaped the Labyrinth!", 0.5f, -0.25f, glm::vec3(-2.5f,1,-6));
    Util::DrawText(program, state.fontTextureID, "Images from Itch.io & seamless Textures", 0.5f, -0.25, glm::vec3(-2.5f,0,-6));
    Util::DrawText(program, state.fontTextureID, "Music by Kevin Macleod and Dylan Sitts", 0.5f, -0.25f, glm::vec3(-2.5f,-1,-6));
    
}
