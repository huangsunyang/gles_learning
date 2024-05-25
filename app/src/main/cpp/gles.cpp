// Write C++ code here.
//
// Do not forget to dynamically load the C++ library into your application.
//
// For instance,
//
// In MainActivity.java:
//    static {
//       System.loadLibrary("gles");
//    }
//
// Or, in MainActivity.kt:
//    companion object {
//      init {
//         System.loadLibrary("gles")
//      }
//    }

#include "gles.h"
#include "ApiGLES.h"
#include "AssetManager.h"
#include "OBJ_Loader.h"

#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale
#include "glm/ext/matrix_clip_space.hpp" // glm::perspective
#include "glm/ext/scalar_constants.hpp" // glm::pi
#include "glm/gtc/type_ptr.hpp"

ApiGLES* api = nullptr;
AssetManager* assetManager;
GLuint program = 0;
objl::Loader loader;
GLuint vbo;
GLuint ebo;

void InitApplication() {
    api = new ApiGLES();
    api->GetDeviceInfo();

    std::string vertexShaderSrc = assetManager->ReadFile("shaders/simple.vert");
    std::string fragmentShaderSrc = assetManager->ReadFile("shaders/simple.frag");

    GLuint vertexShader = api->CreateShader(GL_VERTEX_SHADER, vertexShaderSrc.c_str());
    GLuint fragmentShader = api->CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSrc.c_str());
    program = api->CreateProgram(vertexShader, fragmentShader);
    api->LinkProgram(program);
    api->ShowProgramInfo(program);

    // test Load .obj File
    loader.LoadFile("models/box_stack.obj");
    LOGI("mesh count: %d", loader.LoadedMeshes.size());
    LOGI("material count: %d", loader.LoadedMaterials.size());
    for (objl::Mesh& mesh: loader.LoadedMeshes)
        LOGI("mesh name: %s", mesh.MeshName.c_str());

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, loader.LoadedMeshes[2].Vertices.size() * sizeof(objl::Vertex), &loader.LoadedMeshes[2].Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.LoadedMeshes[2].Indices.size() * sizeof(unsigned int), &loader.LoadedMeshes[2].Indices[0], GL_STATIC_DRAW);
}

glm::vec2 Rotate(0.3, 0.3);
float Translate = 15.0f;

void NativeRender() {
    glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    glm::mat4 mvp = Projection * View * Model;

    int location = glGetUniformLocation(program, "mvp");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    int offset = 0;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (const void *) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (const void *) offsetof(objl::Vertex, Normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (const void *) offsetof(objl::Vertex, TextureCoordinate));

    glDrawElements(GL_TRIANGLES, loader.LoadedMeshes[2].Indices.size(), GL_UNSIGNED_INT, 0);

}

void OnTouchEvent(int event, float x, float y)
{
    // LOGI("%d %f %f", event, x, y);
    if (event == -1) {
        api->GetDeviceState();
    }
    Rotate.x += 0.05;
}

extern "C" {
JNIEXPORT void JNICALL Java_com_example_gles_NativeLibrary_step(JNIEnv *env, jclass obj) {
    static bool isFirstFrame = true;
    if (isFirstFrame) {
        InitApplication();
        isFirstFrame = false;
    }

    NativeRender();
}

JNIEXPORT jboolean JNICALL
Java_com_example_gles_NativeLibrary_onTouch(JNIEnv *env, jclass obj, jint event, jfloat x, jfloat y) {
    OnTouchEvent(event, x, y);
    return true;
}

JNIEXPORT void JNICALL Java_com_example_gles_NativeLibrary_InitAssetManager(JNIEnv* env, jclass obj, jobject assetManagerObject) {
    assetManager = new AssetManager(env, assetManagerObject);
}
};
