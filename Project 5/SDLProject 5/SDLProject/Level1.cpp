#include "Level1.h"

#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 1
unsigned int level1_data[] =
{
    10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    
    state.player = new Entity();
    state.player->position = glm::vec3(5,0,0);
    state.player->entityType = PLAYER;
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-3.0f,0);
    state.player->speed = 1.5f;
    state.player->textureID = Util::LoadTexture("playerSpriteSheet.png");
    
    state.player->animRight = new int[7] {7,8,9,10,11,12,13};
    state.player->animLeft = new int[7] {6,5,4,3,2,1,0};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 7;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 7;
    state.player->animRows = 7;
    
    state.player->height = 1.0f;
    state.player->width = 1.0f;
    
    state.player->jumpPower = 5.0f;
     
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemySpriteSheet.png");
    
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(4,-2.25f, 0);
    
    state.enemies[0].animRight = new int[5] {4,5,6,7,8};
    state.enemies[0].animLeft = new int[5] {4,3,2,1,0};

    state.enemies[0].animIndices = state.player->animRight;
    state.enemies[0].animFrames = 5;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 5;
    state.enemies[0].animRows = 5;
    
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1.0f;
    
   
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    
    if (state.player->position.x >= 12) {
        state.nextScene = 1;
    }
}
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    
    state.player->DrawSpriteFromTextureAtlas(program, state.player->textureID, state.player->animIndices[state.player->animIndex]);
    
    state.enemies->DrawSpriteFromTextureAtlas(program, state.enemies->textureID, state.enemies->animIndices[state.enemies->animIndex]);
}
