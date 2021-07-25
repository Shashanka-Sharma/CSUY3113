#include "Level3.h"

#define LEVEL3_WIDTH 14
#define LEVEL3_HEIGHT 8
#define LEVEL3_ENEMY_COUNT 1
float level3_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 1, 1, 1, 1, 1, 1, 0, 3, 3, 3, 3, 3, 3,
    3, 2, 2, 2, 2, 2, 2, 0, 3, 3, 3, 3, 3, 3
};

void Level3::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    
    state.player = new Entity();
    state.player->position = glm::vec3(5,0,0);
    state.player->entityType = PLAYER;
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-3.0f,0);
    state.player->speed = 1.5f;
    state.player->textureID = Util::LoadTexture("playerSpriteSheet.png");
    
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    
    state.player->jumpPower = 5.0f;
     
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemySpriteSheet.png");
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(4,-2.25f, 0);
    
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1.0f;
    state.enemies[0].isActive = false;
   
}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
}
void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
}
