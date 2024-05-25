//
// Created by HK on 2024/5/26.
//

#ifndef GLES_APIGLES_H
#define GLES_APIGLES_H
#include <GLES3/gl3.h>

class ApiGLES {
public:
    void GetDeviceInfo();
    void GetDeviceState();

    // shader
    GLuint CreateShader(GLenum shaderType, const char * shaderSrc);
    GLuint CreateProgram(GLuint vs, GLuint fs);
    void LinkProgram(GLuint program);
    void ShowProgramInfo(GLuint program);

    // vao

};


#endif //GLES_APIGLES_H
