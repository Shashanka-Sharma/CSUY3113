#include "Menu.h"

#define MENU_WIDTH 14
#define MENU_HEIGHT 8
#define MENU_ENEMY_COUNT 0
float menu_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3
};

void Menu::Initialize() {
    
    Mix_Music* music;
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("The Entertainer.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    
    state.nextScene = -1;
    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    state.fontTextureID = fontTextureID;
    
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    
    state.player = new Entity();
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 4, 1  );
   
}
void Menu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, MENU_ENEMY_COUNT, state.map);
}
void Menu::Render(ShaderProgram *program) {
    
    
    Util::DrawText(program, state.fontTextureID,"Shroom Craze!", 0.5f, -0.25f, glm::vec3(3.5,-2.25f,0));
    Util::DrawText(program, state.fontTextureID, "Press Enter To Start", 0.5f, -0.25, glm::vec3(2.75,-3.25f,0));
    Util::DrawText(program, state.fontTextureID, "By: Shashanka Sharma", 0.5f, -0.25f, glm::vec3(2.75,-4.25f,0));
}

