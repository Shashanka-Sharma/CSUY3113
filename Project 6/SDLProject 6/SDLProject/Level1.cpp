#include "Level1.h"
#define OBJECT_COUNT 10
#define ENEMY_COUNT 4
#define WIN_COUNT 10

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.75f, 10);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.0f;
    
    state.objects = new Entity[OBJECT_COUNT];
    
    GLuint floorTextureID = Util::LoadTexture("floor.jpg");
    Mesh* cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj", 20);
    
    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].entityType = FLOOR;
    state.objects[0].position = glm::vec3(0,-0.25f,0);
    state.objects[0].scale = glm::vec3(20,0.5f,20);
    
    GLuint wallTextureID = Util::LoadTexture("wall.JPG");
    Mesh* wallMesh = new Mesh();
    wallMesh->LoadOBJ("cube.obj", 1);
    
    GLuint winTextureID = Util::LoadTexture("winTile.jpg");
    Mesh* winMesh = new Mesh();
    winMesh->LoadOBJ("cube.obj", 1);
    
    for (size_t i = 1; i < WIN_COUNT; i++) {
        state.objects[i].textureID = winTextureID;
        state.objects[i].mesh = winMesh;
        state.objects[i].entityType = WINTILE;
    }
    
//    for (size_t i = 1; i < WIN_COUNT; i++) {
//        state.objects[i].textureID = winTextureID;
//        state.objects[i].mesh = winMesh;
//        state.objects[i].entityType = WINTILE;
//    }
    state.objects[1].position = glm::vec3(9.5,0.5,-9.5);
    state.objects[2].position = glm::vec3(9.5,1.5,-9.5);
    state.objects[3].position = glm::vec3(9.5,2.5,-9.5);
    state.objects[4].position = glm::vec3(9.5,3.5,-9.5);
    state.objects[5].position = glm::vec3(9.5,4.5,-9.5);
    state.objects[6].position = glm::vec3(9.5,5.5,-9.5);
    state.objects[7].position = glm::vec3(9.5,6.5,-9.5);
    state.objects[8].position = glm::vec3(9.5,7.5,-9.5);
    state.objects[9].position = glm::vec3(9.5,8.5,-9.5);
    
    
    state.enemies = new Entity[ENEMY_COUNT];
    
    GLuint enemyTextureID = Util::LoadTexture("character_0008.png");
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
         state.enemies[i].billboard = true;
         state.enemies[i].textureID = enemyTextureID;
         state.enemies[i].position = glm::vec3(rand() % 20 - 10, 0.5, rand() % 20 - 10);
    }
    state.enemies[0].position = glm::vec3(0,1,0);
    state.enemies[1].position = glm::vec3(-1,1,0);
    state.enemies[2].position = glm::vec3(-2,1,0);
    state.enemies[3].position = glm::vec3(-1,0,0);
    
}

void Level1::Update(float deltaTime) {
    
    if (state.player->position.z <= -9) {
        state.nextScene = 2;
    }
    state.player->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
    
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime ,state.player, state.objects, OBJECT_COUNT);
    }
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime ,state.player, state.objects, OBJECT_COUNT);
    }
    
}

void Level1::Render(ShaderProgram *program) {
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}
