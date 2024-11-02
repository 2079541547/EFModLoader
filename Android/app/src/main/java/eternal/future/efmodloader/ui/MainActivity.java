package eternal.future.efmodloader.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;

import java.io.File;
import java.util.Arrays;

import eternal.future.efmodloader.load.Loader;
import eternal.future.efmodloader.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {


    private ActivityMainBinding binding;

    @SuppressLint("SdCardPath")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        new File(Arrays.toString(this.getExternalFilesDirs(null))).mkdirs();

        Loader.load();


        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

    }

}