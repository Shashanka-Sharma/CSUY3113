#include "Level1.h"

#define LEVEL1_WIDTH 20
#define LEVEL1_HEIGHT 9
#define LEVEL1_ENEMY_COUNT 3
float level1_data[] =
{
    49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,
    49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,
    49, 49, 49, 49, 49, 49, 49, 49, 49, 0, 0, 49, 0, 0, 0, 0, 0, 0, 0,0,
    49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 49, 49, 0, 0, 0, 0, 0, 0, 0,0,
    49, 0, 0, 0, 0, 0, 0, 0, 49, 49,49,49, 49, 0, 0, 0, 0, 0, 0,0,
    49, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 49, 49, 49, 49, 49, 0, 0, 0,0,
    49, 0, 0, 0, 0, 2, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,88,
    2, 2, 2, 2, 2, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
};

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 20, 9);
    
    state.player = new Entity();
    state.player->position = glm::vec3(1,-6.0f,0);
    state.player->entityType = PLAYER;
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-3.0f,0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("playerSpriteSheet.png");
    state.player->lives = 3;
    
    state.player->animRight = new int[7] {0,1,2,3,4,5,6};
    state.player->animLeft = new int[7] {8,9,10,11,12,13,14};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 16;
    state.player->animRows = 1;
    
    state.player->height = 1.0f;
    state.player->width = 1.0f;
    
    state.player->jumpPower = 3.0f;
     
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemySpriteSheet.png");
    
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(5,-1.0, 0);

    state.enemies[0].animRight = new int[5] {4,5,6,7,8};
    state.enemies[0].animLeft = new int[5] {4,3,2,1,0};

    state.enemies[0].animIndices = state.enemies->animRight;
    state.enemies[0].animFrames = 5;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 9;
    state.enemies[0].animRows = 1;


    state.enemies[0].entityType = ENEMY;
    //state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1.0f;
    
   
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    
    state.enemies->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    
    if (state.player->position.x >= 20) {
        state.nextScene = 1;
    }
}
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemies->Render(program);
    
//    state.enemies->DrawSpriteFromTextureAtlas(program, state.enemies->textureID, state.enemies->animIndices[state.enemies->animIndex]);
    
}
