#include "Level2.h"

#define LEVEL2_WIDTH 20
#define LEVEL2_HEIGHT 9
#define LEVEL2_ENEMY_COUNT 3
float level2_data[] =
{
    156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    156, 0, 0, 153, 154, 155, 153, 154, 155, 153, 154, 155, 153, 154, 155, 153, 154, 155, 156, 0,
    156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 156, 0,
    156, 153, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 156, 0,
    156, 155, 153, 154, 155, 153, 154, 155, 153, 154, 155, 153, 154, 155, 153, 154, 0, 0, 156, 0,
    156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 156, 0,
    156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 154, 156, 88,
    156, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
    156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void Level2::Initialize() {
    
    state.nextScene = -1;
    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    state.fontTextureID = fontTextureID;
    
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 20, 9);
    
    state.player = new Entity();
    state.player->position = glm::vec3(3,-6.0f,0);
    state.player->entityType = PLAYER;
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-3.0f,0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("playerSpriteSheet.png");
    state.player->lives = state.lives;
    
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
    
    state.player->jumpPower = 4.0f;
     
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("character_0021.png");
    
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(0,-6.0, 0);

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1.0f;
    state.enemies[0].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(3,-6.0, 0);

    state.enemies[1].entityType = ENEMY;
    //state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].speed = 1.0f;
    state.enemies[1].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(11.5f,2, 0);

    state.enemies[2].entityType = ENEMY;
    //state.enemies[2].aiType = WALKER;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].speed = 1.0f;
    state.enemies[2].acceleration = glm::vec3(0,-3,0);
   
}
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    
    for (size_t i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    }
    if (state.player->position.x >= 20) {
        state.nextScene = 3;
    }
}
void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    
    for (size_t i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}
