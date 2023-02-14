#pragma once
#include "GL/freeglut.h"
#include "Glm/ext.hpp"
#include "Glm/glm.hpp"
#include "model.h"
#include <vector>


// 定义全局变量

// matrix
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 normalMatrix;
glm::mat4 lightProjectionMatrix;
glm::mat4 lightViewMatrix;

GLuint shadowMap;
GLuint renderProgram, depthProgram, livingRoomWallProgram;

// 渲染所用参数
GLuint renderPosLoc, renderTexcoordLoc, renderNormalLoc;
GLuint renderMLoc, renderVLoc, renderPLoc, renderNMLoc, renderViewPosLocation;
GLuint renderLightVMatrixLoc, renderLightPMatrixLoc;
GLuint renderLightOnLoc;
GLuint renderTextureLoc, shadowMapLoc;

GLfloat rendersunlight;
GLfloat rendersunx;
GLfloat rendersuny;
GLfloat rendersunz;

GLuint depthPosLoc, depthTexcoordLoc, depthNormalLoc;
GLuint depthMLoc, depthVLoc, depthPLoc;

//----------------------------------------------------
//定义模型相关变量
//----------------------------------------------------

// desk
VertexData *deskVertexData;
int deskVertexCount, deskIndexCount;
unsigned int *deskIndexes;
GLuint deskVbo, deskIbo;
std::vector<GLuint> desks; //纹理
int desksSize;
GLuint deskTexture = 0;

// floor
VertexData *floorVertexData;
int floorVertexCount, floorIndexCount;
unsigned int *floorIndexes;
GLuint floorVbo, floorIbo;
std::vector<GLuint> floors;
int floorsSize;
GLuint floorTexture = 0;

// flower
VertexData *flowerVertexData;
int flowerVertexCount, flowerIndexCount;
unsigned int *flowerIndexes;
GLuint flowerVbo, flowerIbo;
std::vector<GLuint> flowers;
int flowersSize;
GLuint flowerTexture = 0;

// painting1
VertexData *painting1VertexData;
int painting1VertexCount, painting1IndexCount;
unsigned int *painting1Indexes;
GLuint painting1Vbo, painting1Ibo;
std::vector<GLuint> painting1s;
int painting1sSize;
GLuint painting1Texture = 0;

// painting2
VertexData *painting2VertexData;
int painting2VertexCount, painting2IndexCount;
unsigned int *painting2Indexes;
GLuint painting2Vbo, painting2Ibo;
std::vector<GLuint> painting2s;
int painting2sSize;
GLuint painting2Texture = 0;

// plant
VertexData *plantVertexData;
int plantVertexCount, plantIndexCount;
unsigned int *plantIndexes;
GLuint plantVbo, plantIbo;
std::vector<GLuint> plants;
int plantsSize;
GLuint plantTexture = 0;

// balloon
VertexData *balloonVertexData;
int balloonVertexCount, balloonIndexCount;
unsigned int *balloonIndexes;
GLuint balloonVbo, balloonIbo;
std::vector<GLuint> balloons;
int balloonsSize;
GLuint balloonTexture = 0;

// lamp
VertexData *lampVertexData;
int lampVertexCount, lampIndexCount;
unsigned int *lampIndexes;
GLuint lampVbo, lampIbo;
std::vector<GLuint> lamps;
int lampsSize;
GLuint lampTexture = 0;

// cat
VertexData *catVertexData;
int catVertexCount, catIndexCount;
unsigned int *catIndexes;
GLuint catVbo, catIbo;
std::vector<GLuint> cats;
int catsSize;
GLuint catTexture = 0;

// sun
VertexData *sunVertexData;
int sunVertexCount, sunIndexCount;
unsigned int *sunIndexes;
GLuint sunVbo, sunIbo;
std::vector<GLuint> suns;
int sunsSize;
GLuint sunTexture = 0;

// table
VertexData *tableVertexData;
int tableVertexCount, tableIndexCount;
unsigned int *tableIndexes;
GLuint tableVbo, tableIbo;
std::vector<GLuint> tables;
int tablesSize;
GLuint tableTexture = 0;

// wall
VertexData *wallVertexData;
int wallVertexCount, wallIndexCount;
unsigned int *wallIndexes;
GLuint wallVbo, wallIbo;
std::vector<GLuint> walls;
int wallsSize;
GLuint wallTexture = 0;

//----------------------------------------
//定义摄像机相关变量
//----------------------------------------
glm::vec3 cameraPos; //摄像机位置
glm::vec3 cameraCenter;
glm::vec3 cameraForward;

glm::vec3 cameraTarget;    //原点位置
glm::vec3 cameraDirection; //摄像机朝向
glm::vec3 up;              //上向量
glm::vec3 cameraRight;     //右向量
glm::vec3 cameraUp;        //上向量
float cameraSpeed = 1.25f; //移动速度
float fov = 60.0f;         //摄像机视角
