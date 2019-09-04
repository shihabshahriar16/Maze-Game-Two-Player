/*Maze */
#include<cstdio>
#include<string.h>
#include <iostream>
#include <ctime>        // Used for random number generator used as parameter in srand only once to provide it with a initial number so that it can generate a random number using complex algorithms
#include <windows.h>    // Used for getstd_handle which is used to pick up the console cursor and also used in SetConsoleCursorPosition to place it in any place desired to start writing from there
#include <conio.h>
#include <stdbool.h> // Used for backtracking path
#include <fstream>      // save/output maze into file
using namespace std;
#define range 21 // range of the maze has to be an odd number for maze traversal reasons
int highscore=0 ;
char s[7][150]= {"::::    ::::      :::     ::::::::: ::::::::::      :::::::::  :::    ::: ::::    ::: ::::    ::: :::::::::: :::::::::  ","+:+:+: :+:+:+   :+: :+:        :+:  :+:             :+:    :+: :+:    :+: :+:+:   :+: :+:+:   :+: :+:        :+:    :+: ","+:+ +:+:+ +:+  +:+   +:+      +:+   +:+             +:+    +:+ +:+    +:+ :+:+:+  +:+ :+:+:+  +:+ +:+        +:+    +:+ ","+#+  +:+  +#+ +#++:++#++:    +#+    +#++:++#        +#++:++#:  +#+    +:+ +#+ +:+ +#+ +#+ +:+ +#+ +#++:++#   +#++:++#:  ","+#+       +#+ +#+     +#+   +#+     +#+             +#+    +#+ +#+    +#+ +#+  +#+#+# +#+  +#+#+# +#+        +#+    +#+ ","#+#       #+# #+#     #+#  #+#      #+#             #+#    #+# #+#    #+# #+#   #+#+# #+#   #+#+# #+#        #+#    #+# ","###       ### ###     ### ######### ##########      ###    ###  ########  ###    #### ###    #### ########## ###    ###"};
int sp=0;

struct sq
{
    bool visited ;
    bool u_side ;
    bool d_side ;
    bool l_side ;
    bool r_side ;
    char display ;
};
typedef struct sq sq;
void backcolor()
{
    HANDLE hOut ;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut,BACKGROUND_INTENSITY| BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
}
void changecolor( char ch)
{
    HANDLE hOut ;
    if(ch=='R')
    {
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hOut,FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_INTENSITY| BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
    }
    else if(ch=='G')
    {
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hOut,FOREGROUND_GREEN | BACKGROUND_INTENSITY |BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
    }
    else if(ch=='B')
    {
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hOut,FOREGROUND_BLUE | BACKGROUND_INTENSITY |BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
    }
    else if(ch=='N')
    {
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hOut,BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
    }
}
void goto00()
{
    HANDLE hOut ;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0 ;
    Position.Y = 0 ;
    SetConsoleCursorPosition(hOut, Position);
}
void gotoxy(int x, int y)
{

    HANDLE hOut ;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = x ;
    Position.Y = y ;
    SetConsoleCursorPosition(hOut, Position);

}
//initialsq valuse for maze generation
void initialsq(sq L[][range])
{
    int i, j ;
    for(i=0; i<range; i++)
    {
        for(j=0; j<range; j++)
        {
            L[i][j].display = 223;      // All sqs are initially closed
            L[i][j].visited = false;    // sqs are initialsqd to not visited
            L[i][j].u_side = true;    // each sq contains 4 accessible walls
            L[i][j].d_side = true;
            L[i][j].l_side = true;
            L[i][j].r_side = true;
        }
    }

    for(i=1; i<range-1; i++)
    {
        for(j=1; j<range-1; j++)
        {
            L[1][j].u_side = false;   // border sqs have fewer accessible walls
            L[range-2][j].d_side = false;
            L[i][1].l_side = false;
            L[i][range-2].r_side = false;
        }
    }
}

//drawing loader maze
void loadingmaze(sq l0[][range])
{
    int i, j ;
    if(sp<70&&sp%10==0)
    {
        gotoxy(30,2+sp/10);
        cout << s[sp/10];
    }
    sp++;
    goto00();
    for(i=0; i<range; i++)
    {
        gotoxy(70,10+i);
        for(j=0; j<range; j++)
        {
            cout << l0[i][j].display << ' ';//printf(" %c",&l1[i][j].display);
            //cout << endl;//printf("\n");
        }
    }
    goto00();
}
//redraw maze
void redraw(sq l1[][range],sq l2[][range])
{
    int i, j ;
    for(i=0; i<range; i++)
    {
        for(j=0; j<range; j++)
            cout << l1[i][j].display << ' ';//printf(" %c",&l1[i][j].display);
        cout << "                                                            " ;
        for(j=0; j<range; j++)
            cout << l2[i][j].display << ' ';//printf(" %c",&l1[i][j].display);
        cout << endl;//printf("\n");
    }
}

//generate maze
void generatemaze(sq L[][range],int x)
{
    srand((unsigned)time(NULL));// pick a random start sq
    int dir;
    int randx;
    int randy;
    if(x==0)    //for loading maze start point
    {
        randx=1;
        randy=1;
    }
    else if(x==1)    //for 1st maze start point
    {
        randx=1;
        randy=range-2;
    }
    else if(x==2)        //for 2nd maze start point
    {
        randx=range-2;
        randy=1;
    }
    int visitedsqs=1; //total maze sqs visited excluding display and border sqs
    int totalsqs=((range-1)/2)*((range-1)/2);//total maze sqs excluding display and border sqs
    int percent;
    int backtrackx[1000],backtracky[1000],pointx=0,pointy=0;//array is used to trace the reverse path
    L[randy][randx].display='S'; // set S as the start sq
    L[randy][randx].visited=true;//set start sq as visited
    while(visitedsqs<totalsqs)         // loop until all sqs have been visited
    {
        if(((L[randy-2][randx].visited==false)&&(L[randy][randx].u_side==true)&&(L[randy-2][randx].d_side==true))||
                ((L[randy+2][randx].visited==false)&&(L[randy][randx].d_side==true)&&(L[randy+2][randx].u_side==true))||
                ((L[randy][randx-2].visited==false)&&(L[randy][randx].l_side==true)&&(L[randy][randx-2].r_side==true))||
                ((L[randy][randx+2].visited==false)&&(L[randy][randx].r_side==true)&&(L[randy][randx+2].l_side==true)))
        {
            dir=(rand()%4)+1;          //pick a random wall 1-4 to knock down
            //GO UP
            if((dir==1)&&(randy>1))
            {
                if(L[randy-2][randx].visited==false)    //if not visited
                {
                    L[randy-1][randx].display=' ';     //delete display to knock down wall
                    if (x==1)
                        gotoxy(2*randx,randy-1);
                    else if(x==2)
                        gotoxy(60+2*range+2*randx,randy-1);
                    cout << ' ' ;
                    L[randy-1][randx].visited=true;    //visited sq
                    L[randy][randx].u_side=false;   //set upper side of the sq x,y as false since that is where the connection between x,y and x-1,y is
                    backtrackx[pointx]=randx;                  // save X for  backtrack
                    backtracky[pointy]=randy;                  // save Y for backtrack
                    pointx++;
                    pointy++;
                    randy -=2;                           // move to next sq
                    L[randy][randx].visited=true;     // visited sq
                    L[randy][randx].display=' ';        //visited path
                    if(x==1)
                        gotoxy(2*randx,randy);
                    else if(x==2)
                        gotoxy(2*range+2*randx+60,randy);
                    cout << ' ' ;
                    L[randy][randx].d_side=false;   //knockdown wall
                    visitedsqs++;                       //increase visited sqs
                }
                else
                    continue;
            }
            //Go Down
            else if((dir==2)&&(randy<range-2))
            {
                if(L[randy+2][randx].visited==false)  //if not visited
                {
                    L[randy+1][randx].display=' '; //delete display
                    if(x==1)
                        gotoxy(2*randx,randy+1);
                    else if(x==2)
                        gotoxy(60+2*range+2*randx,randy+1);
                    cout << ' ' ;
                    L[randy+1][randx].visited=true;    //visited sq
                    L[randy][randx].d_side=false;   //knock down wall

                    backtrackx[pointx]=randx;                  // save X for  backtrack
                    backtracky[pointy]=randy;                // save Y for backtrack
                    pointx++;
                    pointy++;
                    randy +=2;                                // move to next sq
                    L[randy][randx].visited=true;     // visited sq
                    L[randy][randx].display=' ';      //visited path
                    if(x==1)
                        gotoxy(2*(randx),randy);
                    else if(x==2)
                        gotoxy(2*(range+randx)+60,randy);
                    cout << ' ' ;
                    L[randy][randx].u_side=false;   //knockdown wall
                    visitedsqs++;                             //increase visited sqs
                }
                else
                    continue;
            }

            //GO Left
            else if((dir==3)&&(randx>1))
            {
                if(L[randy][randx-2].visited==false)  // If not visited
                {
                    L[randy][randx-1].display=' ';    // Delete display
                    if(x==1)
                        gotoxy(2*(randx-1),randy);
                    else if(x==2)
                        gotoxy(60+2*range+2*(randx-1),randy);
                    cout << ' ' ;
                    L[randy][randx-1].visited=true;   // Visited sq
                    L[randy][randx].l_side=false;  // Knock down sq
                    backtrackx[pointx]=randx;                 // save X for  backtrack
                    backtracky[pointy]=randy;                // save Y for backtrack
                    pointx++;
                    pointy++;
                    randx -= 2;                               // Move to next sq
                    L[randy][randx].visited=true;     // Visited sq
                    L[randy][randx].display=' ';      // visited path
                    if(x==1)
                        gotoxy(2*(randx),randy);
                    else if(x==2)
                        gotoxy(60+2*(range+randx),randy);
                    cout << ' ' ;
                    L[randy][randx].r_side=false;              // knock down wall
                    visitedsqs++;                             // increase visited sqs
                }
                else
                    continue ;
            }

            //GO Right
            else if((dir==4)&&(randx<range-2))
            {
                if(L[randy][randx+2].visited==false)  // If not visited
                {
                    L[randy][randx+1].display=' ';    // Delete display
                    if(x==1)
                        gotoxy(2*(randx+1),randy);
                    else if(x==2)
                        gotoxy(60+2*(range+randx+1),randy);
                    cout << ' ' ;
                    L[randy][randx+1].visited=true;   // Visited sq
                    L[randy][randx].r_side=false; // Knock down sq
                    backtrackx[pointx]=randx;                  // save X for  backtrack
                    backtracky[pointy]=randy;              // save Y for backtrack
                    pointx++;
                    pointy++;
                    randx += 2;                        // Move to next sq
                    L[randy][randx].visited=true;     // Visited sq
                    L[randy][randx].display=' ';      // Visited path
                    if(x==1)
                        gotoxy(2*(randx),randy);
                    else if(x==2)
                        gotoxy(60+2*range+2*(randx),randy);
                    cout << ' ' ;
                    L[randy][randx].l_side=false;     // knock down wall
                    visitedsqs++;                      // increase visited sqs
                }
                else
                    continue;
            }
            percent=visitedsqs*100/totalsqs;    // progress in percentage if needed
            if(x==0)
            {
                gotoxy(75,10+range+2);
                cout << "LOADING... " << percent << " %" ;
            }
        }
        else                                                // Backtrack using stack
        {
            pointx--;
            randx=backtrackx[pointx];
            pointy--;
            randy=backtracky[pointy];

        }
        if(x==0)
        {
            goto00();                                      // Clear screen
            loadingmaze(L);                                      // redraw
        }
    }
    if(x==1)
    {
        L[1][range-2].display='E';
        gotoxy(2,range-2);
        cout << 'S';
        gotoxy(2*range-4,1);
        cout << 'E';
    }
    else if(x==2)
    {
        L[range-2][1].display='E';
        gotoxy(2*range+60+2,range-2);
        cout << 'E';
        gotoxy(4*range+60-4,1);
        cout << 'S';
    }
    if(x==1)
        gotoxy(5,range+2);
    else if(x==2)
        gotoxy(5+60+2*range,range+2);
}
void play(sq l1[][range],sq l2[][range],string player1,string player2,int cnt)
{
    char ch=254,p;
    int r1,c1,r2,c2;
    r1=range-2;
    c1=1;
    gotoxy(2*c1,r1);//player 1's initial position
    changecolor('B');
    cout << ch;
    r2=1;
    c2=range-2;
    gotoxy(2*range+60+2*c2,r2);//player 2's initial position
    changecolor('R');
    cout << ch;
    gotoxy(0,range+5);
    int p1=0, p2=0, c=1, cmdwidth=100, cmdhight=25 ;
    time_t start_t, end_t,t;
    double diff_t;
    time(&start_t);
    while(!((r1==1&&c1==range-2)||(r2==range-2&&c2==1)))
    {
        time(&t);
        if(difftime(t,start_t)>cnt)
        {
            gotoxy(0,range+1);
            cout << "Time limit has been reached !!\n";
            break;
        }
        p=getch();
        if(p==97)
        {
            if(l1[r1][c1].l_side==false&&c1!=1)
            {
                gotoxy(2*c1,r1);
                cout << ' ' ;
                c1-=2;
            }
            gotoxy(2*c1,r1);
            changecolor('B');
            cout << ch;
            p1++;
        }
        else if(p==100)
        {
            if(l1[r1][c1].r_side==false&&c1!=range-2)
            {
                gotoxy(2*c1,r1);
                cout << ' ' ;
                c1+=2;
            }
            gotoxy(2*c1,r1);
            changecolor('B');
            cout << ch;
            p1++;
        }
        else if(p==119)
        {
            if(l1[r1][c1].u_side==false&&r1!=1)
            {
                gotoxy(2*c1,r1);
                cout << ' ' ;
                r1-=2;
            }
            gotoxy(2*c1,r1);
            changecolor('B');
            cout << ch;
            p1++;
        }
        else if(p==115)
        {
            if(l1[r1][c1].d_side==false&&r1!=range-2)
            {
                gotoxy(2*c1,r1);
                cout << ' ' ;
                r1+=2;
            }
            gotoxy(2*c1,r1);
            changecolor('B');
            cout << ch;
            p1++;
        }
        else if(p==75)
        {
            if(l2[r2][c2].l_side==false&&c2!=1)
            {
                gotoxy(2*range+2*c2+60,r2);
                cout << ' ' ;
                c2-=2;
            }
            gotoxy(2*range+2*c2+60,r2);
            changecolor('R');
            cout << ch;
            p2++;
        }
        else if(p==77)
        {
            c++;
            if(l2[r2][c2].r_side==false&&c2!=range-2)
            {
                gotoxy(2*range+2*c2+60,r2);
                cout << ' ' ;
                c2+=2;
            }
            gotoxy(2*range+2*c2+60,r2);
            changecolor('R');
            cout << ch;
            p2++;
        }
        else if(p==72)
        {
            if(l2[r2][c2].u_side==false&&r2!=1)
            {
                gotoxy(2*range+2*c2+60,r2);
                cout << ' ' ;
                r2-=2;
            }
            gotoxy(2*range+2*c2+60,r2);
            changecolor('R');
            cout << ch;
            p2++;
        }
        else if(p==80)
        {
            if(l2[r2][c2].d_side==false&&r2!=range-2)
            {
                gotoxy(2*range+2*c2+60,r2);
                cout << ' ' ;
                r2+=2;
            }
            gotoxy(2*range+2*c2+60,r2);
            changecolor('R');
            cout << ch;
            p2++;
        }
    }
    time(&end_t);
    gotoxy(range+10,range+2);
    diff_t = difftime(end_t, start_t);
    char s1[38][50]= {"              ,(/             ((//\\          ","           (    (       ..    .     .(        ","         (..,,/*     (  ( (    \'*,,,,,*      ","      ./            *..,    ,            *    ","     *             .   (   /,             (   ","   /((              *  /    (              *  "," ,    ,#*///*,*  (   ( (  ,,             (   ,","  (   *.      *  /*(*   .   ./*  ,       (  ..","                     (**(     ,(/, ;))((_*    ","                     (                        ","                    .                         ","                    (                         ","                    (                         ","                    /                         ","                    *                         ","                   *#*                        ","                ..      (                     ","                (       ,                     ","                 (*    (/                     ","                 (      (                     ","                (        /                    ","               (          /                   ","              (            (                  ","             /              (                 ","            .                *                ","           .,                 .               ","        .    *              (   .             ","        ..(  /              (  / .            ","                                (             ","         /                       .            ","        *                        /            ","        ,                         /           ","       /                          ,           ","       *                           (          ","     (,                            (,.        ","   (  (                            ,  (       ","./ .*(,                            *(**(      ","                                              "};
    if((r1==1&&c1==range-2) || (r2==range-2&&c2==1))
    {
        for(int i=0; i<38; i++)
        {
            gotoxy(2*range+2,i);
            printf("%s",s1[i]);
        }
    }
    gotoxy(0,range+2);

    changecolor('N');
    int i=(500-diff_t*10)+((50-cnt)*5);
    if(r1==1&&c1==range-2)
    {
        printf("Great timing %f!!!\n", diff_t);
        cout << player1 << " WINS\nScore : " << i << endl;
        highscore=i;
        string a=to_string(highscore);
        a+="(";
        if(p1>p2)
            a+=player1;
        else
            a+=player2;
        a+=") | ";
        ofstream mazegame("highscores.txt",ios::app);
        mazegame<<a;
        mazegame.close();
    }
    else if(r2==range-2&&c2==1)
    {
        printf("Great timing %f!!!\n", diff_t);
        cout << player2 << " WINS\nScore : " << i << endl;
        highscore=i;
        string a=to_string(highscore);
        a+="(";
        if(p1>p2)
            a+=player1;
        else
            a+=player2;
        a+=") | ";
        ofstream mazegame("highscores.txt",ios::app);
        mazegame<<a;
        mazegame.close();
    }
    else
        cout << "Both of the player lost\n";

}
//save Maze
void save(sq l1[][range])
{
    ofstream output;
    char file[20];
    char input;
    printf("\nsave Maze? (Y)/(N);");
    cout << endl << "save Maze? y/n  " ; //scanf("%c",&input);               //save answer
    input=getch();
    if((input== 'y' || input == 'Y'))
    {
        cout << endl << "save as: ";//printf("\nsave as: ");
        cin >> file;//scanf("%s",&file);
        output.open(file);      //open file to be written
        for(int i=0; i<range; i++)
        {
            output << endl;
            for(int j=0; j<range; j++)
            {
                output << l1[i][j].display << " " ; //insert the maze into a file
            }
        }
        cout << "Maze saved to" << "\"" << file << "\"" <<endl;//printf("Maze has been saved to : \ %s \\n",file);
        output.close();         //close file
    }
}

int menu()
{
    system("cls");
    int curpos=1 ;
    char ch ;
    while(1)
    {
        goto00();
        if(curpos==1)
            printf("\t==>> 1. START A NEW GAME\n");
        else
            printf("\t     1. Start a new game\n");
        if(curpos==2)
            printf("\t==>> 2. VIEW HIGH SCORE\n");
        else
            printf("\t     2. View high score\n");
        if(curpos==3)
            printf("\t==>> 3. SET DIFFICULTY\n");
        else
            printf("\t     3. Set difficulty\n");
        if(curpos==4)
            printf("\t==>> 4. HOW TO PLAY\n");
        else
            printf("\t     4. How to play\n");
        if(curpos==5)
            printf("\t==>> 5. EXIT GAME\n");
        else
            printf("\t     5. Exit game\n");

        printf("\nPress UP and DOWN key to navigate the menu \n");
        ch=getche();
        if(ch==72 && curpos!=1)
            curpos--;
        else if(ch==72 && curpos==1)
            curpos=500;
        if(ch==80 && curpos!=5)
            curpos++;
        else if(ch==80 && curpos==5)
            curpos=1;
        else if(ch==13)
            break ;
    }
    return curpos ;
}

void show_score()
{
    char arr[101];
    ifstream mazegame("highscores.txt");
    mazegame.getline(arr,100);
    cout<<arr<<endl;
}
int main()
{
    bool game_over=false ;
    char ch;
    sq l0[range][range],l1[range][range],l2[range][range];             // double array of structs for the grid
    initialsq(l0);
    loadingmaze(l0);
    generatemaze(l0,0);
    getch();
    backcolor();
    int tym=20;
    while(!game_over)
    {
        int x, c=1;
        x=menu();
        if(x==1)
        {
            //cout << tym << endl;
            string p1,p2;
            cout << "Enter the name of player 1 :";
            cin >> p1;
            cout << "Enter the name of player 2 :";
            cin >> p2;
            system("cls");
            initialsq(l1);                              // initialsq
            initialsq(l2);
            redraw(l1,l2);                                  // draw
            generatemaze(l1,1);                           // generate maze
            //getch();
            generatemaze(l2,2);
            //getch();
            play(l1,l2,p1,p2,tym);
            save(l1);
            save(l2);                                    // save maze to text file
            cout << "Press Enter to continue";
            while(1)
            {
                ch=getch();
                if(ch==13)
                    break;
            }
        }

        else if(x==2)
        {
            system("cls");
            cout << "HIGHSCORE : " << highscore << endl;
            char choice;
            cout << "Do you want to see full highscores list?(Y/N) : ";
            choice=_getch();
            if(choice=='Y' || 'y')
                show_score();
            cout << "Press any key to continue";
            _getch();
        }
        else if(x==3)
        {
            system("cls");
            char ch;
            while(1)
            {
                goto00();
                if(c==1)
                    printf("\t==>> 1. VERY HARD\n");
                else
                    printf("\t     1. Very hard\n");
                if(c==2)
                    printf("\t==>> 2. HARD\n");
                else
                    printf("\t     2. Hard\n");
                if(c==3)
                    printf("\t==>> 3. NORMAL\n");
                else
                    printf("\t     3. Normal\n");
                if(c==4)
                    printf("\t==>> 4. EASY\n");
                else
                    printf("\t     4. Easy\n");
                if(c==5)
                    printf("\t==>> 5. VERY EASY\n");
                else
                    printf("\t     5. Very easy\n");

                printf("\nPress UP and DOWN key to navigate the menu \n");
                ch=getche();
                if(ch==72 && c!=1)
                    c--;
                else if(ch==72 && c==1)
                    c=5;
                if(ch==80 && c!=5)
                    c++;
                else if(ch==80 && c==5)
                    c=1;
                else if(ch==13)
                    break ;
            }
            tym=5+c*5;
        }

        else if(x==4)
        {
            system("cls");
            cout << "1. Use the keys (w,a,s,d) to operate the left maze.\n2. Use the arrow keys to operate the right maze.\n3. The player to reach endpoint first wins.\n" ;
            cout << "Press any key to continue";
            getch();
        }

        else if(x==5)       // exit game
        {
            game_over = true ;
            system("cls");
            cout << "Good bye" << endl;//printf("Good bye!\n") ;
        }
    }

    //_getch();
    return 0;
}
