package com.learnopengles.android.Polyhedrons;

import android.app.Activity;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class PolyhedronsRenderer implements GLSurfaceView.Renderer {

        private Activity mActivity;

        static {
            System.loadLibrary("lesson-lib");
        }

        public PolyhedronsRenderer(Activity activity) {
            mActivity = activity;
        }

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            nativeSurfaceCreate(mActivity.getAssets());
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            nativeSurfaceChange(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            nativeDrawFrame();
        }


        public native void nativeSurfaceCreate(AssetManager assetManager);

        public native void nativeSurfaceChange(int width, int height);

        public native void nativeDrawFrame();


}
