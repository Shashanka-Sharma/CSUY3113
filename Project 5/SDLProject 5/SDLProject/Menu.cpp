#include "Menu.h"

#define MENU_WIDTH 14
#define MENU_HEIGHT 8
#define MENU_ENEMY_COUNT 0
unsigned int menu_data[] =
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
    
    state.nextScene = -1;
    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    state.fontTextureID = fontTextureID;
    
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    
    state.player = new Entity();
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 4, 1  );
    
    
    
    
   
}
void Menu::Update(float deltaTime) {
    state.player->Update(deltaTime, NULL, NULL, MENU_ENEMY_COUNT, NULL);
}
void Menu::Render(ShaderProgram *program) {
    Util::DrawText(program, state.fontTextureID,"Shroom Craze!", 0.5f, 0.5f, glm::vec3(0,0,0));
}

