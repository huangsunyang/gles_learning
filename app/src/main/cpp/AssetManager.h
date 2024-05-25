//
// Created by HK on 2024/5/26.
//

#ifndef GLES_ASSETMANAGER_H
#define GLES_ASSETMANAGER_H

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <vector>
#include <string>

using std::vector;
using std::string;

class AssetManager {
public:
    AssetManager(JNIEnv* env, jobject assetManager);
    bool ReadFile(const char* path, char * buffer, int length=-1, int offset=0, int whence=SEEK_SET);
    std::string ReadFile(const char* path, int length=-1, int offset=0, int whence=SEEK_SET);
    int GetFileSize(const char * path, int offset=0, int whence=SEEK_SET);
    bool FileExists(const char * path);
    bool DirectoryExists(const char * path);
    bool PathExists(const char * path);
    vector<string> GetFiles(const char * path);

    static AssetManager * GetInstance();
protected:
    AAsset* OpenFile(const char* path, int offset, int whence);

    AAssetManager * javaAssetManager;
    static AssetManager * instance;
};


#endif //GLES_ASSETMANAGER_H
