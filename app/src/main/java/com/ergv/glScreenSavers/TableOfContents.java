package com.ergv.glScreenSavers;

import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.util.SparseArray;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.SimpleAdapter;

import com.ergv.glScreenSavers.JuliaSet.JuliaSetActivity;
import com.ergv.glScreenSavers.Polyhedrons.PolyhedronsActivity;
import com.ergv.glScreenSavers.bricksShader.BricksShaderActivity;
import com.ergv.glScreenSavers.quantizedColor.QuantizedColorActivity;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class TableOfContents extends ListActivity {
    private static final String ITEM_IMAGE = "item_image";
    private static final String ITEM_TITLE = "item_title";
    private static final String ITEM_SUBTITLE = "item_subtitle";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setTitle(R.string.toc);
        setContentView(R.layout.table_of_contents);

        // Initialize data
        final List<Map<String, Object>> data = new ArrayList<Map<String, Object>>();
        final SparseArray<Class<? extends Activity>> activityMapping = new SparseArray<Class<? extends Activity>>();

        int i = 0;
        {
            final Map<String, Object> item = new HashMap<String, Object>();
            item.put(ITEM_IMAGE, R.drawable.bricks_icon);
            item.put(ITEM_TITLE, getText(R.string.spinning_polyhedrons));
            item.put(ITEM_SUBTITLE, getText(R.string.spinning_polyhedrons_subtitle));
            data.add(item);
            activityMapping.put(i++, PolyhedronsActivity.class);
        }
        {
            final Map<String, Object> item = new HashMap<String, Object>();
            item.put(ITEM_IMAGE, R.drawable.ic_lesson_one);
            item.put(ITEM_TITLE, getText(R.string.quantized_color));
            item.put(ITEM_SUBTITLE, getText(R.string.quantized_color_subtitle));
            data.add(item);
            activityMapping.put(i++, QuantizedColorActivity.class);
        }
        {
            final Map<String, Object> item = new HashMap<String, Object>();
            item.put(ITEM_IMAGE, R.drawable.bricks_icon);
            item.put(ITEM_TITLE, getText(R.string.julia_set));
            item.put(ITEM_SUBTITLE, getText(R.string.julia_set_subtitle));
            data.add(item);
            activityMapping.put(i++, JuliaSetActivity.class);
        }
        {
            final Map<String, Object> item = new HashMap<String, Object>();
            item.put(ITEM_IMAGE, R.drawable.bricks_icon);
            item.put(ITEM_TITLE, getText(R.string.bricks_shader));
            item.put(ITEM_SUBTITLE, getText(R.string.bricks_shader_subtitle));
            data.add(item);
            activityMapping.put(i++, BricksShaderActivity.class);
        }




        final SimpleAdapter dataAdapter = new SimpleAdapter(this, data, R.layout.toc_item, new String[]{ITEM_IMAGE, ITEM_TITLE, ITEM_SUBTITLE}, new int[]{R.id.Image, R.id.Title, R.id.SubTitle});
        setListAdapter(dataAdapter);

        getListView().setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {
                final Class<? extends Activity> activityToLaunch = activityMapping.get(position);

                if (activityToLaunch != null) {
                    final Intent launchIntent = new Intent(TableOfContents.this, activityToLaunch);
                    startActivity(launchIntent);
                }
            }
        });
    }
}
