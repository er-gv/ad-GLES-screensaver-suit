package com.ergv.glScreenSavers.geodeticBodies

import android.app.Activity
import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class GeodeticBodiesRenderer(private val mActivity: Activity) : GLSurfaceView.Renderer {
    companion object {
        init {
            System.loadLibrary("lesson-lib")
        }
    }

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        nativeSurfaceCreate(mActivity.assets)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        nativeSurfaceChange(width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        nativeDrawFrame()
    }

    external fun nativeSurfaceCreate(assetManager: AssetManager?)
    external fun nativeSurfaceChange(width: Int, height: Int)
    external fun nativeDrawFrame()
}
