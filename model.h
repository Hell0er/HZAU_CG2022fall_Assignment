#pragma once
#include "Glm/glm.hpp"
#include "misc.h"
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

//模型读取头文件

/*
        存储顶点信息的结构体
        float position   顶点位置信息 v
        float texcoord   顶点纹理坐标 vt
        float normal     顶点法线信息 vn
*/
struct VertexData {
  glm::vec3 position; //顶点位置信息
  glm::vec2 texcoord; //顶点纹理坐标
  glm::vec3 normal;   //顶点法线信息
};

/*
        读取obj文件，获取obj顶点信息
        @param  filePath   obj文件名
        @param  indexes    顶点的索引信息
        @param  vertexCount   顶点个数
        @param  indexCount    索引个数
        @retval 存储obj信息的VertexData
*/
VertexData *LoadObjModel(const char *filePath, unsigned int **indexes,
                         int &vertexCount, int &indexCount);
//读取完obj文件后返回指向一个结构体数组的指针，保存了obj模型所有的顶点位置信息、顶点纹理坐标、顶点法线信息