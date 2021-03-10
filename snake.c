/*
**  Title of program: snake.c. In order to run this program it must
**  be entered the terminal like so: gcc snake.c -lcurses, then you
**  must type in the output so the program can start like so: ./a
**  .out.
**  Authors of this program are Haval Mohammed & Oscar Dias
**  Date : 02DEC2019
*/

#include    <stdio.h>
#include    <curses.h>
#include     <stdlib.h>
#include    <time.h>
#include     <sys/ioctl.h>
#include    <unistd.h>
#include    <string.h>
#include    <sys/time.h>
#include    <signal.h>

/* We are intializing our prototypes */

/* Initializing the functions */
void drawTrophy(); /* This function creates random trophies, radnom location for the trophy, and random time before it expires. This is called when the trophy is eaten by the snake or the time expires */

void initialize(); /* initialize function builds/loads whats need for the game */

void drawGame(); /* drawGame() draws the map every call to maintain latest update with logic */

void myMove(); /* myMove() function gets the user inout control */

void PLAY(); /* PLAY() function updates the gamePLAY logic, checks WON/LOSS conditions */

bool continuePlay; /* As long as this variable is looped through, this boolean variable will continuing updateing the game */

bool WON; /* When the snake is half of the width of the width of the screen then WON == True...
    If we have won the game or  not*/

int WidthOfGame, HeightOfGame; /* The width & height of the map*/

int snakeHeadX, snakeHeadY; /* To keep position tracked of the snake in the X & Y coordinates */

int snakeSpeed = 5; /* Default speed of the snake */

/* This is coordinates for the trophies, the radnomly selected trophy, and the timer for the trophy */
int trophyX, trophyY, trophyValue, trophyTime, trophyTimeCount;

/*  The snakes body is held by the arrays snakeBodyX & snakeBodyY. This will be checked, in case the snake runs into itself.
*/
int snakeBodyX[50];
int snakeBodyY[50];
int snakeLength = 0; /* Default snakeLength of the snake */

int direction; /* Direction. KEY_UP = 0, KEY_DOWN = 1, KEY_LEFT = 2, KEY_RIGHT = 3 */
/*
**  As we go thru the presentation we'll explain who did what and
**  answer any questions. We'll try our best to explain the answers
**  to your questions.
*/
int main()
{
    struct winsize wsz;    /* wsz is the windows size buffer. This holds the rows, cols,  */
    ioctl(0, TIOCGWINSZ, &wsz); /* input output control, or ioctl is a device-specific system call */
    HeightOfGame = wsz.ws_row;
    WidthOfGame = wsz.ws_col;

    /* This is so that getch() from the move function does not catch up to the flow */
    WINDOW *w = initscr();
    /*  The nodelay option causes getch() to be a non-blocking call
    **  If no input is ready, getch returns ERR. If disabled (bf is
    **  FALSE), getch waits until a key is pressed.
    */
    nodelay(w, TRUE);
    clear();    /* clear screen    */

    initialize(); /* Loaded game configuration */

    while(continuePlay)
    {
        curs_set(0); /* Sets the cursor visiblity to invisible */
        keypad(stdscr, TRUE);
        /*
        **  This while loop will be continously running.The map is
        **  updated, it receives the input from the user and then
        **  updates the map/canvas. Then it
        **  checks all game conditions: if the snake runs into the
        **  border, or it runs into itself, or The user attempts to
        **  reverse the snake's direction.
        */
        drawGame();
        myMove();
        PLAY();
        usleep(1000000 / snakeSpeed); /*  */
    }
    endwin();   /* turn off curses    */
    if(WON)
    {
        printf("You have won the game! \n");
    }
    else
    {
        printf("Sorry you lose! \n");
    }
    return 0;
}
/*  This function is what loads the default configuration for the
**  game.
**  Written by Oscar Dias
*/
void initialize()
{
    WON = true;
    /* This is where if WON == true, then the following */
    
    /* This is for the while loop to be true then continue  playing the game */
    continuePlay = true;
    /* This is the default snakeLength of the snake */
    snakeLength = 0;
    direction = (rand() % 4); /* This where the snake randomly starts in a direction */
    snakeHeadX = (rand() % (WidthOfGame - 5)); /* This is where the snake head in the horizontal coordinate is radnomly starting position */
    snakeHeadY = (rand() % (HeightOfGame - 5)); /* This is where the snake head in the vertical coordinate is radnomly starting position */
    drawTrophy(); /* This is where the drawTrophy() function is called and the trophy is ranodmly placed. */
}
/* Writtten by Oscar Dias */
/*  This is the snakes body. On every turn, the head gets
**  updated to becomes the snakes body first part.
**  It goes down the snakes and replaces the char ahead with
**  the current one. Except this program adds that many 'o' to
**  the tail.
*/
void PLAY()
{
    /*  SnakeBody is going to be this array of 50 which is going to have the WHOLE snake in it
        We start snakeBody at [0] (obv) and we take the snakehead
    */
    int movesnakeHeadX = snakeBodyX[0]; /* initializes that movesnakeHeadX == snakeBodyX array */
    /* initializes that movesnakeHeadY == snakeBodyY array */
    int movesnakeHeadY = snakeBodyY[0];

    /* Creating our previous varaibles to hold our previus position */
    int prevx, prevy;
    snakeBodyX[0] = snakeHeadX; /* so at [0] we are now inputing variables in the element ans specificly now getting the headx(position of it) */
    snakeBodyY[0] = snakeHeadY;/*So at [0] element of body_y we are putting snakeHeadY */

    for(int i = 1; i < snakeLength; i++)
    {
        /* Here we have prevx & prevy == to its relevant array coordinate. Then we have the array coordinates beinging incremented and == to where the movesnakeHeadX & Y is. Then as we progress we then move that into the previous X & Y again. Thus, a continuing play. */
        prevx = snakeBodyX[i];    /* in the prevx is going to (momentarly) save the elements of bodyx[i] (by using i) so then */
        prevy = snakeBodyY[i];
        snakeBodyX[i] = movesnakeHeadX;/* we are doing the same for the y variable as well */
        snakeBodyY[i] = movesnakeHeadY;
        movesnakeHeadX = prevx;
        movesnakeHeadY = prevy;
    }
    /* This is reacting to keyboard input. Then it updates the coordinates in the gameplay. */
    switch(direction)
    {
        case(0):
            snakeHeadY--;   /* KEY_UP */
            break;
        case(1):
            snakeHeadY++;   /* KEY_DOWN */
            break;
        case(2):
            snakeHeadX--;   /* KEY_LEFT */
            break;
        case(3):
            snakeHeadX++;   /* KEY_RIGHT */
            break;
    }
    /* When the coordinates of the head match the trophy coordinates, we update snakeLength, etc. */
    if(snakeHeadX == trophyX && snakeHeadY == trophyY)
    {
            switch(trophyValue)
                {
                    case(1):
                        snakeLength += trophyValue;
                        break;
                    case(2):
                        snakeLength += trophyValue + 1;
                        break;
                    case(3):
                        snakeLength += trophyValue + 2;
                        break;
                    case(4):
                        snakeLength += trophyValue + 5;
                        break;
                }
                snakeSpeed += 5; /*Once trophy hit,then speed increases for whole game*/
        drawTrophy(); /* drawing the new snakeLength of the snake with trophyValue*/
    }
    /*  This checks for the time expiration fot the trophy.
    **  This isn't exactly 1-5 seconds but this looks better for
    **  the gameplay.
    */
    if(trophyTimeCount == trophyTime)
    {
        drawTrophy();
    }
    else
    {
        trophyTimeCount++;
    }
    /* This if condition is checking for if the WON condition has been statisfied .. change to 4 so we can at least win.. */
    if(snakeLength > (WidthOfGame/2))
    {
        WON = true;
        continuePlay = false;
    }
    /*  This if statemeent is checking for if you lost the game.
    **  Specifically if the head of the snake is toouching the
    **  border or head touches its body.
    */
    if(snakeHeadX < 0 || snakeHeadX > WidthOfGame || snakeHeadY < 0 || snakeHeadY > HeightOfGame)
    {
        WON = false;
        continuePlay = false;
    }
    for(int i = 0; i < snakeLength; i++)
    {
        if(snakeHeadX == snakeBodyX[i] && snakeHeadY == snakeBodyY[i])
        {
            WON = false;
            continuePlay = false;
        }
    }
}
/* Written by Haval Mohammed */
/* gets the keyboard inputs for moves */
void myMove()
{
    int ch = getch();
    switch(ch)
    {
        case KEY_UP:
            direction = 0;
            break;
        case KEY_DOWN:
            direction = 1;
            break;
        case KEY_LEFT:
            direction = 2;
            break;
        case KEY_RIGHT:
            direction = 3;
            break;
    }
}
/*  This function is drawing the trophies randomly on the canvas.
**  With a radnom tripy from 1, 3, 5, or 9
**  Written by Haval Mohammed
*/
void drawTrophy()
{
    trophyTimeCount = 0; /* Counter for the time the trophy has been displayed */
    trophyX = (rand() % (WidthOfGame - 5)); /*  Here is where trophyX is == to the logic of rand() method by modulo by WidthOfGame - 5, so that the randomly selected trophy is not touching the border from the top or bottom*/
    trophyY = (rand() % (HeightOfGame - 5)); /*  Here is where trophyY is == to the logic of rand() method by modulo by HeightOfGame - 5, so that the randomly selected trophy is not touching the border from the left or right*/
    trophyTime = ((rand() % 4) + 1) * 30; /* Here is our trophyTime, from 0 to 4, where we then add 1 so which is 1 to 5. The *10 helps us slow down the radnom trophy time*/
    trophyValue = (rand() % 4) + 1;/* This is where randomly selected numbers are designated from Case 1 to case 4 from drawGame() function */
}
/*  This function updates the game for each round.
**  Meaning after you have collected a trophy, the next 'level'
**  is introduced.
**  Written Haval Mohammed
*/
void drawGame()
{
    clear();    /* clear screen */
    /*  With every call this goes thru all the coordinates and
    **  checks the coordinates of the trophies,
    **  the snake, the border, etc.
    **  They are then printed on that location/coordinate.
    */
    for(int y = 0; y < HeightOfGame; y++)
    {
        for(int x = 0; x < WidthOfGame; x++)
        {
            move(y,x);
             /* This is adding the default snake head 'o' to that == x && y coordinate. */
            if( x == snakeHeadX && y == snakeHeadY)
            {
                addstr("o" );
                
            }
            if(y == 0 || x == 0) /* Here we're saying if the y or x coordinates are == 0, then go ahead and add the string "S" to that border */
            {
                addstr("S");
            }
            /* This is to add the srting "S" to the bottom and right borders of the canvas.*/
            if(y == HeightOfGame - 1 || x == WidthOfGame - 1)
            {
                addstr("S");
            }
            /* This is where we add our 1, 3, 5, & 9 numbers to the game. The coordinates such as (x,y).*/
            if( x == trophyX && y == trophyY)
            {
                switch(trophyValue)
                {
                    case(1):
                        addstr("1");
                        break;
                    case(2):
                        addstr("3");
                        break;
                    case(3):
                        addstr("5");
                        break;
                    case(4):
                        addstr("9");
                        break;
                }
            }
            /*  This where where initialize hasTail variable.
            **  Our for loop statement is starting the i variable
            **  from 0, and as long as i is less than the snakeLength,
            **  increment i.
            */
            else
            {
                bool hasTail = false;
                for(int i = 0; i < snakeLength; i++)
                {
                    /* This nested if statement has an
                    incremented/arbitrary element from i &&  the snake head x & y arrays have to == their related x && y coordinates.*/
                    if(snakeBodyX[i] == x && snakeBodyY[i] == y)
                    {
                        /* This is where the additional 'o' are added to the tail of the snake depending in which trophy they consume */
                        addstr("o");
                        hasTail = true;
                    }
                }
                /* If there is no tail, example when starting the game, then add space as the tail */
                if(!hasTail)
                {
                    addstr(" ");    /* The space */
                }
            }
        }
    }
}
