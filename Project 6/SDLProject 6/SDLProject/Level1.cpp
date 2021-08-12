#include "Level1.h"
#define OBJECT_COUNT 181
#define ENEMY_COUNT 4
#define WIN_COUNT 10

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-9.5, 0.5, 8.0);
    state.player->acceleration = glm::vec3(0, 0, -2);
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

    // beacon blocks
    state.objects[1].position = glm::vec3(9.5,0.5,-9.5);
    state.objects[2].position = glm::vec3(9.5,1.5,-9.5);
    state.objects[3].position = glm::vec3(9.5,2.5,-9.5);
    state.objects[4].position = glm::vec3(9.5,3.5,-9.5);
    state.objects[5].position = glm::vec3(9.5,4.5,-9.5);
    state.objects[6].position = glm::vec3(9.5,5.5,-9.5);
    state.objects[7].position = glm::vec3(9.5,6.5,-9.5);
    state.objects[8].position = glm::vec3(9.5,7.5,-9.5);
    state.objects[9].position = glm::vec3(9.5,8.5,-9.5);
    
    // wall blocks
    for (size_t i = 10; i < OBJECT_COUNT; i++) {
        state.objects[i].textureID = wallTextureID;
        state.objects[i].mesh = wallMesh;
        state.objects[i].entityType = WALL;
    }
    state.objects[10].position = glm::vec3(-8.0,0.5,9.5);
    state.objects[11].position = glm::vec3(-8.0,0.5,8.5);
    state.objects[12].position = glm::vec3(-8.0,0.5,7.5);
    state.objects[13].position = glm::vec3(-8.0,0.5,6.5);
    state.objects[14].position = glm::vec3(-8.0,0.5,5.5);
    state.objects[15].position = glm::vec3(-8.0,0.5,4.5);
    state.objects[16].position = glm::vec3(-8.0,0.5,3.5);
    state.objects[17].position = glm::vec3(-8.0,0.5,2.5);
    state.objects[18].position = glm::vec3(-8.0,0.5,1.5);
    
    state.objects[19].position = glm::vec3(-5.0,0.5,0.5);
    
    state.objects[20].position = glm::vec3(-8.0,0.5,-1.5);
    state.objects[21].position = glm::vec3(-8.0,0.5,-2.5);
    state.objects[22].position = glm::vec3(-8.0,0.5,-3.5);
    state.objects[23].position = glm::vec3(-8.0,0.5,-4.5);
    state.objects[24].position = glm::vec3(-8.0,0.5,-5.5);
    state.objects[25].position = glm::vec3(-8.0,0.5,-6.5);
    state.objects[26].position = glm::vec3(-8.0,0.5,-7.5);
    state.objects[27].position = glm::vec3(-8.0,0.5,-8.5);
    state.objects[28].position = glm::vec3(-8.0,0.5,-9.5);
    
    state.objects[29].position = glm::vec3(-5.0,0.5,1.5);
    state.objects[30].position = glm::vec3(-5.0,0.5,2.5);
    state.objects[31].position = glm::vec3(-5.0,0.5,3.5);
    state.objects[32].position = glm::vec3(-5.0,0.5,4.5);
    state.objects[33].position = glm::vec3(-5.0,0.5,5.5);
    state.objects[34].position = glm::vec3(-5.0,0.5,6.5);
    state.objects[35].position = glm::vec3(-5.0,0.5,7.5);
    state.objects[36].position = glm::vec3(-5.0,0.5,8.5);
    state.objects[37].position = glm::vec3(-5.0,0.5,9.5);
    state.objects[38].position = glm::vec3(-5.0,0.5,-1.5);
    state.objects[39].position = glm::vec3(-5.0,0.5,-2.5);
    state.objects[40].position = glm::vec3(-5.0,0.5,-3.5);
    state.objects[41].position = glm::vec3(-5.0,0.5,-4.5);
    state.objects[42].position = glm::vec3(-5.0,0.5,-5.5);
    state.objects[43].position = glm::vec3(-5.0,0.5,-6.5);
    
    state.objects[44].position = glm::vec3(-5.0,0.5,0.25);
    state.objects[45].position = glm::vec3(-5.0,0.5,-0.3);
    
    state.objects[46].position = glm::vec3(-5.0,0.5,-9.5);
    
    state.objects[47].position = glm::vec3(-6.0,0.5,-9.5);
    state.objects[48].position = glm::vec3(-7.0,0.5,-9.5);
    
    state.objects[49].position = glm::vec3(-5.0,0.5,0.5);
    
    state.objects[50].position = glm::vec3(-4.0,0.5,-9.5);
    state.objects[51].position = glm::vec3(-3.0,0.5,-9.5);
    state.objects[52].position = glm::vec3(-2.0,0.5,-9.5);
    state.objects[53].position = glm::vec3(-1.0,0.5,-9.5);
    state.objects[54].position = glm::vec3(0.0,0.5,-9.5);
    state.objects[55].position = glm::vec3(1.0,0.5,-9.5);
    state.objects[56].position = glm::vec3(2.0,0.5,-9.5);
    state.objects[57].position = glm::vec3(3.0,0.5,-9.5);
    state.objects[58].position = glm::vec3(4.0,0.5,-9.5);
    state.objects[59].position = glm::vec3(5.0,0.5,-9.5);
    state.objects[60].position = glm::vec3(6.0,0.5,-9.5);
    
    state.objects[61].position = glm::vec3(5.0,0.5,-9.5);
    state.objects[62].position = glm::vec3(5.0,0.5,-9.5);
    state.objects[63].position = glm::vec3(5.0,0.5,-9.5);
    state.objects[64].position = glm::vec3(5.0,0.5,-9.5);
    state.objects[65].position = glm::vec3(5.0,0.5,-9.5);
    state.objects[66].position = glm::vec3(5.0,0.5,-9.5);
    state.objects[67].position = glm::vec3(5.0,0.5,-9.5);
    state.objects[68].position = glm::vec3(5.0,0.5,-9.5);
    state.objects[69].position = glm::vec3(5.0,0.5,-9.5);
    
    state.objects[70].position = glm::vec3(9.5,0.5,-8.5);
    state.objects[71].position = glm::vec3(9.5,0.5,-7.5);
    state.objects[72].position = glm::vec3(9.5,0.5,-6.5);
    state.objects[73].position = glm::vec3(9.5,0.5,-5.5);
    state.objects[74].position = glm::vec3(9.5,0.5,-4.5);
    state.objects[75].position = glm::vec3(9.5,0.5,-3.5);
    state.objects[76].position = glm::vec3(9.5,0.5,-2.5);
    state.objects[77].position = glm::vec3(9.5,0.5,-1.5);
    state.objects[78].position = glm::vec3(9.5,0.5,-0.5);
    state.objects[79].position = glm::vec3(9.5,0.5,0.5);
    
    state.objects[80].position = glm::vec3(9.5,0.5,1.5);
    state.objects[81].position = glm::vec3(9.5,0.5,2.5);
    state.objects[82].position = glm::vec3(9.5,0.5,3.5);
    state.objects[83].position = glm::vec3(9.5,0.5,4.5);
    state.objects[84].position = glm::vec3(9.5,0.5,5.5);
    state.objects[85].position = glm::vec3(9.5,0.5,6.5);
    state.objects[86].position = glm::vec3(9.5,0.5,7.5);
    state.objects[87].position = glm::vec3(9.5,0.5,8.5);
    state.objects[88].position = glm::vec3(9.5,0.5,9.5);
    state.objects[89].position = glm::vec3(9.5,0.5,-8.5);
    
    state.objects[90].position = glm::vec3(6.5, 0.5, -8.5);
    state.objects[91].position = glm::vec3(6.5, 0.5, -7.5);
    state.objects[92].position = glm::vec3(6.5, 0.5, -6.5);
    state.objects[93].position = glm::vec3(6.5, 0.5, -5.5);
    state.objects[94].position = glm::vec3(6.5, 0.5, -4.5);
    state.objects[95].position = glm::vec3(6.5, 0.5, -3.5);
    state.objects[96].position = glm::vec3(6.5, 0.5, -2.5);
    state.objects[97].position = glm::vec3(6.5, 0.5, -1.5);
    state.objects[98].position = glm::vec3(6.5, 0.5, -0.5);
    state.objects[99].position = glm::vec3(6.5, 0.5, 0.5);
    state.objects[100].position = glm::vec3(6.5, 0.5, 1.5);
    state.objects[101].position = glm::vec3(6.5, 0.5, 2.5);
    state.objects[102].position = glm::vec3(6.5, 0.5, 3.5);
    state.objects[103].position = glm::vec3(6.5, 0.5, 4.5);
    state.objects[104].position = glm::vec3(6.5, 0.5, 5.5);
    state.objects[105].position = glm::vec3(6.5, 0.5, 6.5);
    state.objects[106].position = glm::vec3(6.5, 0.5, 9.5);
    state.objects[107].position = glm::vec3(2.5, 0.5, 9.5);
    state.objects[108].position = glm::vec3(3.5, 0.5, 9.5);
    state.objects[109].position = glm::vec3(4.5, 0.5, 9.5);
    state.objects[110].position = glm::vec3(1.5, 0.5, 9.5);
    state.objects[111].position = glm::vec3(0.5, 0.5, 9.5);
    state.objects[112].position = glm::vec3(5.5, 0.5, 9.5);
    state.objects[113].position = glm::vec3(6.5, 0.5, 9.5);
    state.objects[114].position = glm::vec3(7.5, 0.5, 9.5);
    state.objects[115].position = glm::vec3(8.5, 0.5, 9.5);
    state.objects[116].position = glm::vec3(9.5, 0.5, 9.5);
    state.objects[117].position = glm::vec3(-0.5, 0.5, 9.5);
    state.objects[118].position = glm::vec3(-1.5, 0.5, 9.5);
    state.objects[119].position = glm::vec3(-2.5, 0.5, 9.5);
    state.objects[120].position = glm::vec3(-3.5, 0.5, 9.5);
    
    state.objects[121].position = glm::vec3(-2.0, 0.5, 6.5);
    state.objects[122].position = glm::vec3(-2.0, 0.5, 6.5);
    state.objects[123].position = glm::vec3(-2.0, 0.5, 6.5);
    state.objects[124].position = glm::vec3(-2.0, 0.5, 5.5);
    state.objects[125].position = glm::vec3(-2.0, 0.5, 4.5);
    state.objects[126].position = glm::vec3(-2.0, 0.5, 3.5);
    state.objects[127].position = glm::vec3(-2.0, 0.5, 2.5);
    state.objects[128].position = glm::vec3(-2.0, 0.5, 1.5);
    state.objects[129].position = glm::vec3(-2.0, 0.5, 0.5);
    state.objects[130].position = glm::vec3(-2.0, 0.5, -0.5);
    state.objects[131].position = glm::vec3(-2.0, 0.5, -1.5);
    state.objects[132].position = glm::vec3(-2.0, 0.5, -2.5);
    state.objects[133].position = glm::vec3(-2.0, 0.5, -6.5);
    state.objects[134].position = glm::vec3(-2.0, 0.5, -6.5);
    state.objects[135].position = glm::vec3(-2.0, 0.5, -5.5);
    state.objects[136].position = glm::vec3(-2.0, 0.5, -6.5);
    state.objects[137].position = glm::vec3(-2.0, 0.5, -6.5);
    state.objects[138].position = glm::vec3(-2.0, 0.5, -6.5);
    state.objects[139].position = glm::vec3(-2.0, 0.5, -5.5);
    
    state.objects[140].position = glm::vec3(-1.0, 0.5, 6.5);
    state.objects[141].position = glm::vec3(0.0, 0.5, 6.5);
    state.objects[142].position = glm::vec3(1.0, 0.5, 6.5);
    state.objects[143].position = glm::vec3(2.0, 0.5, 6.5);
    state.objects[144].position = glm::vec3(3.0, 0.5, 6.5);
    state.objects[145].position = glm::vec3(4.0, 0.5, 6.5);
    state.objects[146].position = glm::vec3(5.0, 0.5, 6.5);
    state.objects[147].position = glm::vec3(6.0, 0.5, 6.5);
    state.objects[148].position = glm::vec3(7.0, 0.5, 6.5);
    state.objects[149].position = glm::vec3(7.0, 0.5, 6.5);
    state.objects[150].position = glm::vec3(-4.5, 0.5, 9.5);
    
    state.objects[151].position = glm::vec3(1.5, 0.5, 3.5);
    state.objects[152].position = glm::vec3(1.5, 0.5, 3.5);
    state.objects[153].position = glm::vec3(1.5, 0.5, 3.5);
    state.objects[154].position = glm::vec3(1.5, 0.5, 2.5);
    state.objects[155].position = glm::vec3(1.5, 0.5, 1.5);
    state.objects[156].position = glm::vec3(1.5, 0.5, 0.5);
    state.objects[157].position = glm::vec3(1.5, 0.5, -0.5);
    state.objects[158].position = glm::vec3(1.5, 0.5, -1.5);
    state.objects[159].position = glm::vec3(1.5, 0.5, -2.5);
    state.objects[160].position = glm::vec3(1.5, 0.5, -3.5);
    state.objects[161].position = glm::vec3(1.5, 0.5, -4.5);
    state.objects[162].position = glm::vec3(1.5, 0.5, -5.5);
    state.objects[163].position = glm::vec3(1.5, 0.5, -5.5);
    
    state.objects[164].position = glm::vec3(2.5, 0.5, -5.5);
    
    state.objects[165].position = glm::vec3(0.5, 0.5, -5.5);
    state.objects[166].position = glm::vec3(-0.5, 0.5,-5.5);
    state.objects[167].position = glm::vec3(-1.5, 0.5,-5.5);
    state.objects[168].position = glm::vec3(1.5, 0.5, 2.5);
    state.objects[169].position = glm::vec3(1.5, 0.5, 2.5);
    
    state.objects[170].position = glm::vec3(2.5, 0.5, 3.5);
    state.objects[171].position = glm::vec3(3.5, 0.5, 3.5);
    state.objects[172].position = glm::vec3(4.5, 0.5, 3.5);
    state.objects[173].position = glm::vec3(5.5, 0.5, 3.5);
    state.objects[174].position = glm::vec3(6.5, 0.5, 3.5);
    
    state.objects[175].position = glm::vec3(-9.5, 0.5, 9.5);
    state.objects[176].position = glm::vec3(-7.5, 0.5, 9.5);
    state.objects[177].position = glm::vec3(-6.5, 0.5, 9.5);
    state.objects[178].position = glm::vec3(-5.5, 0.5, 9.5);
    state.objects[179].position = glm::vec3(1.5, 0.5, 3.5);
    state.objects[180].position = glm::vec3(-8.5, 0.5, 9.5);

    state.enemies = new Entity[ENEMY_COUNT];
    
    GLuint enemyTextureID = Util::LoadTexture("character_0008.png");
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
         state.enemies[i].billboard = true;
         state.enemies[i].textureID = enemyTextureID;
    }
    state.enemies[0].position = glm::vec3(-9.5,0.5,-8);
    state.enemies[1].position = glm::vec3(3,0.5,0);
    state.enemies[2].position = glm::vec3(5,0.5,0);
    state.enemies[3].position = glm::vec3(4,0.5,0);
    
}

void Level1::Update(float deltaTime) {
    
    if (state.player->position.z <= -9) {
        state.nextScene = 2;
    }
    else if (state.player->position.x < -10) {
        state.player->isActive = false;
    }
    
    state.player->Update(deltaTime, state.player, state.objects, OBJECT_COUNT, state.enemies,ENEMY_COUNT);
    
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime ,state.player, state.objects, OBJECT_COUNT,state.enemies,ENEMY_COUNT);
    }
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime ,state.player, state.objects, OBJECT_COUNT, state.enemies, ENEMY_COUNT);
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
