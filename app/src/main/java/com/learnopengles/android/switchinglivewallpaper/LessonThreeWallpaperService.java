package com.learnopengles.android.switchinglivewallpaper;

import android.opengl.GLSurfaceView.Renderer;

import com.learnopengles.android.bricksShader.BricksShaderNativeRenderer;


public class LessonThreeWallpaperService extends OpenGLES2WallpaperService {
	@Override
	Renderer getNewRenderer() {
		return null; //new BricksShaderNativeRenderer(this.getApplicationContext());
	}
}
