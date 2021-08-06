#include "Level1.h"
#define OBJECT_COUNT 4
#define ENEMY_COUNT 1

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.75f, 0);
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
    
//    GLuint crateTextureID = Util::LoadTexture("crate1_diffuse.png");
//    Mesh* crateMesh = new Mesh();
//    crateMesh->LoadOBJ("cube.obj", 1);
//
//    state.objects[1].textureID = crateTextureID;
//    state.objects[1].mesh = crateMesh;
//    state.objects[1].position = glm::vec3(0,0.5,-5);
//    state.objects[1].entityType = CRATE;
//
//    state.objects[2].textureID = crateTextureID;
//    state.objects[2].mesh = crateMesh;
//    state.objects[2].position = glm::vec3(-1,0.5,-5);
//    state.objects[2].entityType = CRATE;
    
//    state.objects[3].textureID = crateTextureID;
//    state.objects[3].mesh = crateMesh;
//    state.objects[3].position = glm::vec3(0,1.5,-5);
//    state.objects[3].entityType = CRATE;
    
    state.enemies = new Entity[ENEMY_COUNT];
    
    GLuint enemyTextureID = Util::LoadTexture("minotaur.png");
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
         state.enemies[i].billboard = true;
         state.enemies[i].textureID = enemyTextureID;
         state.enemies[i].position = glm::vec3(rand() % 20 - 10, 0.5, rand() % 20 - 10);
         state.enemies[i].rotation = glm::vec3(0, 0, 0);
         state.enemies[i].acceleration = glm::vec3(0, 0, 0);
    }
}

void Level1::Update(float deltaTime) {
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
