//
// Created by HK on 2024/5/26.
//

#include "ApiGLES.h"
#include "gles.h"

#define INFO_GL_STRING(name) LOGI(#name": %s", glGetString(name))

static int gl_state_int[4];
#define INFO_GL_INT(name) glGetIntegerv(name, gl_state_int); LOGI(#name": %d", gl_state_int[0])
#define INFO_GL_STATE_INT2(name) glGetIntegerv(name, gl_state_int); LOGI(#name": %d, %d", gl_state_int[0], gl_state_int[1])
#define INFO_GL_STATE_INT3(name) glGetIntegerv(name, gl_state_int); LOGI(#name": %d, %d, %d", gl_state_int[0], gl_state_int[1], gl_state_int[2])

void ApiGLES::GetDeviceInfo() {
    INFO_GL_STRING(GL_VENDOR);
    INFO_GL_STRING(GL_RENDERER);
    INFO_GL_STRING(GL_VERSION);
    INFO_GL_STRING(GL_SHADING_LANGUAGE_VERSION);
    INFO_GL_STRING(GL_EXTENSIONS);

    INFO_GL_INT(GL_PACK_ALIGNMENT);
    INFO_GL_INT(GL_UNPACK_ALIGNMENT);

    INFO_GL_INT(GL_MAX_TEXTURE_IMAGE_UNITS);
    INFO_GL_INT(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    INFO_GL_INT(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);

    INFO_GL_INT(GL_MAX_VERTEX_UNIFORM_VECTORS);
    INFO_GL_INT(GL_MAX_FRAGMENT_UNIFORM_VECTORS);
    INFO_GL_INT(GL_MAX_VARYING_VECTORS);

    INFO_GL_INT(GL_MAX_TEXTURE_SIZE);
    INFO_GL_INT(GL_MAX_CUBE_MAP_TEXTURE_SIZE);
    INFO_GL_INT(GL_MAX_RENDERBUFFER_SIZE);
    INFO_GL_INT(GL_MAX_VIEWPORT_DIMS);

    INFO_GL_INT(GL_MAX_VERTEX_ATTRIBS);

    INFO_GL_INT(GL_NUM_COMPRESSED_TEXTURE_FORMATS);
    INFO_GL_INT(GL_NUM_SHADER_BINARY_FORMATS);
}

void ApiGLES::GetDeviceState() {
    INFO_GL_INT(GL_ARRAY_BUFFER_BINDING);
    INFO_GL_INT(GL_ACTIVE_TEXTURE);
}

GLuint ApiGLES::CreateShader(GLenum shaderType, const char *shaderSrc) {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0)
        return 0;

    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    int compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        int info_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);

        char * info = new char[info_length];
        glGetShaderInfoLog(shader, info_length, nullptr, info);

        LOGE("%s", info);
        delete[] info;

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint ApiGLES::CreateProgram(GLuint vs, GLuint fs) {
    GLuint program = glCreateProgram();
    if (vs)
        glAttachShader(program, vs);

    if (fs)
        glAttachShader(program, fs);

    return program;
}

void ApiGLES::LinkProgram(GLuint program) {
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint info_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
        char * info = new char[info_length];

        glGetProgramInfoLog(program, info_length, NULL, info);
        LOGE("%s", info);
        delete[] info;
    } else {
        // try get and save program binary here
    }
}


static int program_state_int;
#define INFO_GL_PROGRAM_INT(name) glGetProgramiv(program, name, &program_state_int); LOGI("%s(#%d): %d", #name, program, program_state_int)
void ApiGLES::ShowProgramInfo(GLuint program) {
    INFO_GL_PROGRAM_INT(GL_ACTIVE_ATTRIBUTES);
    INFO_GL_PROGRAM_INT(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
    INFO_GL_PROGRAM_INT(GL_ACTIVE_UNIFORM_BLOCKS);
    INFO_GL_PROGRAM_INT(GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH);
    INFO_GL_PROGRAM_INT(GL_ACTIVE_UNIFORMS);
    INFO_GL_PROGRAM_INT(GL_ACTIVE_UNIFORM_MAX_LENGTH);
    INFO_GL_PROGRAM_INT(GL_ATTACHED_SHADERS);
    INFO_GL_PROGRAM_INT(GL_DELETE_STATUS);
    INFO_GL_PROGRAM_INT(GL_INFO_LOG_LENGTH);
    INFO_GL_PROGRAM_INT(GL_LINK_STATUS);
    INFO_GL_PROGRAM_INT(GL_PROGRAM_BINARY_RETRIEVABLE_HINT);
    INFO_GL_PROGRAM_INT(GL_TRANSFORM_FEEDBACK_BUFFER_MODE);
    INFO_GL_PROGRAM_INT(GL_TRANSFORM_FEEDBACK_VARYINGS);
    INFO_GL_PROGRAM_INT(GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH);
    INFO_GL_PROGRAM_INT(GL_VALIDATE_STATUS);
}