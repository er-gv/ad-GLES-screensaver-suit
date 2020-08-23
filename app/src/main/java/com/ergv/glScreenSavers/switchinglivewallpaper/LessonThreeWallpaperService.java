package com.ergv.glScreenSavers.switchinglivewallpaper;

import android.opengl.GLSurfaceView.Renderer;


public class LessonThreeWallpaperService extends OpenGLES2WallpaperService {
	@Override
	Renderer getNewRenderer() {
		return null; //new BricksShaderNativeRenderer(this.getApplicationContext());
	}
}
