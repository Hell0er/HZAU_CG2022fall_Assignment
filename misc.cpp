#define _CRT_SECURE_NO_DEPRECATE
#include "misc.h"
#include "SOIL2/SOIL2.h"
#include <stdio.h>
#include <stdlib.h>

void printShaderInfoLog(GLuint obj) {
  int infologLength = 0;
  int charsWritten = 0;
  char *infoLog;

  glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

  if (infologLength > 0) {
    infoLog = (char *)malloc(infologLength);
    glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
    printf("%s\n", infoLog);
    free(infoLog);
  }
}

// bufferType 传入参数区分VEO VBO==GL_ARRAY_BUFFER  size是模型顶点数组的大小
// data就是顶点数组   usage是GL_STATIC_DRAW /GL_DYNAMIC_DRAW/GL_STREAM_DRAW
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage,
                          void *data /* = nullptr */) {
  // unsigned int 定义一个缓冲器对象
  GLuint object;
  glGenBuffers(1, &object);         //生成缓冲器对象
  glBindBuffer(bufferType, object); //绑定缓冲器对象
  glBufferData(
      bufferType, size, data,
      usage); // glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数，交给缓冲进行管理
  glBindBuffer(bufferType, 0); //解绑缓冲器,等到需要激活着色器程序时再绑定使用
  return object;
}

//获取文本内容并返回
char *LoadFileContent(const char *path) {
  FILE *pFile = fopen(path, "rb"); // rb:读方式打开一个二进制文件
  if (pFile) {
    fseek(pFile, 0, SEEK_END);
    int nLen = ftell(pFile);
    char *buffer = nullptr;
    if (nLen != 0) {
      buffer = new char[nLen + 1];
      // rewind(pFile);
      fseek(pFile, 0, SEEK_SET);
      fread(buffer, nLen, 1, pFile);
      buffer[nLen] = '\0';
    } else {
      printf("fail to load file %s ,content len is 0\n", path);
    }
    fclose(pFile);
    return buffer;
  } else {
    printf("fail to open file %s\n", path);
  }
  fclose(pFile);
  return nullptr;
}

// GLuint CreateTexture(const char* imgFileName)
//{
//	//创建纹理对象 一些参数巴拉巴拉的
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
// GL_LINEAR);//纹理目标是GL_TEXTURE_2D
//	//优化代码 设置多级渐远纹理
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
// GL_LINEAR_MIPMAP_LINEAR);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
////第二个参数需要我们指定设置的选项与应用的纹理轴
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
////最后一个参数需要我们传递一个环绕方式(Wrapping)，此处会给当前激活的纹理设定纹理环绕方式为GL_MIRRORED_REPEAT。
//	return SOIL_load_OGL_texture(imgFileName, 0, 0, SOIL_FLAG_POWER_OF_TWO |
// SOIL_FLAG_INVERT_Y);
//}

GLuint CreateTexture(const char *imgFileName) {
  GLuint texture;
  texture = SOIL_load_OGL_texture(imgFileName, SOIL_LOAD_AUTO,
                                  SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

  if (texture == 0)
    return -1;
  //创建纹理对象后就要绑定！不然就没了啊！！！！
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  return texture;
}

GLuint CreateGPUProgram(const char *vsShaderPath, const char *fsShaderPath) {
  GLuint vsShader = glCreateShader(GL_VERTEX_SHADER); //产生单个的着色器对象
  GLuint fsShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *vsCode =
      LoadFileContent(vsShaderPath); //指向对应的着色器GLSL代码文件
  const char *fsCode = LoadFileContent(fsShaderPath);
  glShaderSource(
      vsShader, 1, &vsCode,
      nullptr); // glShaderSource
                // 将真正的着色器字符源码（通过第三个参数）传送给我们的着色器对象，
  glShaderSource(fsShader, 1, &fsCode, nullptr); // ram -> vram
  glCompileShader(vsShader); //随后我们编译着色器程序：
  glCompileShader(fsShader);
  printShaderInfoLog(vsShader);
  printShaderInfoLog(fsShader);
  GLuint program = glCreateProgram();
  //然后附加到着色器程序上
  glAttachShader(program, vsShader);
  glAttachShader(program, fsShader);
  //链接顶点着色器和片元着色器
  glLinkProgram(program);
  glDetachShader(program, vsShader);
  glDetachShader(program, fsShader);
  //删除释放着色器对象
  glDeleteShader(vsShader);
  glDeleteShader(fsShader);
  return program;
}
