package eternal.future.efmodloader;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import eternal.future.efmodloader.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'efmodloader' library on application startup.
    static {
        System.loadLibrary("efmodloader");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

    }

}