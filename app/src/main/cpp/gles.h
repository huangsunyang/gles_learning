//
// Created by Bytedance on 2024/5/20.
//

#ifndef GLES_GLES_H
#define GLES_GLES_H

#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LOG_TAG "gles"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C"
{
JNIEXPORT void JNICALL Java_com_example_gles_NativeLibrary_init(JNIEnv* env, jclass obj, jint width, jint height);
JNIEXPORT void JNICALL Java_com_example_gles_NativeLibrary_step(JNIEnv* env, jclass obj);
JNIEXPORT jboolean JNICALL Java_com_example_gles_NativeLibrary_onTouch(JNIEnv* env, jclass obj, jint event, jfloat x, jfloat y);
};

#endif //GLES_GLES_H
