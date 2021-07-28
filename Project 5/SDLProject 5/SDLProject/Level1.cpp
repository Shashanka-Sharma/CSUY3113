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
    49, 0, 0, 0, 0, 0, 0, 0, 0, 49,49,49, 49, 0, 0, 0, 0, 0, 0,0,
    49, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 49, 49, 49, 49, 49, 0, 0, 0,0,
    49, 0, 0, 0, 0, 2, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,88,
    2, 2, 2, 2, 2, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
};

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    state.fontTextureID = fontTextureID;
    
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 20, 9);
    
    state.player = new Entity();
    state.player->position = glm::vec3(1,-6.0f,0);
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
    
    state.player->jumpPower = 3.0f;
     
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("character_0021.png");
    
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(3.4,-1.0, 0);

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1.0f;
    state.enemies[0].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(11,-6.0, 0);

    state.enemies[1].entityType = ENEMY;
    //state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].speed = 1.0f;
    state.enemies[1].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(17.5f,-1.0, 0);

    state.enemies[2].entityType = ENEMY;
    //state.enemies[2].aiType = WALKER;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].speed = 1.0f;
    state.enemies[2].acceleration = glm::vec3(0,-3,0);
    
    
   
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    
    for (size_t i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    }
    if (state.player->position.x >= 20) {
        state.nextScene = 2;
    }
}
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    
    for (size_t i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
    
}
