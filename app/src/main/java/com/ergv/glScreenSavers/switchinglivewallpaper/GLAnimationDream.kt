package com.ergv.glScreenSavers.switchinglivewallpaper

import android.app.ActivityManager
import android.opengl.GLSurfaceView
import android.service.dreams.DreamService
import com.ergv.glScreenSavers.MandelbrotSet.MandelbrotSetRenderer

class GLAnimationDream : DreamService() {


    private var mGLSurfaceView: GLSurfaceView? = null

    override fun onCreate(){
        super.onCreate( )
        mGLSurfaceView = GLSurfaceView(this)

        // Check if the system supports OpenGL ES 3.0.
        val activityManager = getSystemService(ACTIVITY_SERVICE) as ActivityManager
        val configurationInfo = activityManager.deviceConfigurationInfo
        val supportsEs3 = configurationInfo.reqGlEsVersion >= 0x30000

        if (supportsEs3) {
            // Request an OpenGL ES 3.0 compatible context.
            mGLSurfaceView!!.setEGLContextClientVersion(3)

            // Set the renderer to our demo renderer, defined below.
            mGLSurfaceView!!.setRenderer(MandelbrotSetRenderer(this))
        } else {
            // This is where you could create an OpenGL ES 1.x compatible
            // renderer if you wanted to support both ES 1 and ES 2.
            return
        }


    }

    override fun onDestroy() {
        super.onDestroy()
    }
    override fun onDreamingStarted() {
        super.onDreamingStarted()
        mGLSurfaceView!!.onResume()
    }

    override fun onDreamingStopped() {
        super.onDreamingStopped()
    }

    override fun onAttachedToWindow() {
        super.onAttachedToWindow()

        isInteractive = false
        isFullscreen = true
        setContentView(mGLSurfaceView)

    }




    override fun onDetachedFromWindow() {

        super.onDetachedFromWindow()

    }


}