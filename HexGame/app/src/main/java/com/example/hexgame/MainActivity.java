package com.example.hexgame;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;

public class MainActivity extends AppCompatActivity {
    public int board_size = 6;
    public int game_type = 1;
    public int difficulty = 2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button b = (Button)findViewById(R.id.ApplyButton);
        b.setOnClickListener(new View.OnClickListener(){    //opening gameplay with selected radio buttons
            @Override
            public void onClick(View v) {

                Intent a = new Intent(MainActivity.this, Game.class);
                a.putExtra("board_size",board_size);
                a.putExtra("game_type",game_type);
                finish();
                startActivityForResult(a,1);   //opening activity Game
            }
        });
    }

    public void sizeradiobuttonclicked(View v) {    //selecting size

        boolean checked = ((RadioButton) v).isChecked();

        switch(v.getId()) {
            case R.id.size6:
                if (checked)
                    board_size = 6;
                break;
            case R.id.size7:
                if (checked)
                    board_size = 7;
                break;
            case R.id.size8:
                if (checked)
                    board_size = 8;
                break;
            case R.id.size9:
                if (checked)
                    board_size=9;
                break;
        }
    }

    public void gametyperadiobutton(View v) {       //selecting pvp or pvc

        boolean checked = ((RadioButton) v).isChecked();

        switch(v.getId()) {
            case R.id.pvp:
                if (checked)
                    game_type=1;
                break;
            case R.id.pvcom:
                if (checked)
                    game_type=2;
                break;
        }
    }



}