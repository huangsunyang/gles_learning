package com.example.gles;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;

class MyGLSurfaceView extends GLSurfaceView {

    private final GLRenderer renderer;

    public MyGLSurfaceView(Context context){
        super(context);

        // Create an OpenGL ES 2.0/3.0 context
        setEGLContextClientVersion(3);

        renderer = new GLRenderer();

        // Set the Renderer for drawing on the GLSurfaceView
        setRenderer(renderer);

        setOnTouchListener(new View.OnTouchListener() {
            public boolean onTouch(View v, MotionEvent event) {
                return NativeLibrary.onTouch(event.getAction(), event.getX(), event.getY());
            }
        });
    }
}

public class GLESExampleActivity extends Activity {

    private GLSurfaceView gLView;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        NativeLibrary.InitAssetManager(getAssets(), "");

        // Create a GLSurfaceView instance and set it
        // as the ContentView for this Activity.
        gLView = new MyGLSurfaceView(this);
        setContentView(gLView);
    }
}

