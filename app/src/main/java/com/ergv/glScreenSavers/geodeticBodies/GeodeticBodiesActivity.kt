package com.ergv.glScreenSavers.geodeticBodies

import android.app.Activity
import android.app.ActivityManager
import android.opengl.GLSurfaceView
import android.os.Bundle


class GeodeticBodiesActivity : Activity() {
    private var mGLSurfaceView: GLSurfaceView? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mGLSurfaceView = GLSurfaceView(this)

        // Check if the system supports OpenGL ES 2.0.
        val activityManager = getSystemService(ACTIVITY_SERVICE) as ActivityManager
        val configurationInfo = activityManager.deviceConfigurationInfo
        val supportsEs3 = configurationInfo.reqGlEsVersion >= 0x30000
        if (supportsEs3) {
            // Request an OpenGL ES 3.0 compatible context.
            mGLSurfaceView!!.setEGLContextClientVersion(3)

            // Set the native renderer to our demo renderer,defined below.
            mGLSurfaceView!!.setRenderer(GeodeticBodiesRenderer(this))
        } else {
            // This is where you could create an OpenGL ES 1.x compatible
            // renderer if you wanted to support both ES 1 and ES 2.
            return
        }
        setContentView(mGLSurfaceView)
    }

    override fun onResume() {
        // The activity must call the GL surface view's onResume() on activity onResume().
        super.onResume()
        mGLSurfaceView!!.onResume()
    }

    override fun onPause() {
        // The activity must call the GL surface view's onPause() on activity onPause().
        super.onPause()
        mGLSurfaceView!!.onPause()
    }
}