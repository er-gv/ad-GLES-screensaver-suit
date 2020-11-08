package com.ergv.glScreenSavers

import android.app.Activity
import android.app.ListActivity
import android.content.Intent
import android.os.Bundle
import android.util.SparseArray
import android.widget.AdapterView.OnItemClickListener
import android.widget.SimpleAdapter
import com.ergv.glScreenSavers.MandelbrotSet.MandelbrotSetActivity
import com.ergv.glScreenSavers.Polyhedrons.PolyhedronsActivity
import com.ergv.glScreenSavers.bricksShader.BricksShaderActivity
import com.ergv.glScreenSavers.geodeticBodies.GeodeticBodiesActivity
import java.util.*


class TableOfContents : ListActivity() {
    public override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setTitle(R.string.toc)
        setContentView(R.layout.table_of_contents)

        // Initialize data
        val data: MutableList<Map<String, Any?>> = ArrayList()
        val activityMapping = SparseArray<Class<out Activity?>>()
        var i = 0
        run {
            val item: MutableMap<String, Any?> = HashMap()
            item[ITEM_IMAGE] = R.drawable.bricks_icon
            item[ITEM_TITLE] = getText(R.string.geodetic_bodies_title)
            item[ITEM_SUBTITLE] = getText(R.string.geodetic_bodies_subtitle)
            data.add(item)
            activityMapping.put(i++, GeodeticBodiesActivity::class.java)
        }
        run {
            val item: MutableMap<String, Any?> = HashMap()
            item[ITEM_IMAGE] = R.drawable.bricks_icon
            item[ITEM_TITLE] = getText(R.string.spinning_polyhedrons)
            item[ITEM_SUBTITLE] = getText(R.string.spinning_polyhedrons_subtitle)
            data.add(item)
            activityMapping.put(i++, PolyhedronsActivity::class.java)
        }

        run {
            val item: MutableMap<String, Any?> = HashMap()
            item[ITEM_IMAGE] = R.drawable.bricks_icon
            item[ITEM_TITLE] = getText(R.string.mandelbrot_set)
            item[ITEM_SUBTITLE] = getText(R.string.mandelbrot_set_subtitle)
            data.add(item)
            activityMapping.put(i++, MandelbrotSetActivity::class.java)
        }
        run {
            val item: MutableMap<String, Any?> = HashMap()
            item[ITEM_IMAGE] = R.drawable.bricks_icon
            item[ITEM_TITLE] = getText(R.string.bricks_shader)
            item[ITEM_SUBTITLE] = getText(R.string.bricks_shader_subtitle)
            data.add(item)
            activityMapping.put(i++, BricksShaderActivity::class.java)
        }
        val dataAdapter = SimpleAdapter(this, data, R.layout.toc_item, arrayOf(ITEM_IMAGE, ITEM_TITLE, ITEM_SUBTITLE), intArrayOf(R.id.Image, R.id.Title, R.id.SubTitle))
        listAdapter = dataAdapter
        listView.onItemClickListener = OnItemClickListener { parent, view, position, id ->
            val activityToLaunch = activityMapping[position]
            if (activityToLaunch != null) {
                val launchIntent = Intent(this@TableOfContents, activityToLaunch)
                startActivity(launchIntent)
            }
        }
    }

    companion object {
        private const val ITEM_IMAGE = "item_image"
        private const val ITEM_TITLE = "item_title"
        private const val ITEM_SUBTITLE = "item_subtitle"
        private const val ITEM_ID = "item_id"
    }
}