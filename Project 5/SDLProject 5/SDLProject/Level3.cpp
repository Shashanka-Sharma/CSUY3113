#include "Level3.h"

#define LEVEL3_WIDTH 20
#define LEVEL3_HEIGHT 9
#define LEVEL3_ENEMY_COUNT 9
float level3_data[] =
{
    12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    52, 13, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12,
    52, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 12, 0, 52,
    52, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 52, 0, 52,
    52, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 52, 0, 52, 0, 52,
    52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 52, 0, 52, 0, 52, 0, 52,
    52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 52, 0, 52, 0, 52, 0, 52, 0, 52,
    52, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,13, 13, 13, 13, 13,13, 13,
    52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void Level3::Initialize() {
    
    state.nextScene = -1;
    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    state.fontTextureID = fontTextureID;
    
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 20, 9);
    
    state.player = new Entity();
    state.player->position = glm::vec3(2,0,0);
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
    
    state.player->jumpPower = 2.5f;
     
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("character_0021.png");
    
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(1,-6.0, 0);

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1.0f;
    state.enemies[0].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(3,-6.0, 0);

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].speed = 1.0f;
    state.enemies[1].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(11.5f,2, 0);

    state.enemies[2].entityType = ENEMY;
    state.enemies[2].aiType = WAITANDGO;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].speed = 1.0f;
    state.enemies[2].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[3].textureID = enemyTextureID;
    state.enemies[3].position = glm::vec3(5,-6.0, 0);

    state.enemies[3].entityType = ENEMY;
    //state.enemies[3].aiType = WAITANDG3
    state.enemies[3].aiState = IDLE;
    state.enemies[3].speed = 1.0f;
    state.enemies[3].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[4].textureID = enemyTextureID;
    state.enemies[4].position = glm::vec3(7,-6.0, 0);

    state.enemies[4].entityType = ENEMY;
    state.enemies[4].aiType = WAITANDGO;
    state.enemies[4].aiState = IDLE;
    state.enemies[4].speed = 1.0f;
    state.enemies[4].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[5].textureID = enemyTextureID;
    state.enemies[5].position = glm::vec3(9,-6.0, 0);

    state.enemies[5].entityType = ENEMY;
    state.enemies[5].aiType = WAITANDGO;
    state.enemies[5].aiState = IDLE;
    state.enemies[5].speed = 1.0f;
    state.enemies[5].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[6].textureID = enemyTextureID;
    state.enemies[6].position = glm::vec3(13.5,-3.0, 0);

    state.enemies[6].entityType = ENEMY;
    state.enemies[6].aiType = WAITANDGO;
    state.enemies[6].aiState = IDLE;
    state.enemies[6].speed = 1.0f;
    state.enemies[6].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[7].textureID = enemyTextureID;
    state.enemies[7].position = glm::vec3(15.5,0, 0);

    state.enemies[7].entityType = ENEMY;
    //state.enemies[7].aiType = WAITANDGO;
    state.enemies[7].aiState = IDLE;
    state.enemies[7].speed = 1.0f;
    state.enemies[7].acceleration = glm::vec3(0,-3,0);
    
    state.enemies[8].textureID = enemyTextureID;
    state.enemies[8].position = glm::vec3(17.5,3, 0);

    state.enemies[8].entityType = ENEMY;
    //state.enemies[8].aiType = WAITANDGO;
    state.enemies[8].aiState = IDLE;
    state.enemies[8].speed = 1.0f;
    state.enemies[8].acceleration = glm::vec3(0,-3,0);
    
   
}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    
    for (size_t i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    }
    if (state.player->position.x >= 20) {
        state.nextScene = 4;
    }
}
void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    
    for (size_t i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}
