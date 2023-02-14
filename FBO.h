#pragma once

#include "GL/glew.h"
#include <map>
#include <string>
#include <vector>


/*
        Frame buffer object
        此程序中主要用于获取深度图
*/
class FrameBufferObject {
public:
  GLuint mFrameBufferObject;
  GLint mPrevFrameBuffer;
  std::map<std::string, GLuint> mBuffers;
  std::vector<GLenum> mDrawBuffers;

public:
  /*
          构造函数 gen一个frame buffer object
  */
  FrameBufferObject();

  /*
          绑定一个colorbuffer, 并把其放入map
          将attachment放入vector中
          @param bufferName   colorBuffer的名字,  也就是map中的key
          @param attachment   GL_COLOR_ATTACHMENT
          @width  colorbuffer的宽度， 设为屏幕宽度即可
          @height colorbuffer的高度
  */
  void AttachColorBuffer(const char *bufferName, GLenum attachment, int width,
                         int height);

  /*
          绑定一个depthbuffer, 并把其放入map
          @param bufferName   depthBuffer的名字,  也就是map中的key
          @param width  宽度
          @param height 高度
  */
  void AttachDepthBuffer(const char *bufferName, int width, int height);

  /*
          设置结束，绑定framebuffer。并将帧数据绘制到attachment中
  */
  void Finish();

  /*
          绑定当前framebuffer为此framebuffer
          清空当前framebuffer的缓冲区
  */
  void Bind();

  /*
          解绑定
  */
  void Unbind();

  /*
          获取所用使用的buffer
          @param bufferName  buffer的名     map的key
          @retval  GLuint   buffer名所对应的buffer    map的value
  */
  GLuint GetBuffer(const char *bufferName);
};