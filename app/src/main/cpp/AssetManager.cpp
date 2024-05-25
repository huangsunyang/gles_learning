//
// Created by HK on 2024/5/26.
//

#include "gles.h"
#include "AssetManager.h"

AssetManager* AssetManager::instance = nullptr;

AssetManager* AssetManager::GetInstance() {
    return instance;
}

AssetManager::AssetManager(JNIEnv *env, jobject assetManager) {
    javaAssetManager = AAssetManager_fromJava(env, assetManager);
    instance = this;
}

AAsset* AssetManager::OpenFile(const char *path, int offset, int whence) {
    AAsset *file = AAssetManager_open(javaAssetManager, path, AASSET_MODE_UNKNOWN);
    if (file == nullptr) {
        LOGE("path %s not exists", path);
    }

    if (AAsset_seek(file, offset, whence) == -1) {
        LOGE("path %s seek to %d(%d) failed", path, offset, whence);
        AAsset_close(file);
        file = nullptr;
    }

    return file;
}

bool AssetManager::ReadFile(const char * path, char * buffer, int length, int offset, int whence) {
    AAsset * file = OpenFile(path, offset, whence);
    if (file == nullptr)
        return false;

    if (length < 0)
        length = AAsset_getRemainingLength(file);
    else
        length = std::min(length, (int)AAsset_getRemainingLength(file));

    int count = 0;
    while (count < length) {
        int part = AAsset_read(file, buffer, length - count);
        if (part < 0)
            AAsset_close(file);
        else if (part == 0)
            break;
        count += part;
    }

    if (count < length) {
        LOGE("read %s error, expected %d, but got %d", path, length, count);
        AAsset_close(file);
        return false;
    }

    AAsset_close(file);
    return true;
}

std::string AssetManager::ReadFile(const char *path, int length, int offset, int whence) {
    if (length < 0)
        length = GetFileSize(path, offset, whence);
    else
        length = std::min(length, GetFileSize(path, offset, whence));

    std::string result(length, '\0');
    if (ReadFile(path, &result[0], length, offset, whence))
        return result;
    return "";
}

int AssetManager::GetFileSize(const char *path, int offset, int whence) {
    AAsset * file = OpenFile(path, offset, whence);
    if (file == nullptr)
        return 0;

    return AAsset_getRemainingLength(file);
}

bool AssetManager::FileExists(const char *path) {
    AAsset *file = AAssetManager_open(javaAssetManager, path, AASSET_MODE_UNKNOWN);
    if (file == nullptr)
        return false;
    AAsset_close(file);
    return true;
}

bool AssetManager::DirectoryExists(const char *path) {
    AAssetDir *dir = AAssetManager_openDir(javaAssetManager, path);
    if (dir == nullptr)
        return false;
    AAssetDir_close(dir);
    return true;
}

bool AssetManager::PathExists(const char *path) {
    return FileExists(path) || DirectoryExists(path);
}

vector<string> AssetManager::GetFiles(const char *path) {
    vector<string> results;
    AAssetDir *dir = AAssetManager_openDir(javaAssetManager, path);
    if (dir == nullptr) {
        LOGE("path %s not exists", path);
        return results;
    }

    const char * fileName;
    while ((fileName = AAssetDir_getNextFileName(dir)) != nullptr)
        results.push_back(fileName);

    AAssetDir_close(dir);
    return results;
}
