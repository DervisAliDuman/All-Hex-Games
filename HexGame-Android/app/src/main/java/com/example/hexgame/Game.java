package com.example.hexgame;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;


public class Game extends AppCompatActivity {
    public int board_size = 6;
    public boolean player1_turn=true;
    public int flag1=0,flag2=0;
    public int game_type=1;
    public int lastx_coordinate=0,lasty_coordinate=0;
    public int [][]all_move_logs = new int[board_size*board_size][3];
    public int move_count=0;
    public int scoreX=0,scoreO=0;

    private Button buttons[][];
    private Integer control_table[][];

    RelativeLayout relativeLayout; //For 2d button array


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);

        Intent a = getIntent();
        board_size = a.getIntExtra("board_size",6);  //taking input from main activity
        game_type = a.getIntExtra("game_type",1);

        relativeLayout = findViewById(R.id.relative_main);
        relativeLayout.setBackgroundColor(Color.rgb(35, 10, 10));

        buttons = new Button[board_size][board_size];
        control_table = new Integer[board_size][board_size];

        for (int i = 0; i < board_size; i++) {  //making buttons and adding interface and layout
            for (int j = 0; j < board_size; j++) {
                control_table[i][j]=0;
                Button button = new Button(this);
                int width = Resources.getSystem().getDisplayMetrics().widthPixels-180 ;

                RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams((int)(width/(board_size*1.5)), (int)(width/(board_size*1.5)));

                button.setText("");
                button.setTextSize(9);
                int sum=(i)+board_size*j;
                button.setId(sum);
                button.setOnClickListener(Clicked(i,j,button));
                button.setTranslationX((i)*(int)(width/(board_size*1.5)+4)+j*((int)(width/(board_size*1.5)+4)/2));
                button.setTranslationY((j+1)*(int)(width/(board_size*1.5)+4));

                buttons[i][j]=button;

                buttons[i][j].setBackgroundColor(Color.rgb(150, 25, 25));

                relativeLayout.addView(button, layoutParams);
            }
        }

        Button b = (Button)findViewById(R.id.newactivity);
        b.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {   //reset button, starts main activity
                Intent a = new Intent(Game.this, MainActivity.class);
                //finish();
                startActivity(a);
            }
        });

        Button b2 = (Button)findViewById(R.id.qckreset);
        b2.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {   //resets board quickly
                Intent a = new Intent(Game.this,Game.class);
                a.putExtra("board_size",board_size);
                a.putExtra("game_type",game_type);

                startActivityForResult(a,1);
                finish();

            }
        });

        Button b3 = (Button)findViewById(R.id.undobutton);
        b3.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {  //BUTTON UNDO, undos mooves
                if(move_count>0){
                    buttons[all_move_logs[move_count-1][0]][all_move_logs[move_count-1][1]].setBackgroundColor(Color.rgb(150,25,25));
                    buttons[all_move_logs[move_count-1][0]][all_move_logs[move_count-1][1]].setText("");
                    move_count-=1;
                }
                if(game_type==2){
                    buttons[all_move_logs[move_count-1][0]][all_move_logs[move_count-1][1]].setBackgroundColor(Color.rgb(150,25,25));
                    buttons[all_move_logs[move_count-1][0]][all_move_logs[move_count-1][1]].setText("");
                    move_count-=1;
                }

                fixTurns();
            }
        });

        Button b4 = (Button)findViewById(R.id.savebutton);
        b4.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {   //SAVES THE GAME
                TextView informationtext= (TextView)findViewById(R.id.informationtext);
                String FILE_NAME = "save.txt";
                String text = "";
                FileOutputStream foutput = null;

                text+=""+board_size;    //Saving size, game type, move count
                text+="\n";
                text+=""+game_type;
                text+="\n";
                text+=""+move_count;
                text+="\n";

                for (int i = 0; i < move_count; i++){ //writing moves
                    text+=""+all_move_logs[i][0];
                    text+="\n";
                    text+=""+all_move_logs[i][1];
                    text+="\n";
                    text+=""+all_move_logs[i][2];
                    text+="\n";
                }

                try {
                    foutput = openFileOutput(FILE_NAME, MODE_PRIVATE);
                    foutput.write(text.getBytes());
                    informationtext.setText("SAVED");
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    if (foutput != null) {
                        try {
                            foutput.close();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }

            }
        });

        Button b5 = (Button)findViewById(R.id.real_load_button);
        b5.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {           //LOADİNG BOARD
                TextView informationtext= (TextView)findViewById(R.id.informationtext);
                FileInputStream finput = null;
                try {
                    finput = openFileInput("save.txt");
                    InputStreamReader isr = new InputStreamReader(finput);
                    BufferedReader br = new BufferedReader(isr);

                    String line;

                    int x;

                    if ((line = br.readLine()) != null){
                        x= Integer.parseInt(line);
                        reset_game(x);	//reset with given size
                    }

                    if ((line = br.readLine()) != null){    //taking gametype
                        x= Integer.parseInt(line);
                        game_type=x;
                    }

                    if ((line = br.readLine()) != null){  //taking move count
                        x= Integer.parseInt(line);
                        move_count=x;
                    }


                    for(int i=0 ; (line = br.readLine()) != null ; i++){ //taking board
                        x= Integer.parseInt(line);
                        all_move_logs[i][0]=x;
                        line = br.readLine();
                        x= Integer.parseInt(line);
                        all_move_logs[i][1]=x;
                        line = br.readLine();
                        x= Integer.parseInt(line);
                        all_move_logs[i][2]=x;
                    }

                    for(int i=0; i<move_count;i++){  //placing board

                        if(all_move_logs[i][2] == 0){
                            buttons[all_move_logs[i][0]][all_move_logs[i][1]].setText("X");
                            buttons[all_move_logs[i][0]][all_move_logs[i][1]].setBackgroundColor(Color.rgb(25,25,255));
                        }
                        else{
                            buttons[all_move_logs[i][0]][all_move_logs[i][1]].setText("O");
                            buttons[all_move_logs[i][0]][all_move_logs[i][1]].setBackgroundColor(Color.rgb(25,255,25));
                        }


                    }

                    fixTurns();

                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    if (finput!= null) {
                        try {
                            finput.close();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }

            }
        });

    }

    private View.OnClickListener Clicked(final int i,final int j,Button b){
        return new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                TextView informationtext= (TextView)findViewById(R.id.informationtext);
                TextView scoretextX= (TextView)findViewById(R.id.scoretextX);
                TextView scoretextO= (TextView)findViewById(R.id.scoretextO);
                if (player1_turn) {				//player 1 plays
                    if(buttons[i][j].getText()=="") {
                        buttons[i][j].setBackgroundColor(Color.rgb(25, 25, 255));
                        buttons[i][j].setText("X");

                        lastx_coordinate=j+1;
                        lasty_coordinate=i+1;

                        all_move_logs[move_count][0]=i; //move logs saving
                        all_move_logs[move_count][1]=j;
                        all_move_logs[move_count][2]=0;
                        move_count+=1;

                        reset_control_table();
                        scoreX=0;
                        is_game_done_for_user1(lastx_coordinate,lasty_coordinate); //wincheck

                        scoretextX.setText("Score X: "+scoreX);

                        player1_turn=false;

                        if(flag1==1 && flag2 == 1){ //wincheck with flags
                            informationtext.setText("WINNER X");
                            Button b3 = (Button)findViewById(R.id.undobutton);
                            b3.setEnabled(false);

                            DetermineWinnerWay();
                        }
                        else if(game_type==1){

                            informationtext.setText("Player2's Turn");
                        }
                        else{											//Computer plays
                            informationtext.setText("Computer's Turn");


                            playAI();       //plays AI

                            /*buttons[lasty_coordinate-1][lastx_coordinate-1].setBackgroundColor(Color.rgb(25, 255, 25));
                            buttons[lasty_coordinate-1][lastx_coordinate-1].setText("O");*/
                            reset_control_table();
                            scoreO=0;
                            is_game_done_for_user2(lastx_coordinate,lasty_coordinate); //wincheck
                            scoretextO.setText("Score O: "+scoreO);

                            all_move_logs[move_count][0]=lasty_coordinate-1; //move logs saving
                            all_move_logs[move_count][1]=lastx_coordinate-1;
                            all_move_logs[move_count][2]=1;
                            move_count+=1;

                            if(flag1==1 && flag2 == 1){ //wincheck with functions
                                informationtext.setText("WINNER O");
                                Button b3 = (Button)findViewById(R.id.undobutton);
                                b3.setEnabled(false);
                                DetermineWinnerWay();

                            }
                            else{
                                informationtext.setText("Player1's Turn");
                            }

                            player1_turn = true;

                        }

                    }
                }
                else if(game_type==1){			//player 2 plays
                    if(buttons[i][j].getText()=="") {
                        buttons[i][j].setBackgroundColor(Color.rgb(25, 255, 25));
                        buttons[i][j].setText("O");

                        all_move_logs[move_count][0]=i; //move logs saving
                        all_move_logs[move_count][1]=j;
                        all_move_logs[move_count][2]=1;
                        move_count+=1;

                        lastx_coordinate=j+1;   //last coodrinate saving
                        lasty_coordinate=i+1;

                        reset_control_table();
                        scoreO=0;
                        is_game_done_for_user2(lastx_coordinate,lasty_coordinate); //wincheck
                       scoretextO.setText("Score O: "+scoreO);
                        if(flag1==1 && flag2 == 1){     //wincheck with flags
                            informationtext.setText("WINNER O");
                            Button b3 = (Button)findViewById(R.id.undobutton);
                            b3.setEnabled(false);
                            DetermineWinnerWay();

                        }
                        else{
                            informationtext.setText("Player1's Turn");
                        }

                        player1_turn = true;
                    }
                }
            }
        };
    }

    public void reset_game(int size){ //reset buttons and size of everything

        scoreO=0;
        scoreX=0;

        move_count=0;

        for(int i = 0 ; i < board_size ; i++){ //deleting buttons from frame
            for(int j = 0; j < board_size ; j++){
                relativeLayout.removeView(buttons[i][j]);
            }
        }
        board_size=size;
        all_move_logs = new int[board_size*board_size][3];

        reset_control_table();


        /*                  RE-ADDİNG BUTTONS                           */

        buttons = new Button[board_size][board_size];
        control_table = new Integer[board_size][board_size];


        for (int i = 0; i < board_size; i++) {  //recreating buttons
            for (int j = 0; j < board_size; j++) {
                control_table[i][j]=0;
                Button button = new Button(this);
                int width = Resources.getSystem().getDisplayMetrics().widthPixels-180 ;

                RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams((int)(width/(board_size*1.5)), (int)(width/(board_size*1.5)));

                button.setText("");
                button.setTextSize(9);
                int sum=(i)+board_size*j;
                button.setId(sum);
                button.setOnClickListener(Clicked(i,j,button));
                button.setTranslationX((i)*(int)(width/(board_size*1.5)+4)+j*((int)(width/(board_size*1.5)+4)/2));
                button.setTranslationY((j+1)*(int)(width/(board_size*1.5)+4));

                buttons[i][j]=button;

                buttons[i][j].setBackgroundColor(Color.rgb(150, 25, 25));

                relativeLayout.addView(button, layoutParams);
            }
        }

        player1_turn=true;

    }

    public void fixTurns(){		//fixs turns
        TextView informationtext= (TextView)findViewById(R.id.informationtext);
        if(move_count%2==0){
            player1_turn = true;
            informationtext.setText("Player1's Turn");
        }
        else{
            player1_turn = false;
            informationtext.setText("Player2's Turn");
        }
    }

    public void DetermineWinnerWay(){  //paints winner way and disables buttons

        for(int i = 0 ; i < board_size ; i++){
            for(int j = 0; j < board_size ; j++){
                if(control_table[i][j]==1){
                    buttons[i][j].setBackgroundColor(Color.rgb(255,0,0)); //paints the winner way
                }
                buttons[i][j].setEnabled(false); //disable buttons
            }
        }

    }

    //checks for last move of user1 for if game end or not
    public void is_game_done_for_user1(int coordinate_x,int coordinate_y){
        scoreX+=1;
        control_table[coordinate_y-1][coordinate_x-1]=1;


        for(int i=1;i<=board_size;i++){	//if we go most right or most left
            if(coordinate_y == 1 && coordinate_x==i)
                flag1=1;
            if(coordinate_y == board_size && coordinate_x==i)
                flag2=1;
        }
        //Moving around recursively
        if(coordinate_y-2>=0 && control_table[coordinate_y - 2][coordinate_x-1] == 0 && buttons[coordinate_y-2][coordinate_x-1].getText()=="X")
            is_game_done_for_user1(coordinate_x,coordinate_y-1);
        if(coordinate_y-2>=0 && coordinate_x <board_size && control_table[coordinate_y-2][coordinate_x] == 0 && buttons[coordinate_y-2][coordinate_x].getText()=="X")
            is_game_done_for_user1(coordinate_x+1,coordinate_y-1);
        if(coordinate_x < board_size && control_table[coordinate_y-1][coordinate_x] == 0 && buttons[coordinate_y-1][coordinate_x].getText()=="X")
            is_game_done_for_user1(coordinate_x+1,coordinate_y);
        if(coordinate_y < board_size && control_table[coordinate_y][coordinate_x-1] == 0 && buttons[coordinate_y][coordinate_x-1].getText()=="X")
            is_game_done_for_user1(coordinate_x,coordinate_y+1);
        if(coordinate_y < board_size && coordinate_x- 2 >= 0 && control_table[coordinate_y][coordinate_x-2] == 0 && buttons[coordinate_y][coordinate_x-2].getText()=="X")
            is_game_done_for_user1(coordinate_x-1,coordinate_y+1);
        if(coordinate_x-2 >= 0 && control_table[coordinate_y - 1][coordinate_x -2] == 0 && buttons[coordinate_y-1][coordinate_x-2].getText()=="X")
            is_game_done_for_user1(coordinate_x-1,coordinate_y);

    }

    //checks for last move of user2 for if game end or not
    public void is_game_done_for_user2(int coordinate_x,int coordinate_y){
        scoreO+=1;
        control_table[coordinate_y-1][coordinate_x-1]=1;
        for(int i=1;i<=board_size;i++){	//if we go top or lowest place
            if(coordinate_y == i && coordinate_x==1)
                flag1=1;
            if(coordinate_y == i && coordinate_x==board_size)
                flag2=1;
        }

        //Moving around recursively
        if(coordinate_y-2>=0 && control_table[coordinate_y - 2][coordinate_x-1] == 0 && buttons[coordinate_y-2][coordinate_x-1].getText()=="O")
            is_game_done_for_user2(coordinate_x,coordinate_y-1);
        if(coordinate_y-2>=0 && coordinate_x <board_size && control_table[coordinate_y-2][coordinate_x] == 0 && buttons[coordinate_y-2][coordinate_x].getText()=="O")
            is_game_done_for_user2(coordinate_x+1,coordinate_y-1);
        if(coordinate_x < board_size && control_table[coordinate_y-1][coordinate_x] == 0 && buttons[coordinate_y-1][coordinate_x].getText()=="O")
            is_game_done_for_user2(coordinate_x+1,coordinate_y);
        if(coordinate_y < board_size && control_table[coordinate_y][coordinate_x-1] == 0 && buttons[coordinate_y][coordinate_x-1].getText()=="O")
            is_game_done_for_user2(coordinate_x,coordinate_y+1);
        if(coordinate_y < board_size && coordinate_x- 2 >= 0 && control_table[coordinate_y][coordinate_x-2] == 0 && buttons[coordinate_y][coordinate_x-2].getText()=="O")
            is_game_done_for_user2(coordinate_x-1,coordinate_y+1);
        if(coordinate_x-2 >= 0 && control_table[coordinate_y - 1][coordinate_x -2] == 0 && buttons[coordinate_y-1][coordinate_x-2].getText()=="O")
            is_game_done_for_user2(coordinate_x-1,coordinate_y);

    }

    public void reset_control_table(){	//resets my control table
        flag1=0;
        flag2=0;

        control_table = new Integer[board_size][board_size];

        for (int i = 0; i < board_size; i++) {
            for (int j = 0; j < board_size; j++) {
                control_table[i][j] = 0;
            }
        }
    }

   /* public int wincheck (){               //It is my wincheck fuction in comment for just in case
        if(player1_turn){
            reset_control_table();
            scoreX=0;
            is_game_done_for_user1(lastx_coordinate,lasty_coordinate);
            if(flag1==1 && flag2 == 1){
                return 10;
            }

        }
        else {
            reset_control_table();
            scoreO=0;
            is_game_done_for_user2(lastx_coordinate,lasty_coordinate);
            if(flag1==1 && flag2 == 1){
                return -10;
            }
        }
        return 0;
    }*/




    ////////////////ATENTİON\\\\\\\\\\\\\\\\\\\\\
    //All theese Codes below this line converted from internet. (This is homewrk and in the pdf it says we can use minimax algorithm from internet)


    public char [][]brd;
    public int [][]trW;
    public int [][]trB;
    public int size=board_size;
    public int level = 1;
    public char [][] hexCells;

    private int  MaximizingPlayer = 1;
    private int  MinimizingPlayer = -1;

    public void copyboardtohexCells(){
        hexCells = new char[board_size][board_size];
        brd = new char[board_size][board_size];
        trB = new int[board_size][board_size];
        trW = new int[board_size][board_size];
        for (int i =0 ; i< board_size;i++){
            for(int j =0 ; j< board_size;j++){
                if(buttons[i][j].getText()==""){
                    hexCells[i][j] = '.';
                }else if(buttons[i][j].getText()=="O"){
                    hexCells[i][j] = 'o';
                }else if(buttons[i][j].getText()=="X"){
                    hexCells[i][j] = 'x';
                }
            }
        }
    }

    private void playAI(){
        size = board_size;
        copyboardtohexCells();
        int [] move = BestMove( MinimizingPlayer);

        lastx_coordinate=move[1]+1;
        lasty_coordinate=move[0]+1;

        buttons[move[0]][move[1]].setBackgroundColor(Color.rgb(25, 255, 25));
        buttons[move[0]][move[1]].setText("O");
        /*
            LAST MOVE EKLE
         */
    }
    private int [] BestMove(int player){
        int minval = Integer.MAX_VALUE; // INF
        int maxval = Integer.MIN_VALUE; // -INF

        int[] bestMove = new int[2];
        int i, j;
        int moveVal;
        bestMove[0] = -1;
        bestMove[1] = -1;
        if (player == MaximizingPlayer) {
            for (i = 0; i < size; i++)
                for (j = 0; j < size; j++) {
                    if (hexCells[i][j] == '.') {
                        hexCells[i][j] = 'x'; // it was 'w'
                        moveVal = minimax(level, false);
                        hexCells[i][j] = '.';
                        if (moveVal > maxval) {
                            bestMove[0] = i;
                            bestMove[1] = j;
                            maxval = moveVal;
                        }
                    }
                }
        }
        else {                             /* player == MinimizingPlayer */
            for (i = 0; i < size; i++)
                for (j = 0; j < size; j++) {
                    if ( hexCells[i][j] == '.') {
                        hexCells[i][j] = 'o'; // it was 'b'
                        moveVal = minimax(level, true);
                        hexCells[i][j] = '.';
                        if (moveVal < minval) {

                            bestMove[0] = i;
                            bestMove[1] = j;
                            minval = moveVal;
                        }
                    }
                }
        }

        return bestMove;
    }
    int minimax(int depth, boolean maxTurn) {
        if (depth == 0 || winnerFound()!=0)
            return staticEvaluation((maxTurn) ? 1: 0);

        if (BoardNotFull() == 0)
            return 0;

        if (maxTurn) {
            int maxEval = -Integer.MAX_VALUE;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++)
                    if (hexCells[i][j] == '.') {
                        hexCells[i][j] = 'x';
                        maxEval = maximum(maxEval, minimax(depth - 1, !(maxTurn)));
                        hexCells[i][j] = '.';
                    }
            }
            return maxEval;
        } else {/* minimizing player */
            int minEval = Integer.MAX_VALUE;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++)
                    if (hexCells[i][j] == '.') {
                        hexCells[i][j] = 'o';
                        minEval = minimum(minEval, minimax(depth - 1, !(maxTurn)));
                        hexCells[i][j] = '.';
                    }
            }
            return minEval;
        }
    }
    private int winnerFound(){
        int[] s = new int[1];
        transformBlackMatrix();                    /* update trB matrix */
        transformWhiteMatrix();                    /* update trW matrix */
        if (WhiteWon(size,trW,s)!=0)
            return MaximizingPlayer;

        if (BlackWon(size, trB, s)!=0)
            return MinimizingPlayer;

        return 0;
    }
    private void transformBlackMatrix(){ /* for Black */
        /* returns a matrix of integers where (1) is the source node */
        /* and the rest of the elements with 'b' are initialized with 3 */
        /* if the case is none of the above then A[i][j] = 0 */
        /* free the memory when check for winner is completed */
        int i, j;

        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++) {
                if (j == 0 && hexCells[i][j] == 'o')
                    trB[i][j] = 1;    /* source nodes are j = 0 with 'b' */
                else if (hexCells[i][j] == 'o')
                    trB[i][j] = 3;
                else
                    trB[i][j] = 0;
            }

    }
    void transformWhiteMatrix() /* for  White */
        /* input array B is the array of chars with 'b', 'w' and '.' */
        /* returns a matrix of integers where (1) is the source node */
        /* and the rest of the elements with 'w' are initialized with 3 */
        /* if the case is none of the above then A[i][j] = 0 */
        /* free the memory when check for winner is completed */
    { int i, j;

        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++) {
                if (i == 0 && hexCells[i][j] == 'x')
                    trW[i][j] = 1;    /* source nodes are i = 0 with 'w' */
                else if (hexCells[i][j] == 'x')
                    trW[i][j] = 3;
                else
                    trW[i][j] = 0;
            }

    }
    int WhiteWon(int n, int[][] matrix, int[] seq)
        /* returns 1 if path was found else reutns 0 */
        /* seq is the max sequence of white pawns */
    {
        int i, j;
        int flag = 0;
        int[][] visited= new int [n][n];          /* keep track of already visited indexes */


        for (i = 0; i < n; i++) /* initialize visited (array) with false */
            for (j = 0; j < n; j++)      /* since we don't know the paths */
                visited[i][j] = 0;

        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) {
                if (matrix[i][j] == 1 && visited[i][j] == 0) /* if source cell */
                    if (PathSearchWhite(i, j, n, matrix, visited) == 1) {
                        /* search */
                        flag = 1;                          /* path was found */
                    }

            }

        int diff = maxi(visited, n) - mini(visited, n);
        if (diff <= 0)
            seq[0] = 0;
        else
            seq[0] = diff + 1;


        visited = null;

        if (flag == 1)
            return 1;
        else
            return 0;  /* path was not found */

    }
    int PathSearchWhite(int i, int j, int n, int[][] matrix, int[][] visited)
    {
        if (valid(i, j, n)==1 && matrix[i][j] != 0 && visited[i][j] == 0) {
            /* terminal cases */
            visited[i][j] = 1;                            /* cell visited */
            if (matrix[i][j] == 3 && i == n-1)          /* if cell is the */
                return 1;        /* destination(the other end) return true */

            // traverse north
            /* if path is found in this direction */
            if (PathSearchWhite(i-1, j, n, matrix, visited) == 1)
                return 1;                                   /* return true */

            // traverse west
            /* if path is found in this direction */
            if (PathSearchWhite(i, j-1, n, matrix, visited)== 1)
                return 1;                                   /* return true */

            // traverse south
            /* if path is found in this direction */
            if (PathSearchWhite(i+1, j, n, matrix, visited)== 1)
                return 1;                                  /* return true */

            // traverse east
            /* if path is found in this direction */
            if (PathSearchWhite(i, j+1, n, matrix, visited)== 1)
                return 1;                                   /* return true */

            // traverse northeast
            /* if path is found in this direction */
            if (PathSearchWhite(i-1, j+1, n, matrix, visited)== 1)
                return 1;                                   /* return true */

            // traverse southwest
            /* if path is found in this direction */
            if (PathSearchWhite(i+1, j-1, n, matrix, visited)== 1)
                return 1;                                  /* return true */


        }

        return 0;                                   /* no path was found */
    }
    int valid(int i, int j, int n)
    { if ((i >= 0) && (i < n) &&
            (j >= 0) && (j < n) )
        return 1;
    else
        return 0;
    }
    int maxi(int[][] visited, int n)
    { int i, j, max = -1;
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (visited[i][j] == 1)
                    max = i;

        return max;
    }
    int mini(int[][] visited, int n)
    { int i, j, min;
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (visited[i][j] == 1)
                    return min = i;

        return -1;

    }
    int BlackWon(int n, int[][] matrix, int[] seq)
        /* returns 1 if path was found else reutns 0 */
    { int i, j;
        int[][] visited;          /* keep track of already visited indexes */
        int flag = 0;
        visited = new int[n][n];

        for (i = 0; i < n; i++) /* initialize visited (array) with false */
            for (j = 0; j < n; j++)      /* since we don't know the paths */
                visited[i][j] = 0;

        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) {
                if (matrix[i][j] == 1 && visited[i][j] == 0) /* if source cell */
                    if (PathSearchBlack(i, j, n, matrix, visited) == 1 ) {
                        flag = 1; /* path was found */
                    }

            }

        int diff = maxj(visited, n) - minj(visited, n);
        if (diff <= 0)
            seq[0] = 0;
        else
            seq[0] = diff + 1;


        visited = null;
        if (flag == 1)
            return 1;   /* path was found */
        else
            return 0;   /* path was not found */

    }
    int PathSearchBlack(int i, int j, int n, int[][] matrix, int[][] visited)
        /* searches for path from right to left */
    {
        if (valid(i, j, n)!=0 && matrix[i][j] != 0 && visited[i][j] == 0) {
            /* terminal cases */
            visited[i][j] = 1;                            /* cell visited */
            if (matrix[i][j] == 3 && j == n-1)          /* if cell is the */
                return 1;        /* destination(the other end) return true */

            // traverse north
            /* if path is found in this direction */
            if (PathSearchBlack(i-1, j, n, matrix, visited)==1)
                return 1;                                   /* return true */

            // traverse west
            /* if path is found in this direction */
            if (PathSearchBlack(i, j-1, n, matrix, visited)==1)
                return 1;                                   /* return true */

            // traverse south
            /* if path is found in this direction */
            if (PathSearchBlack(i+1, j, n, matrix, visited)==1)
                return 1;                                   /* return true */

            // traverse east
            /* if path is found in this direction */
            if (PathSearchBlack(i, j+1, n, matrix, visited)==1)
                return 1;                                   /* return true */

            // traverse northeast
            /* if path is found in this direction */
            if (PathSearchBlack(i-1, j+1, n, matrix, visited)==1)
                return 1;                                   /* return true */

            // traverse southwest
            /* if path is found in this direction */
            if (PathSearchBlack(i+1, j-1, n, matrix, visited)==1)
                return 1;                                   /* return true */


        }

        return 0;                                   /* no path was found */
    }
    int maxj(int[][] visited, int n)
        /* returns the greater j co-ordinate where last 1 was found */
        /* in "visited"  array */
    { int i, j, max = -1;
        for (j = 0; j < n; j++)
            for (i = 0; i < n; i++)
                if (visited[i][j] == 1)
                    max = j;

        return max;
    }
    int minj(int[][] visited, int n)
    { int i, j, min;
        for (j = 0; j < n; j++)
            for (i = 0; i < n; i++)
                if (visited[i][j] == 1)
                    return min = j;

        return -1;


    }
    int staticEvaluation( int maxTurn)
        /* evaluates a board-state */
    { int winner = 0;
        int seqW = 0;
        int seqB = 0;
        int[] s = new int[1];
        int k;
        transformWhiteMatrix();
        transformBlackMatrix();

        for (k = 0; k < size; k++) {
            changeW(size, trW, k);
            if (WhiteWon(size, trW, s) != 0) {
                /* if the two sides are connected */
                if (k == 0) {
                    winner = MaximizingPlayer;
                    break; }

            }
            else                      /* if White in not the winner, then */
                seqW = maximum(seqW, s[0]);       /* save the maximum sequence */
        }




        for (k = 0; k < size; k++) {
            changeB(size, trB, k);
            if (BlackWon(size, trB, s) != 0) {
                /* if the two sides are connected */
                if (k == 0) {
                    winner = MinimizingPlayer;
                    break; }

            }
            else                      /* if Black in not the winner, then */
                seqB = maximum(seqB, s[0]);       /* save the maximum sequence */
        }



        int scoreW = seqW* size / 2;
        int scoreB = seqB* size / 2;



        if (winner == MaximizingPlayer)
            return 100;


        if (winner == MinimizingPlayer)
            return -100;

        if (maxTurn != 0)
            return scoreW - scoreB  + CenterVal( maxTurn) + DirVal( maxTurn)/2 +2* BridgeVal( maxTurn);
        else return scoreW - scoreB  - CenterVal(maxTurn) - DirVal( maxTurn)/2 - 2*BridgeVal( maxTurn);


    }
    int CenterVal( int maxTurn)
    { int i, j, counter = 0;
        int val = 0;
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                if (hexCells[i][j] != '.')         /* number of filled cells */
                    counter++;

        if (counter < size/2 ) {
            for (i = size/2 - 1; i < size/2; i++)
                for (j = size/2 - 1; j < size/2; j++)
                    if (hexCells[i][j] == 'o' && maxTurn==0)
                        val++;
                    else if (hexCells[i][j] == 'x' && maxTurn!=0)
                        val++;

            return val;
        }
        else
            return 0;


    }

    void changeW(int n, int[][] matr, int k)                    /* White */
        /* updates transformed matrix with new sources for different k, */
        /* where k is a source row */
    { int i, j;
        if (k == 0)      /* when k = 0 we dont need to change the matrix */
            return;

        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) {
                if (i == k && matr[i][j] == 3) /* if was (non-source) cell */
                    matr[i][j] = 1;                           /* new source */
                else if (matr[i][j] == 3)
                    continue;
                else {                            /* old sources must be 0 */
                    matr[i][j] = 0; }
            }
    }
    int maximum(int a, int b)
    { return (a >= b) ? a : b;
    }
    void changeB(int n, int[][] matr, int k)                    /* Black */
        /* updates transformed matrix with new sources for different k, */
        /* where k is a source column */
    { int i, j;

        if (k == 0)     /* when k = 0, we dont need to change the matrix */
            return;

        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) {
                if (j == k && matr[i][j] == 3) /* if was (non-source) cell */
                    matr[i][j] = 1;                           /* new source */
                else if (matr[i][j] == 3)
                    continue;
                else {                            /* old sources must be 0 */
                    matr[i][j] = 0;  }
            }
    }
    int DirVal( int maxTurn)
    { int i, j, val = 0;
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                if (maxTurn!=0 && hexCells[i][j] == 'x') {
                    if (valid(i+1, j, size)==1 && valid(i-1, j, size)==1)
                        if (hexCells[i+1][j] == 'x' || hexCells[i-1][j] == 'x')
                            val++;
                }
                else if (maxTurn==0 && hexCells[i][j] == 'o') {
                    if (valid(i, j+1, size)!=0 && valid(i, j-1, size)!=0)
                        if (hexCells[i][j+1] == 'o' || hexCells[i][j-1] == 'o')
                            val++;
                }
            }
        }
        return val;
    }

    int BridgeVal( int maxTurn)
    { int i, j, bridge = 0;
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++) {
                if (maxTurn==0 && hexCells[i][j] == 'o' && surrounded_ratio_b(i, j) >= 1) {
                    if (valid(i-1, j+2, size)==1 && hexCells[i-1][j+2] == 'o') bridge++;
                    if (valid(i+1, j-2, size)==1 && hexCells[i+1][j-2] == 'o') bridge++;
                    if (valid(i-1, j-1, size)==1 && hexCells[i-1][j-1] == 'o') bridge++;
                    if (valid(i+2, j-1, size)==1 && hexCells[i+2][j-1] == 'o') bridge++;
                    if (valid(i+1, j+1, size)==1 && hexCells[i+1][j+1] == 'o') bridge++;
                }
                else if (hexCells[i][j] == 'x' && surrounded_ratio_w(i, j) >= 2) {
                    if (valid(i-1, j+2, size)==1 && hexCells[i-1][j+2] == 'x') bridge++;
                    if (valid(i+1, j-2, size)==1 && hexCells[i+1][j-2] == 'x') bridge++;
                    if (valid(i-1, j-1, size)==1 && hexCells[i-1][j-1] == 'x') bridge++;
                    if (valid(i+2, j-1, size)==1 && hexCells[i+2][j-1] == 'x') bridge++;
                    if (valid(i+1, j+1, size)==1 && hexCells[i+1][j+1] == 'x') bridge++;
                }

            }

        return bridge;


    }
    int surrounded_ratio_b(int i, int j)
    { int r = 0;
        if (valid(i, j+1,size)==1 && hexCells[i][j+1] == 'x') r++;
        if (valid(i, j-1,size)==1 && hexCells[i][j-1] == 'x') r++;
        if (valid(i-1, j+1,size) == 1 && hexCells[i-1][j+1] == 'x') r++;
        return r;
    }
    int surrounded_ratio_w(int i, int j)
    { int r = 0;
        if (valid(i-1, j,size)==1 && hexCells[i-1][j] == 'o') r++;
        if (valid(i+1, j,size)==1 && hexCells[i+1][j] == 'o') r++;
        if (valid(i+1, j-1,size)==1 && hexCells[i+1][j-1] == 'o') r++;
        return r;
    }
    int BoardNotFull()
    { for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (hexCells[i][j] == '.')
                return 1; /* there is at least one cell available */

        return 0; /* board is full */
    }
    int minimum(int a, int b)
    { return (a <= b) ? a : b;
    }

}
