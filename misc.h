#pragma once
#include "GL/glew.h"

/*
        创建buffer object
        @param  bufferType   buffer的类型
        @param  size         buffer大小
        @param  usage        GL_STATIC_DRAW
        @param  data         buffer的数据
        @retval  创建好的buffer object
        //bufferType 传入参数区分IBO=索引绘制GL_ELEMENT_ARRAY_BUFFER
   VBO==GL_ARRAY_BUFFER  size是模型顶点数组的大小   data就是顶点数组
   usage是GL_STATIC_DRAW /GL_DYNAMIC_DRAW/GL_STREAM_DRAW
*/
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage,
                          void *data = nullptr);

/*
        获取文件内容
        @param  path   文件路径及文件名
        @retval  文件内容
*/
char *LoadFileContent(const char *path);

/*
        创建纹理
        @param  imgFileName   图片名
        @retval  创建好的纹理
*/
GLuint CreateTexture(const char *imgFileName);

/*
        创建gpu program
        @param  vsShaderPath   vertex shader文件名
        @param  fsShaderPath   fragment shader文件名
        @retval  创建好的gpu program
*/
GLuint CreateGPUProgram(const char *vsShaderPath, const char *fsShaderPath);