package com.example.gles;

import android.content.res.AssetManager;

public class NativeLibrary
{
    static
    {
        System.loadLibrary("gles");
    }
    public static native void InitAssetManager(AssetManager assetManager, String dir);
    public static native void step();

    public static native boolean onTouch(int event, float x, float y);
}
