#include "Finale.h"

#define FINALE_WIDTH 14
#define FINALE_HEIGHT 8
#define FINALE_ENEMY_COUNT 0
float finale_data[] =
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

void Finale::Initialize() {
    
    state.nextScene = -1;
    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    state.fontTextureID = fontTextureID;
    
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    
    state.player = new Entity();
    state.map = new Map(FINALE_WIDTH, FINALE_HEIGHT, finale_data, mapTextureID, 1.0f, 4, 1  );
   
}
void Finale::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, FINALE_ENEMY_COUNT, state.map);
}
void Finale::Render(ShaderProgram *program) {
    
    
    Util::DrawText(program, state.fontTextureID,"You Win!", 0.5f, -0.25f, glm::vec3(3.5,-2.25f,0));
    Util::DrawText(program, state.fontTextureID, "Images by KennyPlatformerAssets", 0.5f, -0.25, glm::vec3(1.75,-3.25f,0));
    Util::DrawText(program, state.fontTextureID, "Music by Kevin Macleod", 0.5f, -0.25f, glm::vec3(1.75,-4.25f,0));
    
}
