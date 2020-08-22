package com.learnopengles.android.JuliaSet;

import android.app.Activity;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.content.res.AssetManager;

/**
 * This class implements our custom renderer. Note that the GL10 parameter passed in is unused for OpenGL ES 2.0
 * renderers -- the static class GLES20 is used instead.
 */
public class JuliaSetRenderer implements GLSurfaceView.Renderer {

	static {
		System.loadLibrary("lesson-lib");
	}

	private Activity mActivity;

	public JuliaSetRenderer(Activity activity) {
		mActivity = activity;
	}

	public static native void nativeSurfaceCreate(AssetManager assetManager);

	public static native void nativeSurfaceChange(int width, int height);

	public static native void nativeDrawFrame();

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		AssetManager assetManager = mActivity.getAssets();
        nativeSurfaceCreate(assetManager);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		nativeSurfaceChange(width, height);
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		nativeDrawFrame();
	}

}