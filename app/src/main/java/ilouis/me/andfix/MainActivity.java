package ilouis.me.andfix;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import java.io.File;

public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }

    public void jisuan(View view) {
        Caculator caculator = new Caculator();
        caculator.cal();
        Toast.makeText(this, "success", Toast.LENGTH_SHORT).show();
    }


    public void fix(View view) {
        try {
            Log.d("MainActivity", "Environment.getExternalStorageDirectory():" + Environment.getExternalStorageDirectory());
            DexManager.getInstance(this).loadFile(new File(Environment.getExternalStorageDirectory(), "fix.dex"));
        } catch (Error e){
            e.printStackTrace();
            Toast.makeText(this, "请将fxi.dex放入你的getExternalStorageDirectory", Toast.LENGTH_SHORT).show();
        }
    }
}
