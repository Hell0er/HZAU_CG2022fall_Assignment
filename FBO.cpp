#include "FBO.h"

FrameBufferObject::FrameBufferObject() {
  glGenFramebuffers(1, &mFrameBufferObject);
}
void FrameBufferObject::AttachColorBuffer(const char *bufferName,
                                          GLenum attachment, int width,
                                          int height) {
  GLuint colorBuffer;
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
  glGenTextures(1, &colorBuffer);
  glBindTexture(GL_TEXTURE_2D, colorBuffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, colorBuffer,
                         0);
  mDrawBuffers.push_back(attachment);
  mBuffers.insert(std::pair<std::string, GLuint>(bufferName, colorBuffer));
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBufferObject::AttachDepthBuffer(const char *bufferName, int width,
                                          int height) {
  GLuint depthMap;
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depthMap, 0);
  mBuffers.insert(std::pair<std::string, GLuint>(bufferName, depthMap));
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBufferObject::Finish() {
  int nCount = (int)mDrawBuffers.size();
  if (nCount > 0) {
    GLenum *buffers = new GLenum[nCount];
    int i = 0;
    while (i < nCount) {
      buffers[i] = mDrawBuffers[i];
      i++;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
    glDrawBuffers(nCount, buffers);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}
void FrameBufferObject::Bind() {
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mPrevFrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void FrameBufferObject::Unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, mPrevFrameBuffer);
}
GLuint FrameBufferObject::GetBuffer(const char *bufferName) {
  auto iter = mBuffers.find(bufferName);
  if (iter != mBuffers.end()) {
    return iter->second;
  }
  return 0;
}