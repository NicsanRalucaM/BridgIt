#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <cstring>
#include <winbgim.h>
#include <time.h>
#include <stdlib.h>
#include <graphics.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include<cstring>
#define MAX 20
#include "BridgIt.h"
using namespace std;
int LG=9,nr_optiuni, nr_meniu,seconds,minutes;
char s1[MAX],s2[MAX],s3[MAX],regulitext[10001],linieclasament[100],urmeaza1[30],urmeaza2[30],urmeaza3[30];
int nrmutari=0,msec,xstart=800-((LG/2)*50)-50,ystart=400-((LG/2)*50)-50,x,y,C1=8,C2=8,culoareBifaR=2,culoareBifaE=4,culoare_subBifa=14;
bool gata,ruleaza=true,nume_joc_de2=false,nume_joc_de1=false;
char nrtimer[10],minutesTimer[10],nrMUTARI[10];
int viz1[200],viz2[200], matrice1[200][200],matrice2[200][200],TablaDeJoc[200][200], scor,scor1,scor2;

struct buton
{
    int l,t,r,b,xt,yt,f,fs;
    bool pressed;
    //l- left, t-top, r-right, b-bottom, xt- x text, yt- y text, f- font, fs- fontsize;
} ;
buton engleza, romana, easy, medium, hard, backBUTON, singlePlayer, multiPlayer,hints,start,options,music,undo,newgame,score,stergeB;
///music.pressed=true;
void initializare_buton(buton &m,int left,int top,int xtext,int ytext,int brsize,int bbsize,int font,int fontsize) ///alex
{
    m.l=left;
    m.t=top;
    m.r=m.l+brsize;
    m.b=m.t+bbsize;
    m.xt=m.l+xtext;
    m.yt=m.t+ytext;
    m.f=font;
    m.fs=fontsize;
    m.pressed=false;
}
void drawB(buton a, char nume[])//functie pentru desenat butoane  ---alex
{
    rectangle(a.l,a.t,a.r,a.b);
    settextstyle(a.f,0,a.fs);
    outtextxy(a.xt,a.yt,nume);
}

void DFS(int x,int v[200],int mat[200][200])//fuctie de parcurgere a grafului in adancime ---raluca
{
    v[x]=1;
    for(int i=1; i<=(LG/2)*(LG/2+1); i++)
        if(v[i]==0 && (mat[x][i]==1||mat[i][x]==1))
            DFS(i,v,mat);
}
void creare_matrice1_graf()//functie in care este reprezentat graful1 in matrice1    ---raluca
{
    int  x=0,y=0,i,j;

    for ( i=1; i<=(LG/2)*(LG/2+1); i++)//initializeaza matricea grafului 1
        for( j=1; j<=(LG/2)*(LG/2+1); j++)
            matrice1[i][j]=0;

    for ( i=1; i<=LG; i++)
        for( j=1; j<=LG; j++)
        {
            if(j%2==0)
                if(i%2!=0)
                    x++;
                else
                    y++;
            if(TablaDeJoc[i][j]==3)//actualizeaza matricea grafului 1 la fiecare muchie gasita
                if(j%2!=0)//in functie de tipul acesteia:
                {
                    matrice1[x][x+1]=1;//orizontala
                    matrice1[x+1][x]=1;
                }
                else
                {
                    matrice1[y][y+LG/2]=1;//verticala
                    matrice1[y+LG/2][y]=1;
                }
        }
}
void creare_matrice2_graf()//functie in care este reperezentat graful2 in matrice2 ---raluca
{
    int  x=0,y=0,i,j;

    for ( i=1; i<=(LG/2)*(LG/2+1); i++)//initilizeaza matricea grafului 2
        for( j=1; j<=(LG/2)*(LG/2+1); j++)
            matrice2[i][j]=0;

    for ( i=2; i<=LG; i++)
        for( j=1; j<=LG; j++)
        {
            if(j%2!=0)
                if(i%2==0)
                    x++;
                else
                    y++;
            if(TablaDeJoc[i][j]==4)//actualizeaza matricea grafului 2 la fiecare muchie gasita
                if(j%2==0)//in functie de tipul acesteia:
                {
                    matrice2[x][x+1]=1;             //orizontala
                    matrice2[x+1][x]=1;
                }
                else
                {
                    matrice2[y][y+LG/2+1]=1;    //verticala
                    matrice2[y+LG/2+1][y]=1;
                }
        }
}
void afisare()
{
    for (int i=1; i<=LG; i++)
    {
        for(int j=1; j<=LG; j++)
            cout<<TablaDeJoc[i][j]<<" ";
        cout<<endl;
    }
    cout<<"............"<<endl;
    for (int i=1; i<=(LG/2)*(LG/2+1); i++)
    {
        for(int j=1; j<=(LG/2)*(LG/2+1); j++)
            cout<<matrice2[i][j]<<" ";
        cout<<endl;
    }
    cout<<"------------------"<<endl;
    for(int i=1; i<=LG/2*(LG/2+1); i++)
        cout<<viz1[i]<<" ";
    cout<<endl;
    cout<<nrmutari<<endl;
}
int nrcol(int x)// calculeaza nr coloanei   --emilia
{
    if(x<=LG*50+xstart)
        return x/50+1;
    else
        return -1;
}
int nrlin(int x)// calculeaza nr liniei   ----emilia
{
    if(x<=LG*50+ystart)
        return x/50+1;
    else
        return -1;
}
void creareTablaDeJoc()//initializeaza matricea tabel de joc, marcand pozitiile punctelor    ---emilia+raluca
{
    for(int i=1; i<=LG; i++)
        for(int j=1; j<=LG; j++)
            TablaDeJoc[i][j]=0;

    TablaDeJoc[1][1]=TablaDeJoc[1][LG]=TablaDeJoc[LG][1]=TablaDeJoc[LG][LG]=8;//pozitie imposibila
    for(int i=1; i<=LG; i+=2)
    {
        for(int j=2; j<=LG; j+=2)
        {
            TablaDeJoc[i][j]=1; //prima culoare C1
            TablaDeJoc[j][i]=2; //a doua culoare C2
        }
    }
}
void desenareTablaDeJoc() //---emilia+raluca
{
    setcolor(WHITE);
    int i=1,j=1,x,y;
    cleardevice();
    creareTablaDeJoc();
    initializare_buton(backBUTON,1250,600,35,30,225,100,10,5);
    initializare_buton(undo,30,400,45,30,200,100,10,5);
    initializare_buton(newgame,1250,320,10,30,225,100,10,5);
    if(music.pressed==true)
    {
        initializare_buton(music,1250,460,35,30,225,100,10,5);
        music.pressed=true;
    }
    else
        initializare_buton(music,1250,460,35,30,225,100,10,5);

    if(engleza.pressed==true)
    {
        drawB(backBUTON, "BACK");
        drawB(undo,"UNDO");
        drawB(newgame,"NEW GAME");
        if(music.pressed==true)
            setcolor(RED);
        else
            setcolor(WHITE);
        drawB(music,"MUSIC");
        setcolor(WHITE);
    }
    if(romana.pressed==true)
    {
        drawB(undo,"UNDO");
        drawB(backBUTON,"MENIU");
        drawB(newgame,"JOC NOU");
        if(music.pressed==true)
            setcolor(RED);
        else
            setcolor(WHITE);
        drawB(music,"Muzica");
        setcolor(WHITE);
    }
/// spatiu pentru desenat butoanele de pe langa tabla comune toturor nivelelor

    //tabla de joc
    for(y=50+ystart; y<=LG*50+ystart; y+=50)
    {
        for(x=xstart+50; x<=LG*50+xstart; x+=50)
        {
            if(!((i==1&&j==1) || (i==1 && j==LG) || (i==LG && j==1) || (i==LG && j==LG)))
            {
                if(TablaDeJoc[i][j]==1)
                {
                    setcolor(C1);
                    setfillstyle(SOLID_FILL,C1);
                    fillellipse((x-30),(y-30),15,15);  //58=(x/2)-diam/2
                }
                if(TablaDeJoc[i][j]==2)
                {
                    setcolor(C2);
                    setfillstyle(SOLID_FILL,C2);
                    fillellipse((x-30),(y-30),15,15);
                }
            }
            j++;
        }
        i++;
        j=1;
    }
}
int castigator1() //functia returneaza 1 daca a castigat C1 ---raluca
{
    int x=0;
    creare_matrice1_graf();

    for (int i=1; i<=LG/2; i++)
    {
        for(int i=1; i<=LG/2*(LG/2+1); i++)
            viz1[i]=0;

        DFS(i,viz1,matrice1);
        for(int j=LG/2*(LG/2)+1; j<=LG/2*(LG/2+1); j++)
            if(viz1[j]==1)
            {
                gata=true;
                return 1;
            }
    }
    return 0;
}
int castigator2()// functia returneaza 1 daca a castigat C2  ---raluca
{
    int x=0;
    creare_matrice2_graf();
    for (int i=1; i<=(LG/2+1)*(LG/2-1)+1; i=i+LG/2+1)
    {
        for(int i=1; i<=LG/2*(LG/2+1); i++)
            viz2[i]=0;

        DFS(i,viz2,matrice2);
        for(int j=(LG/2)+1; j<=LG/2*(LG/2+1); j=j+LG/2+1)
            if(viz2[j]==1)
            {
                gata=true;
                return 1;
            }
    }
    return 0;
}
void verificare_castigator()//functia verifica daca exista un castigator ---raluca si alex
{
    char s11[20]= {0},s22[20]= {0},s33[20]= {0};
    FILE *f;
    if(hard.pressed==true&&singlePlayer.pressed==true)
    {
        if(castigator1()==1)
        {
            scor2++;
            f=fopen("ClasamentCurent.txt","w+");
            settextstyle(BOLD_FONT,0,5);
            setcolor(RED);
            if(romana.pressed==true)
            {
                if(singlePlayer.pressed==true)
                {
                    outtextxy(590,85,"Computerul a castigat!");
                    fprintf(f, "   Computer: %d      %s: %d",scor2,s3,scor1);
                }
                else
                {
                    setcolor(RED);
                    strcpy(s22,s2);
                    strcat(s22," a castigat");
                    outtextxy(600,85,s22);
                    fprintf(f, "    %s: %d      %s: %d",s1,scor1,s2,scor2);
                }
            }
            else
            {
                if(singlePlayer.pressed==true)
                {

                    outtextxy(590, 85, "Computer won");
                    fprintf(f, "   Computer: %d      %s: %d",scor2,s3,scor1);
                }
                else
                {
                    setcolor(RED);
                    strcpy(s22,s2);
                    strcat(s22," won!");
                     outtextxy(600,85,s22);
                    fprintf(f, "    %s: %d      %s: %d",s1,scor1,s2,scor2);
                    setcolor(WHITE);
                }

            }fclose(f);
            gata=true;

        }

        else if(castigator2()==1)
        {
            settextstyle(BOLD_FONT,0,5);
            setcolor(RED);
            if(romana.pressed==true)
            {
                scor1++;
                f=fopen("ClasamentCurent.txt","w+");
                if(singlePlayer.pressed==true)
                {
                    setcolor(RED);
                    strcpy(s33,s3);
                    strcat(s33," a castigat");
                    outtextxy(600,85,s33);
                    fprintf(f, "   Computer: %d      %s: %d",scor2,s3,scor1);
                }
                else
                {
                    setcolor(RED);
                    strcpy(s11,s1);
                    strcat(s11," a castigat");
                    outtextxy(600,85,s11);
                    fprintf(f, "    %s: %d      %s: %d",s1,scor1,s2,scor2);
                }
                fclose(f);
            }
            else
            {
                scor1++;
                f=fopen("ClasamentCurent.txt","w+");
                if(singlePlayer.pressed==true)
                {
                    setcolor(RED);
                    strcpy(s33,s3);
                    strcat(s33," won !!");
                    outtextxy(600,85,s33);
                    fprintf(f, "   Computer: %d      %s: %d",scor2,s3,scor1);
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(RED);
                    strcpy(s11,s1);
                    strcat(s11," won !!");
                    outtextxy(600,85,s11);
                    fprintf(f, "    %s: %d      %s: %d",s1,scor1,s2,scor2);
                    setcolor(WHITE);
                }

                fclose(f);
            }
            gata=true;
        }}
    else if(castigator1()==1)
    {
        settextstyle(BOLD_FONT,0,5);
        setcolor(RED);
        if(romana.pressed==true)
        {
            scor1++;
            f=fopen("ClasamentCurent.txt","w+");
            if(singlePlayer.pressed==true)
            {
                setcolor(RED);
                strcpy(s33,s3);
                strcat(s33," a castigat");
                outtextxy(600,85,s33);
                fprintf(f, "   Computer: %d      %s: %d",scor2,s3,scor1);
            }
            else
            {
                setcolor(RED);
                strcpy(s11,s1);
                strcat(s11," a castigat");
                outtextxy(600,85,s11);
                fprintf(f, "    %s: %d      %s: %d",s1,scor1,s2,scor2);
            }
            fclose(f);
        }
        else
        {
            scor1++;
            f=fopen("ClasamentCurent.txt","w+");
            if(singlePlayer.pressed==true)
            {
                setcolor(RED);
                strcpy(s33,s3);
                strcat(s33," won !!");
                outtextxy(600,85,s33);
                fprintf(f, "   Computer: %d      %s: %d",scor2,s3,scor1);
                setcolor(WHITE);
            }
            else
            {
                setcolor(RED);
                strcpy(s11,s1);
                strcat(s11," won !!");
                outtextxy(600,85,s11);
                fprintf(f, "    %s: %d      %s: %d",s1,scor1,s2,scor2);
                setcolor(WHITE);
            }

            fclose(f);
        }
        gata=true;
    }
    else if(castigator2()==1)
    {
        scor2++;
        f=fopen("ClasamentCurent.txt","w+");
        settextstyle(BOLD_FONT,0,5);
        setcolor(RED);
        if(romana.pressed==true)
        {
            if(singlePlayer.pressed==true)
            {
                outtextxy(590,85,"Computerul a castigat!");
                fprintf(f, "   Computer: %d      %s: %d",scor2,s3,scor1);
            }
            else
            {
                setcolor(RED);
                strcpy(s22,s2);
                strcat(s22," a castigat");
                outtextxy(600,85,s22);
                fprintf(f, "    %s: %d      %s: %d",s1,scor1,s2,scor2);
            }
        }
        else
        {
            if(singlePlayer.pressed==true)
            {

                outtextxy(590, 85, "Computer won");
                fprintf(f, "   Computer: %d      %s: %d",scor2,s3,scor1);
            }
            else
            {
                setcolor(RED);
                strcpy(s22,s2);
                strcat(s22," won!");
                outtextxy(600,85,s22);
                fprintf(f, "    %s: %d      %s: %d",s1,scor1,s2,scor2);
                setcolor(WHITE);
            }

        }fclose(f);
        gata=true;
    }
}

int exceptie(int l,int c) ///toti
{
    if((l==LG-3&&c==2)||(l==LG-5&&c==4)||(l==4&&c==6)||(l==2&&c==8))
        return 1;
    if((l==LG-3&&c==4)||(l==LG-5&&c==6)||(l==4&&c==8)||(l==2&&c==10))
        return 2;
    return 0;
}
void Undo()  //EMILIA
{
    setcolor(RED);
    settextstyle(3,0,5);
    if(romana.pressed==true)
        outtextxy(600,85,"GRABA STRICA TREABA!");
    else
        outtextxy(600,85,"BE SMARTER NEXT TIME!");
    undo.pressed=false;
    delay(2000);
    setcolor(BLACK);
    if(romana.pressed==true)
        outtextxy(600,85,"GRABA STRICA TREABA!");
    else
        outtextxy(600,85,"BE SMARTER NEXT TIME!");
    setcolor(WHITE);
    if(gata==true)
        verificare_castigator();
    setcolor(WHITE);
}
void punerePiesa()  ///toti
{
    int linia,coloana,coloanag,liniag;
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x=mousex()-xstart;
        y=mousey()-ystart;
        if(mousex()>=newgame.l&&mousex()<=newgame.r&&mousey()>=newgame.t&&mousey()<=newgame.b)
        {
            nrmutari=0;
            joc_de2();

        }
        if(x+xstart>=backBUTON.l&&x+xstart<=backBUTON.r&&y+ystart>=backBUTON.t&&y+ystart<=backBUTON.b)//buton back in optiuni
        {
            setcolor(WHITE);
            MainMenu();
        }
        if(x+xstart>=undo.l&&x+xstart<=undo.r&&y+ystart>=undo.t&&y+ystart<=undo.b)
        {
            undo.pressed=true;
            Undo();
        }
        if(x+xstart>=music.l&&x+xstart<=music.r&&y+ystart>=music.t&&y+ystart<=music.b)   //muzica ruleaza sau nu
        {
            music.pressed=(!music.pressed);
            if(music.pressed==true&&ruleaza==false)  // muzica nu ruleaza
            {
                PlaySound(TEXT("MusicForBridgItWAV.wav"),NULL,SND_LOOP|SND_ASYNC);
                if(engleza.pressed==true)
                {
                    setcolor(RED);
                    drawB(music,"MUSIC ");
                    setcolor(WHITE);
                }
                if(romana.pressed==true)
                {
                    setcolor(RED);
                    drawB(music,"MUZICA");
                    setcolor(WHITE);
                }
                ruleaza=true;
            }
            if(music.pressed==false&&ruleaza==true)
            {
                PlaySound(NULL, 0, 0);
                if(engleza.pressed==true)
                {
                    setcolor(WHITE);
                    drawB(music,"MUSIC ");
                }
                if(romana.pressed==true)
                {
                    setcolor(WHITE);
                    drawB(music,"MUZICA");
                }
                ruleaza=false;
            }
        }

        /// spatiu pentru conditii butoane in joc de 2
    }

    linia=nrlin(y);
    coloana=nrcol(x);
    coloanag=coloana+nrcol(xstart)-1;
    liniag=linia+nrlin(ystart)-1;
    if(TablaDeJoc[linia][coloana]==0&&coloanag*50>xstart&&liniag*50>ystart&&coloanag*50<=xstart+LG*50&&liniag*50<=ystart+LG*50&&gata==false)//conditii pentru a nu desena in portiunea xstart si ystart
    {
        if(nrmutari%2!=0)
        {
            setcolor(C1);
            setfillstyle(SOLID_FILL,C1);
            if(coloana%2!=0)
            {
                if(coloana+1<LG&&coloana-1>1)
                {
                    bar((((coloanag-1)*50) -25),(((liniag)*50)-25),(((coloanag-1)*50)+75),(((liniag)*50)-30));//orizontala
                    TablaDeJoc[linia][coloana]=3;
                    nrmutari++;
                    setcolor(BLACK);
                    outtextxy(100,50,urmeaza1);
                    setcolor(C2);
                    outtextxy(100,50,urmeaza2);
                    setcolor(WHITE);
                }
            }
            else
            {
                bar((coloanag)*50-30,((liniag-1)*50)+75,(coloanag)*50-25,(liniag-1)*50-25);//verticala
                TablaDeJoc[linia][coloana]=3;
                nrmutari++;
                setcolor(BLACK);
                outtextxy(100,50,urmeaza1);
                setcolor(C2);
                outtextxy(100,50,urmeaza2);
                setcolor(WHITE);

            }
        }
        else if(nrmutari%2==0)
        {
            setcolor(C2); //albastru
            setfillstyle(SOLID_FILL,C2);
            if(coloana%2==0)
            {
                bar((((coloanag-1)*50) -25),((liniag*50)-25),(((coloanag-1)*50)+75),(((liniag)*50)-30));//orizontala
                TablaDeJoc[linia][coloana]=4;
                nrmutari++;
                setcolor(BLACK);
                outtextxy(100,50,urmeaza2);
                setcolor(C1);
                outtextxy(100,50,urmeaza1);
                setcolor(WHITE);
            }
            else
            {
                if((linia+1<LG&&linia-1>1))
                {
                    bar((coloanag)*50-30,((liniag-1)*50)+75,(coloanag)*50-25,(liniag-1)*50-25);//verticala
                    TablaDeJoc[linia][coloana]=4;
                    nrmutari++;
                    setcolor(BLACK);
                    outtextxy(100,50,urmeaza2);
                    setcolor(C1);
                    outtextxy(100,50,urmeaza1);
                    setcolor(WHITE);
                }
            }
        }
        verificare_castigator();
    }
}
void afisare_scor() // emilia
{
    //nr mutare
    setcolor(WHITE);
    settextstyle(10,0,4);
    if(romana.pressed==true)
    {
        outtextxy(20,200,"Numarul de mutari:");
        if(hard.pressed==true&&singlePlayer.pressed==true)
            setcolor(C2);
        else
            setcolor(C1);
        outtextxy(20,250,"Numarul de mutari 1:");
        if(hard.pressed==true&&singlePlayer.pressed==true)
            setcolor(C1);
        else
            setcolor(C2);
        outtextxy(20,300,"Numarul de mutari 2:");
        setcolor(WHITE);
    }
    else
    {
        outtextxy(20,200, "Number of moves:");
        if(hard.pressed==true&&singlePlayer.pressed==true)
            setcolor(C2);
        else
            setcolor(C1);
        outtextxy(20,250, "Number of moves 1:");
        if(hard.pressed==true&&singlePlayer.pressed==true)
            setcolor(C1);
        else
            setcolor(C2);
        outtextxy(20,300, "Number of moves 2:");
        setcolor(WHITE);
    }
    if(nrmutari==0)
    {
        outtextxy(400,200, itoa(0,nrMUTARI,10));
        outtextxy(400,250, itoa(0,nrMUTARI,10));
        outtextxy(400,300, itoa(0,nrMUTARI,10));

    }
    else
    {
        setcolor(WHITE);
        outtextxy(400,200, itoa(nrmutari,nrMUTARI,10));
        if(nrmutari%2==0)
        {
            if(hard.pressed==true&&singlePlayer.pressed==true)
                setcolor(C2);
            else
                setcolor(C1);
            outtextxy(400,250, itoa(nrmutari/2,nrMUTARI,10));
            if(hard.pressed==true&&singlePlayer.pressed==true)
                setcolor(C1);
            else
                setcolor(C2);
            outtextxy(400,300, itoa(nrmutari/2,nrMUTARI,10));
            setcolor(WHITE);
        }
        else
        {
            if(hard.pressed==true&&singlePlayer.pressed==true)
                setcolor(C2);
            else
                setcolor(C1);
            outtextxy(400,250, itoa(nrmutari/2,nrMUTARI,10));
            if(hard.pressed==true&&singlePlayer.pressed==true)
                setcolor(C1);
            else
                setcolor(C2);
            outtextxy(400,300, itoa(nrmutari/2 +1,nrMUTARI,10));
            setcolor(WHITE);
        }
    }
    settextstyle(10,0,5);
}
inline int fastrand()// genereaza un nr rand
{
    static unsigned int g_seed;
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}
void punerePiesaCalculatorEasy()// functia implementeaza mutarile calculatorului pentru nivelul easy   ---alex
{
    int linia,coloana,coloanag,liniag;
    //CALCULATORUL MUTA PRIMUL

    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x=mousex()-xstart;
        y=mousey()-ystart;
        if(x+xstart>=newgame.l&&x+xstart<=newgame.r&&y+ystart>=newgame.t&&y+ystart<=newgame.b)
        {
            nrmutari=0;
            joc_de1_easy();

        }
        if(x+xstart>=backBUTON.l&&x+xstart<=backBUTON.r&&y+ystart>=backBUTON.t&&y+ystart<=backBUTON.b)//buton back in optiuni
        {
            setcolor(WHITE);
            MainMenu();
        }
        if(x+xstart>=undo.l&&x+xstart<=undo.r&&y+ystart>=undo.t&&y+ystart<=undo.b)
        {
            undo.pressed=true;
            Undo();
        }
        if(x+xstart>=music.l&&x+xstart<=music.r&&y+ystart>=music.t&&y+ystart<=music.b)   //muzica ruleaza sau nu
        {
            music.pressed=(!music.pressed);
            if(music.pressed==true&&ruleaza==false)  // muzica nu ruleaza
            {
                PlaySound(TEXT("MusicForBridgItWAV.wav"),NULL,SND_LOOP|SND_ASYNC);
                if(engleza.pressed==true)
                {
                    setcolor(RED);
                    drawB(music,"MUSIC ");
                    setcolor(WHITE);
                }
                if(romana.pressed==true)
                {
                    setcolor(RED);
                    drawB(music,"MUZICA");
                    setcolor(WHITE);
                }
                ruleaza=true;
            }
            if(music.pressed==false&&ruleaza==true)
            {
                PlaySound(NULL, 0, 0);
                if(engleza.pressed==true)
                {
                    setcolor(WHITE);
                    drawB(music,"MUSIC ");
                }
                if(romana.pressed==true)
                {
                    setcolor(WHITE);
                    drawB(music,"MUZICA");
                }
                ruleaza=false;
            }
        }
        ///spatiu pentru conditii butoane in easy
    }
    if(nrmutari%2!=0&&gata==false)
    {
        setcolor(C1);
        linia=nrlin(y);
        coloana=nrcol(x);
        coloanag=coloana+nrcol(xstart)-1;
        liniag=linia+nrlin(ystart)-1;
        if(TablaDeJoc[linia][coloana]==0&&coloanag*50>xstart&&liniag*50>ystart&&coloanag*50<=xstart+LG*50&&liniag*50<=ystart+LG*50&&gata==false)//conditii pentru a nu desena in portiunea xstart si ystart
        {
            setcolor(C1);
            setfillstyle(SOLID_FILL,C1);
            if(coloana%2!=0)
            {
                if(coloana+1<LG&&coloana-1>1)
                {
                    bar((((coloanag-1)*50) -25),(((liniag)*50)-25),(((coloanag-1)*50)+75),(((liniag)*50)-30));//orizontala
                    TablaDeJoc[linia][coloana]=3;
                    nrmutari++;

                    if(romana.pressed==true)
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,"COMPUTERUL A MUTAT");
                        setcolor(C1);
                        outtextxy(100,50,urmeaza3);
                        setcolor(WHITE);
                    }
                    else
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,"COMPUTER MOVED");
                        setcolor(C1);
                        outtextxy(100,50,urmeaza3);
                        setcolor(WHITE);
                    }
                }
            }
            else
            {
                bar((coloanag)*50-30,((liniag-1)*50)+75,(coloanag)*50-25,(liniag-1)*50-25);//verticala
                TablaDeJoc[linia][coloana]=3;
                nrmutari++;
                if(romana.pressed==true)
                {
                    setcolor(BLACK);
                    outtextxy(100,50,"COMPUTERUL A MUTAT");
                    setcolor(C1);
                    outtextxy(100,50,urmeaza3);
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(BLACK);
                    outtextxy(100,50,"COMPUTER MOVED");
                    setcolor(C1);
                    outtextxy(100,50,urmeaza3);
                    setcolor(WHITE);
                }
            }
            afisare_scor();
        }
        verificare_castigator();
    }

    else if(nrmutari%2==0&&gata==false)
    {
        while(nrmutari%2==0)
        {
            srand(time(NULL));
            coloana=fastrand()%LG;
            linia=fastrand()%LG;
            coloanag=coloana+nrcol(xstart)-1;
            liniag=linia+nrlin(ystart)-1;
            if(TablaDeJoc[linia][coloana]==0&&coloanag*50>xstart&&liniag*50>ystart&&coloanag*50<=xstart+LG*50&&liniag*50<=ystart+LG*50)//conditii pentru a nu desena in portiunea xstart si ystart
            {
                setcolor(C2);
                delay(msec);
                setfillstyle(SOLID_FILL,C2);
                if(coloana%2==0)
                {
                    bar((((coloanag-1)*50) -25),((liniag*50)-25),(((coloanag-1)*50)+75),(((liniag)*50)-30));//orizontala
                    TablaDeJoc[linia][coloana]=4;
                    nrmutari++;
                    if(romana.pressed==true)
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,urmeaza3);
                        setcolor(C2);
                        outtextxy(100,50,"COMPUTERUL A MUTAT");
                        setcolor(WHITE);
                    }
                    else
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,urmeaza3);
                        setcolor(C2);
                        outtextxy(100,50,"COMPUTER MOVED");
                        setcolor(WHITE);
                    }
                }
                else
                {
                    if((linia+1<LG&&linia-1>1))
                    {
                        bar((coloanag)*50-30,((liniag-1)*50)+75,(coloanag)*50-25,(liniag-1)*50-25);//verticala
                        TablaDeJoc[linia][coloana]=4;
                        nrmutari++;
                        if(romana.pressed==true)
                        {
                            setcolor(BLACK);
                            outtextxy(100,50,urmeaza3);
                            setcolor(C2);
                            outtextxy(100,50,"COMPUTERUL A MUTAT");
                            setcolor(WHITE);
                        }
                        else
                        {
                            setcolor(BLACK);
                            outtextxy(100,50,urmeaza3);
                            setcolor(C2);
                            outtextxy(100,50,"COMPUTER MOVED");
                            setcolor(WHITE);
                        }
                    }
                }
            }
        }
        afisare_scor();
        verificare_castigator();
    }
}
void punerePiesaCalculatorMedium() ///raluca
{
    int verticala=0,orizontala=0,c,l,linia,coloana,coloanag,liniag;
    setcolor(C2);
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x=mousex()-xstart;
        y=mousey()-ystart;
        if(mousex()>=newgame.l&&mousex()<=newgame.r&&mousey()>=newgame.t&&mousey()<=newgame.b)
        {
            nrmutari=0;
            joc_de1_medium();
        }
        if(x+xstart>=backBUTON.l&&x+xstart<=backBUTON.r&&y+ystart>=backBUTON.t&&y+ystart<=backBUTON.b)//buton back in optiuni
        {
            setcolor(WHITE);
            MainMenu();
        }
        if(x+xstart>=undo.l&&x+xstart<=undo.r&&y+ystart>=undo.t&&y+ystart<=undo.b)
        {
            undo.pressed=true;
            Undo();
        }
        if(x+xstart>=music.l&&x+xstart<=music.r&&y+ystart>=music.t&&y+ystart<=music.b)   //muzica ruleaza sau nu
        {
            music.pressed=(!music.pressed);
            if(music.pressed==true&&ruleaza==false)  // muzica nu ruleaza
            {
                PlaySound(TEXT("MusicForBridgItWAV.wav"),NULL,SND_LOOP|SND_ASYNC);
                if(engleza.pressed==true)
                {
                    setcolor(RED);
                    drawB(music,"MUSIC ");
                    setcolor(WHITE);
                }
                if(romana.pressed==true)
                {
                    setcolor(RED);
                    drawB(music,"MUZICA");
                    setcolor(WHITE);
                }
                ruleaza=true;
            }
            if(music.pressed==false&&ruleaza==true)
            {
                PlaySound(NULL, 0, 0);
                if(engleza.pressed==true)
                {
                    setcolor(WHITE);
                    drawB(music,"MUSIC ");
                }
                if(romana.pressed==true)
                {
                    setcolor(WHITE);
                    drawB(music,"MUZICA");
                }
                ruleaza=false;
            }
        }
///spatiu pentru conditii pt butoane medium

    }
    if(nrmutari%2!=0&&gata==false)
    {
        setcolor(C1);
        linia=nrlin(y);
        coloana=nrcol(x);
        coloanag=coloana+nrcol(xstart)-1;
        liniag=linia+nrlin(ystart)-1;
        if(TablaDeJoc[linia][coloana]==0&&coloanag*50>xstart&&liniag*50>ystart&&coloanag*50<=xstart+LG*50&&liniag*50<=ystart+LG*50)//conditii pentru a nu desena in portiunea xstart si ystart
        {
            setcolor(C1);
            setfillstyle(SOLID_FILL,C1);
            if(coloana%2!=0)
            {
                if(coloana+1<LG&&coloana-1>1)
                {
                    bar((((coloanag-1)*50) -25),(((liniag)*50)-25),(((coloanag-1)*50)+75),(((liniag)*50)-30));//orizontala
                    TablaDeJoc[linia][coloana]=3;
                    nrmutari++;
                    orizontala=1;
                    if(romana.pressed==true)
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,"COMPUTERUL A MUTAT");
                        setcolor(C1);
                        outtextxy(100,50,urmeaza3);
                        setcolor(WHITE);
                    }
                    else
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,"COMPUTER MOVED");
                        setcolor(C1);
                        outtextxy(100,50,urmeaza3);
                        setcolor(WHITE);
                    }
                }
            }
            else
            {
                bar((coloanag)*50-30,((liniag-1)*50)+75,(coloanag)*50-25,(liniag-1)*50-25);//verticala
                TablaDeJoc[linia][coloana]=3;
                nrmutari++;
                verticala=1;
                if(romana.pressed==true)
                {
                    setcolor(BLACK);
                    outtextxy(100,50,"COMPUTERUL A MUTAT");
                    setcolor(C1);
                    outtextxy(100,50,urmeaza3);
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(BLACK);
                    outtextxy(100,50,"COMPUTER MOVED");
                    setcolor(C1);
                    outtextxy(100,50,urmeaza3);
                    setcolor(WHITE);
                }
            }
        }
        afisare_scor();
        verificare_castigator();
    }
    delay(1000);
    if(nrmutari%2==0&&gata==false)
    {
        delay(msec);
        c=coloana;
        l=linia;
        if(verticala==1)
        {
            if(linia==LG-1&&TablaDeJoc[linia-2][coloana]==0)
                linia=linia-2;
            else if(linia==2&&TablaDeJoc[linia+2][coloana]==0)
                linia=linia+2;
            else if(TablaDeJoc[linia-2][coloana]==0)
                linia=linia-2;
            else if(TablaDeJoc[linia+2][coloana]==0)
                linia=linia+2;
        }
        if(orizontala==1&&linia!=1&&linia!=LG)
        {
            if(TablaDeJoc[linia][coloana+2]==0)
                coloana=coloana+2;
            else if(TablaDeJoc[linia][coloana-2]==0)
                coloana=coloana-2;
        }
    }
    if((c==coloana&&linia==l)||nrmutari==0)
    {
        while(nrmutari%2==0)
        {
            srand(time(NULL));
            coloana=fastrand()%LG;
            linia=fastrand()%LG;
            coloanag=coloana+nrcol(xstart)-1;
            liniag=linia+nrlin(ystart)-1;
            if(TablaDeJoc[linia][coloana]==0&&coloanag*50>xstart&&liniag*50>ystart&&coloanag*50<=xstart+LG*50&&liniag*50<=ystart+LG*50)//conditii pentru a nu desena in portiunea xstart si ystart
            {
                setcolor(C2);
                setfillstyle(SOLID_FILL,C2);
                if(coloana%2==0)
                {
                    bar((((coloanag-1)*50) -25),((liniag*50)-25),(((coloanag-1)*50)+75),(((liniag)*50)-30));//orizontala
                    TablaDeJoc[linia][coloana]=4;
                    nrmutari++;
                    if(romana.pressed==true)
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,urmeaza3);
                        setcolor(C2);
                        outtextxy(100,50,"COMPUTERUL A MUTAT");
                        setcolor(WHITE);
                    }
                    else
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,urmeaza3);
                        setcolor(C2);
                        outtextxy(100,50,"COMPUTER MOVED");
                        setcolor(WHITE);
                    }

                }
                else if((linia+1<LG&&linia-1>1))
                {
                    bar((coloanag)*50-30,((liniag-1)*50)+75,(coloanag)*50-25,(liniag-1)*50-25);//verticala
                    TablaDeJoc[linia][coloana]=4;
                    nrmutari++;
                    if(romana.pressed==true)
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,urmeaza3);
                        setcolor(C2);
                        outtextxy(100,50,"COMPUTERUL A MUTAT");
                        setcolor(WHITE);
                    }
                    else
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,urmeaza3);
                        setcolor(C2);
                        outtextxy(100,50,"COMPUTER MOVED");
                        setcolor(WHITE);
                    }
                }
                afisare_scor();
                verificare_castigator();
            }
        }
    }
    else
    {
        coloanag=coloana+nrcol(xstart)-1;
        liniag=linia+nrlin(ystart)-1;
        if(TablaDeJoc[linia][coloana]==0&&coloanag*50>xstart&&liniag*50>ystart&&coloanag*50<=xstart+LG*50&&liniag*50<=ystart+LG*50)//conditii pentru a nu desena in portiunea xstart si ystart
        {
            setcolor(C2);
            setfillstyle(SOLID_FILL,C2);
            if(coloana%2==0)
            {
                bar((((coloanag-1)*50) -25),((liniag*50)-25),(((coloanag-1)*50)+75),(((liniag)*50)-30));//orizontala
                TablaDeJoc[linia][coloana]=4;
                nrmutari++;
                if(romana.pressed==true)
                {
                    setcolor(BLACK);
                    outtextxy(100,50,urmeaza3);
                    setcolor(C2);
                    outtextxy(100,50,"COMPUTERUL A MUTAT");
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(BLACK);
                    outtextxy(100,50,urmeaza3);
                    setcolor(C2);
                    outtextxy(100,50,"COMPUTER MOVED");
                    setcolor(WHITE);
                }
            }
            else if((linia+1<LG&&linia-1>1))
            {
                bar((coloanag)*50-30,((liniag-1)*50)+75,(coloanag)*50-25,(liniag-1)*50-25);//verticala
                TablaDeJoc[linia][coloana]=4;
                nrmutari++;
                if(romana.pressed==true)
                {
                    setcolor(BLACK);
                    outtextxy(100,50,urmeaza3);
                    setcolor(C2);
                    outtextxy(100,50,"COMPUTERUL A MUTAT");
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(BLACK);
                    outtextxy(100,50,urmeaza3);
                    setcolor(C2);
                    outtextxy(100,50,"COMPUTER MOVED");
                    setcolor(WHITE);
                }
            }
            verificare_castigator();
        }
    }
}
void punerePiesaCalculatorHard() ///toti
{
    int linia,coloana,liniag,coloanag,orizontala=0,verticala=0;
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x=mousex()-xstart;
        y=mousey()-ystart;
        if(mousex()>=newgame.l&&mousex()<=newgame.r&&mousey()>=newgame.t&&mousey()<=newgame.b)
        {
            nrmutari=0;
            joc_de1_hard();
        }
        if(x+xstart>=backBUTON.l&&x+xstart<=backBUTON.r&&y+ystart>=backBUTON.t&&y+ystart<=backBUTON.b)//buton back in optiuni
        {
            setcolor(WHITE);
            MainMenu();
        }
        if(x+xstart>=undo.l&&x+xstart<=undo.r&&y+ystart>=undo.t&&y+ystart<=undo.b)
        {
            undo.pressed=true;
            Undo();
        }
        if(x+xstart>=music.l&&x+xstart<=music.r&&y+ystart>=music.t&&y+ystart<=music.b)   //muzica ruleaza sau nu
        {
            music.pressed=(!music.pressed);
            if(music.pressed==true&&ruleaza==false)  // muzica nu ruleaza
            {
                PlaySound(TEXT("MusicForBridgItWAV.wav"),NULL,SND_LOOP|SND_ASYNC);
                if(engleza.pressed==true)
                {
                    setcolor(RED);
                    drawB(music,"MUSIC ");
                    setcolor(WHITE);
                }
                if(romana.pressed==true)
                {
                    setcolor(RED);
                    drawB(music,"MUZICA");
                    setcolor(WHITE);
                }
                ruleaza=true;
            }
            if(music.pressed==false&&ruleaza==true)
            {
                PlaySound(NULL, 0, 0);

                if(engleza.pressed==true)
                {
                    setcolor(WHITE);
                    drawB(music,"MUSIC ");
                }
                if(romana.pressed==true)
                {
                    setcolor(WHITE);
                    drawB(music,"MUZICA");
                }
                ruleaza=false;
            }
        }
        ///spatiu pentru conditii butoane pt hard

    }
    if(nrmutari==0&&gata==false)
    {
        linia=LG-1;
        coloana=2;
        coloanag=coloana+nrcol(xstart)-1;
        liniag=linia+nrlin(ystart)-1;
        setcolor(C1);
        bar((coloanag)*50-30,((liniag-1)*50)+75,(coloanag)*50-25,(liniag-1)*50-25);//verticala
        nrmutari++;
        setcolor(C1);
        TablaDeJoc[linia][coloana]=3;
        if(romana.pressed==true)
        {
            setcolor(BLACK);
            outtextxy(100,50,urmeaza3);
            setcolor(C1);
            outtextxy(100,50,"COMPUTERUL A MUTAT");
            setcolor(WHITE);
        }
        else
        {
            setcolor(BLACK);
            outtextxy(100,50,urmeaza3);
            setcolor(C1);
            outtextxy(100,50,"COMPUTER MOVED");
            setcolor(WHITE);
        }
        afisare_scor();
    }
    if(nrmutari%2!=0&&gata==false)
    {
        linia=nrlin(y);
        coloana=nrcol(x);
        coloanag=coloana+nrcol(xstart)-1;
        liniag=linia+nrlin(ystart)-1;
        if(TablaDeJoc[linia][coloana]==0&&coloanag*50>xstart&&liniag*50>ystart&&coloanag*50<=xstart+LG*50&&liniag*50<=ystart+LG*50)//conditii pentru a nu desena in portiunea xstart si ystart
        {
            setcolor(C2);
            setfillstyle(SOLID_FILL,C2);

            if(romana.pressed==true)
            {
                setcolor(BLACK);
                outtextxy(100,50,"COMPUTERUL A MUTAT");
                setcolor(C1);
                outtextxy(100,50,urmeaza3);
                setcolor(WHITE);
            }
            else
            {
                setcolor(BLACK);
                outtextxy(100,50,"COMPUTER MOVED");
                setcolor(C2);
                outtextxy(100,50,urmeaza3);
                setcolor(WHITE);
            }
            delay(600);

            if(coloana%2==0)
            {
                bar((((coloanag-1)*50) -25),((liniag*50)-25),(((coloanag-1)*50)+75),(((liniag)*50)-30));//orizontala
                TablaDeJoc[linia][coloana]=4;
                nrmutari++;
                orizontala=1;
                if(romana.pressed==true)
                {
                    setcolor(BLACK);
                    outtextxy(100,50,"COMPUTERUL A MUTAT");
                    setcolor(C2);
                    outtextxy(100,50,urmeaza3);
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(BLACK);
                    outtextxy(100,50,"COMPUTER MOVED");
                    setcolor(C1);
                    outtextxy(100,50,urmeaza3);
                    setcolor(WHITE);
                }
            }
            else if(linia+1<LG&&linia-1>1)
            {
                bar((coloanag)*50-30,((liniag-1)*50)+75,(coloanag)*50-25,(liniag-1)*50-25);//verticala
                TablaDeJoc[linia][coloana]=4;
                nrmutari++;
                verticala=1;
                if(romana.pressed==true)
                {
                    setcolor(BLACK);
                    outtextxy(100,50,"COMPUTERUL A MUTAT");
                    setcolor(C2);
                    outtextxy(100,50,urmeaza3);
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(BLACK);
                    outtextxy(100,50,"COMPUTER MOVED");
                    setcolor(C2);
                    outtextxy(100,50,urmeaza3);
                    setcolor(WHITE);
                }
            }
        }
        afisare_scor();
        verificare_castigator();
    }
    if(nrmutari%2==0&&gata==false)
    {
        delay(msec);
        if(exceptie(linia,coloana)==0)
        {
            if(coloana==1||coloana==LG)
                swap(linia,coloana);
            else if(linia+coloana<LG)
            {
                if(orizontala==1)
                {
                    linia=linia+1;
                    coloana=coloana+1;
                }
                if(verticala==1)
                {
                    linia=linia-1;
                    coloana=coloana-1;
                }
            }
            else if(linia+coloana>LG)
            {
                if(verticala==1)
                {
                    linia=linia+1;
                    coloana=coloana+1;
                }
                if(orizontala==1)
                {
                    linia=linia-1;
                    coloana=coloana-1;
                }
            }
        }
        else if(exceptie(linia,coloana)==1)
            coloana=coloana+2;
        else if(exceptie(linia,coloana)==2)
            coloana=coloana-2;
        coloanag=coloana+nrcol(xstart)-1;
        liniag=linia+nrlin(ystart)-1;
        if(TablaDeJoc[linia][coloana]==0&&coloanag*50>xstart&&liniag*50>ystart&&coloanag*50<=xstart+LG*50&&liniag*50<=ystart+LG*50)//conditii pentru a nu desena in portiunea xstart si ystart
        {
            setcolor(C1);
            setfillstyle(SOLID_FILL,C1);
            if(coloana%2!=0)
            {
                if(coloana+1<LG&&coloana-1>1)
                {
                    bar((((coloanag-1)*50) -25),(((liniag)*50)-25),(((coloanag-1)*50)+75),(((liniag)*50)-30));//orizontala
                    TablaDeJoc[linia][coloana]=3;
                    nrmutari++;
                    if(romana.pressed==true)
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,urmeaza3);
                        setcolor(C1);
                        outtextxy(100,50,"COMPUTERUL A MUTAT");
                        setcolor(WHITE);
                    }
                    else
                    {
                        setcolor(BLACK);
                        outtextxy(100,50,urmeaza3);
                        setcolor(C1);
                        outtextxy(100,50,"COMPUTER MOVED");
                        setcolor(WHITE);
                    }
                }
            }
            else
            {
                bar((coloanag)*50-30,((liniag-1)*50)+75,(coloanag)*50-25,(liniag-1)*50-25);//verticala
                TablaDeJoc[linia][coloana]=3;
                nrmutari++;
                if(romana.pressed==true)
                {
                    setcolor(BLACK);
                    outtextxy(100,50,urmeaza3);
                    setcolor(C1);
                    outtextxy(100,50,"COMPUTERUL A MUTAT");
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(BLACK);
                    outtextxy(100,50,urmeaza3);
                    setcolor(C1);
                    outtextxy(100,50,"COMPUTER MOVED");
                    setcolor(WHITE);
                }
            }
        }
        afisare_scor();
        verificare_castigator();
    }
}
void joc_de2()  //emilia+raluca
{
    cleardevice();
    desenareTablaDeJoc();
    gata=false;
    bool ok=false;
    do
    {
        //timer
        if(gata==false)
        {
            if(nrmutari==0)
            {
                seconds=0;
                minutes=0;
            }
            setcolor(GREEN);
            settextstyle(10,0,5);
            outtextxy(690,50,itoa(minutes,minutesTimer,10));// int to char
            outtextxy(740,50,":");
            setcolor(GREEN);
            outtextxy(790,50,itoa(seconds,nrtimer,10));
            if(seconds>59)
            {
                seconds=0;
                minutes++;
                setcolor(BLACK);
                outtextxy(790,50,"    ");
            }
            else
                Sleep(1000);
            seconds++;
            setcolor(BLACK);
            //nr mutare
            setcolor(WHITE);
            settextstyle(10,0,4);
            if(romana.pressed==true)
            {
                outtextxy(20,200,"Numarul de mutari:");
                setcolor(C1);
                outtextxy(20,250,"Numarul de mutari 1:");
                setcolor(C2);
                outtextxy(20,300,"Numarul de mutari 2:");
                setcolor(WHITE);
            }
            else
            {
                outtextxy(20,200, "Number of moves:");
                setcolor(C1);
                outtextxy(20,250, "Number of moves 1:");
                setcolor(C2);
                outtextxy(20,300, "Number of moves 2:");
                setcolor(WHITE);
            }
            if(nrmutari==0)
            {
                outtextxy(400,200, itoa(0,nrMUTARI,10));
                outtextxy(400,250, itoa(0,nrMUTARI,10));
                outtextxy(400,300, itoa(0,nrMUTARI,10));

            }
            else
            {
                setcolor(WHITE);
                outtextxy(400,200, itoa(nrmutari,nrMUTARI,10));
                if(nrmutari%2==0)
                {
                    setcolor(C1);
                    outtextxy(400,250, itoa(nrmutari/2,nrMUTARI,10));
                    setcolor(C2);
                    outtextxy(400,300, itoa(nrmutari/2,nrMUTARI,10));
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(C1);
                    outtextxy(400,250, itoa(nrmutari/2,nrMUTARI,10));
                    setcolor(C2);
                    outtextxy(400,300, itoa(nrmutari/2 +1,nrMUTARI,10));
                    setcolor(WHITE);
                }
            }
        }
        punerePiesa();
    }
    while (!ok);
    setcolor(WHITE);
}

void joc_de1_easy()///alex
{
    cleardevice();
    desenareTablaDeJoc();
    gata=false;
    bool ok=false;
    do
    {
        //timer
        if(gata==false)
        {
            if(nrmutari==0)
            {
                seconds=0;
                minutes=0;
            }
            setcolor(GREEN);
            settextstyle(10,0,5);
            outtextxy(690,50,itoa(minutes,minutesTimer,10));// int to char
            outtextxy(740,50,":");
            setcolor(GREEN);
            outtextxy(790,50,itoa(seconds,nrtimer,10));
            if(seconds>59)
            {
                seconds=0;
                minutes++;
                setcolor(BLACK);
                outtextxy(790,50,"    ");
            }
            else
                Sleep(1000);
            seconds++;
            setcolor(BLACK);
            //nr mutare
            setcolor(WHITE);
            settextstyle(10,0,4);
        }
        punerePiesaCalculatorEasy();
    }
    while (!ok);
    setcolor(WHITE);
}
void joc_de1_medium() ///raluca
{
    setcolor(WHITE);
    cleardevice();
    desenareTablaDeJoc();
    gata=false;
    bool ok=false;
    do
    {
        //timer
        if(gata==false)
        {
            if(nrmutari==0)
            {
                seconds=0;
                minutes=0;
            }
            setcolor(GREEN);
            settextstyle(10,0,5);
            outtextxy(690,50,itoa(minutes,minutesTimer,10));// int to char
            outtextxy(740,50,":");
            setcolor(GREEN);
            outtextxy(790,50,itoa(seconds,nrtimer,10));
            if(seconds>59)
            {
                seconds=0;
                minutes++;
                setcolor(BLACK);
                outtextxy(790,50,"    ");
            }
            else
                Sleep(1000);
            seconds++;
            setcolor(BLACK);

        }
        punerePiesaCalculatorMedium();
    }
    while (!ok);
    setcolor(WHITE);

}
void joc_de1_hard() ///toti
{
    cleardevice();
    desenareTablaDeJoc();
    gata=false;
    bool ok=false;
    do
    {
        //timer
        if(gata==false)
        {
            if(nrmutari==0)
            {
                seconds=0;
                minutes=0;
            }
            setcolor(GREEN);
            settextstyle(10,0,5);
            outtextxy(690,50,itoa(minutes,minutesTimer,10));// int to char
            outtextxy(740,50,":");
            setcolor(GREEN);
            outtextxy(790,50,itoa(seconds,nrtimer,10));
            if(seconds>59)
            {
                seconds=0;
                minutes++;
                setcolor(BLACK);
                outtextxy(790,50,"    ");
            }
            else
                Sleep(1000);
            seconds++;
            setcolor(BLACK);
        }
        punerePiesaCalculatorHard();
    }
    while (!ok);
    setcolor(WHITE);

}
void citesteSir(char text[MAX], char s[MAX], int x, int y, bool stergere)
{
    strcpy(s,"");
    char s1[MAX];
    char t[2];
    char tasta;
    char text2[MAX];
    t[0]=tasta;
    t[1]='\0';
    strcat(s,t);
    strcpy(s1,s);
    strcat(s1,"_");
    setcolor(WHITE);
    strcpy(text2,text);
    strcat(text2,s1);
    outtextxy(x,y,text2);
    do
    {
        tasta=getch();
        if (tasta==8) // backspace
            if (strlen(s)>0)
            {
                setcolor(BLACK);
                strcpy(s1,s);
                strcat(s1,"_");
                strcpy(text2,text);
                strcat(text2,s1);
                outtextxy(x,y,text2);
                s[strlen(s)-1]='\0';
                strcpy(s1,s);
                strcat(s1,"_");
                strcpy(text2,text);
                strcat(text2,s1);
                outtextxy(x,y,text2);
                setcolor(WHITE);
                strcpy(text2,text);
                strcat(text2,s1);
                outtextxy(x,y,text2);
            }
            else
                Beep(1000,100);
        else
        {
            t[0]=tasta;
            t[1]='\0';
            strcat(s,t);
            strcpy(s1,s);
            strcat(s1,"_");
            setcolor(WHITE);
            strcpy(text2,text);
            strcat(text2,s1);
            outtextxy(x,y,text2);
        }
    }
    while (tasta!=13);
    t[0]=tasta;
    t[1]='\0';
    strcat(s,t);
    strcpy(s1,s);
    strcat(s1,"_");
    strcpy(text2,text);
    strcat(text2,s1);
    setcolor(BLACK);
    outtextxy(x,y,text2);
    if (!stergere)
    {
        strcpy(text2,text);
        strcat(text2,s);
        setcolor(WHITE);
        outtextxy(x,y,text2);
    }
}
void nume_2persoane()   ///emilia
{
    cleardevice();
    if(engleza.pressed==true)
    {
        //SetTextAlign(HDC,uint16_t);
        outtextxy(350,200,"CHOOSE THE NAMES OF THE PLAYERS");
        setcolor(C1);
        outtextxy(200,400,"PLAYER 1 NAME:");
        setcolor(C2);
        outtextxy(900,400,"PLAYER 2 NAME:");
    }
    if(romana.pressed==true)
    {
        outtextxy(350,200,"ALEGE NUMELE JUCATORILOR");
        setcolor(C1);
        outtextxy(200,400,"NUME JUCATOR 1:");
        setcolor(C2);
        outtextxy(900,400,"NUME JUCATOR 2:");

    }
    char a[MAX]= {0};
    char b[MAX]= {0};
    citesteSir(a,s1,240,450,false);
    citesteSir(b,s2,940,450,false);
    nume_joc_de2=true;
}
void nume_1pers() ///emilia
{
    cleardevice();
    if(engleza.pressed==true)
    {
        outtextxy(450,200,"CHOOSE THE NAME OF THE PLAYER");
        if(hard.pressed==true)
            setcolor(C2);
        else
            setcolor(C1);
        outtextxy(700,400,"PLAYER NAME:");
    }
    if(romana.pressed==true)
    {
        outtextxy(450,200,"ALEGE NUMELE JUCATORULUI");
        setcolor(C1);
        outtextxy(700,400,"NUME JUCATOR:");

    }
    setcolor(WHITE);
    char a[MAX]= {0};
    citesteSir(a,s3,740,450,false);
    nume_joc_de1=true;
}
void mesaj_verificare()
{
    setcolor(RED);
    settextstyle(3,0,5);
    outtextxy(500,300,"Go To Options / Alege Setarile");///punere mesaj de afisare
    delay(1000);
    setcolor(WHITE);

}
void ScoreF() //ALEX
{
    FILE *f1,*f2;
    char aux[50];
    bool apasat=false;
    int x,y,randnou=0;
    cleardevice();
    if(scor1==0&&scor2==0)
    {

        f2=fopen("Clasament.txt","r");
        f1=fopen("ClasamentCurent.txt","r");
        if(fgetc(f2)==EOF&&fgetc(f1)==EOF)
        {
            setcolor(RED);
            settextstyle(3,0,5);
            outtextxy(500,300,"NO GAME PLAYED YET");
            delay(2000);
            setcolor(BLACK);
            outtextxy(500,300,"NO GAME PLAYED YET");
            setcolor(WHITE);
            fclose(f2);
        }
        else
        {
            if(fgetc(f1)!=EOF)
            {
                fgets(aux, sizeof(aux),f1);
                fclose(f1);
                f1=fopen("ClasamentCurent.txt","w");
                fprintf(f1,"");
                fclose(f1);
                f2=fopen("Clasament.txt","a+");
                fprintf(f2,"\n");
                fprintf(f2,aux);
                fclose(f2);
            }
            f2=fopen("Clasament.txt","r");
            while(!feof(f2))
            {
                fgets(linieclasament, sizeof(linieclasament),f2);
                settextstyle(10,0,3);
                outtextxy(600,randnou*30,linieclasament);
                randnou++;
            }
            fclose(f2);
        }
    }
    if(scor1!=0||scor2!=0)
    {

        f2=fopen("Clasament.txt","a+");
        f1=fopen("ClasamentCurent.txt","r");

        if(fgetc(f1)!=EOF)
        {
            fgets(aux, sizeof(aux),f1);
            fclose(f1);
            f1=fopen("ClasamentCurent.txt","w");
            fprintf(f1,"");
            fclose(f1);
            fprintf(f2,"\n");
            fprintf(f2,aux);
            fclose(f2);
        }
        f2=fopen("Clasament.txt","r");
        while(!feof(f2))
        {
            fgets(linieclasament, sizeof(linieclasament),f2);
            settextstyle(10,0,3);
            outtextxy(600,randnou*30,linieclasament);
            randnou++;
        }
        fclose(f2);
        scor1=scor2=0;
    }

    initializare_buton(backBUTON,20,670,15,10,170,60,10,5);
    initializare_buton(stergeB,20,735,5,5,170,60,10,5);
    if(romana.pressed==true)
    {
        drawB(stergeB,"STERGE");
        drawB(backBUTON, "INAPOI");
    }
    else
    {
        drawB(stergeB,"Erase");
        drawB(backBUTON, "BACK");
    }
    while(!apasat)
    {
        clearmouseclick(WM_LBUTTONDOWN);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            x=mousex();
            y=mousey();
            if(x>=backBUTON.l&&x<=backBUTON.r&&y>=backBUTON.t&&y<=backBUTON.b)
            {
                apasat=true;
                setcolor(WHITE);
                MainMenu();
            }
            if(x>=stergeB.l&&x<=stergeB.r&&y>=stergeB.t&&y<=stergeB.b)
            {

                f1=fopen("Clasament.txt","w");
                fprintf(f1,"");
                fclose(f1);
                f2=fopen("ClasamentCurent.txt","w");
                fprintf(f2,"");
                fclose(f2);
                f1=fopen("Clasament.txt","r");
                ScoreF();
            }
        }
    }


}
void MainMenu() //ALEX
{
    FILE *f1,*f2;
    char aux[100];
    nr_meniu++;
    if(music.pressed==true)
        PlaySound(TEXT("MusicForBridgItWAV.wav"),NULL,SND_FILENAME|SND_ASYNC);
    int a,b;
    bool apasat=false;
    cleardevice();
    readimagefile("FUNDALBRIDGIT4.jpg",0,0,1600,800);
    initializare_buton(hints,400,600,35,30,200,100,10,5);
    initializare_buton(start, 700,600,35,30,200,100,10,5);
    initializare_buton(options,1000,600,13,30,200,100,10,5);
    initializare_buton(score, 700,720,30,15,200,70,10,5);
    if(engleza.pressed==true||nr_optiuni==0)
    {
        drawB(hints, "HINTS");
        drawB(start, "PLAY");
        drawB(options, "OPTIONS");
        drawB(score, "SCORE");
    }
    if(romana.pressed==true)
    {
        drawB(hints, "REGULI");
        drawB(start, "START");
        drawB(options, "SETARI");
        drawB(score, "SCOR");
    }
    while(!apasat)
    {
        clearmouseclick(WM_LBUTTONDOWN);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            a=mousex();
            b=mousey();
            if(a>=start.l&&a<=start.r&&b>=start.t&&b<=start.b)
            {
                nume_joc_de1=false;
                nume_joc_de2=false;
                strcpy(urmeaza1,"");
                strcpy(urmeaza2,"");
                strcpy(urmeaza3,"");
                scor1=scor2=0;
                f1=fopen("ClasamentCurent.txt","r");
                f2=fopen("Clasament.txt","a+");
                if(fgetc(f1)!=EOF)
                {
                    fgets(aux, sizeof(aux),f1);
                    fclose(f1);
                    f1=fopen("ClasamentCurent.txt","w");
                    fprintf(f1,"");
                    fclose(f1);
                    fprintf(f2,"\n");
                    fprintf(f2,aux);
                    fclose(f2);
                }
                if(nr_optiuni==0||(singlePlayer.pressed==false&&multiPlayer.pressed==false)||(easy.pressed==false&&medium.pressed==false&&hard.pressed==false)||C1==C2)
                    mesaj_verificare();
                else
                {
                    nrmutari=0;
                    if(multiPlayer.pressed==true)
                    {
                        if(nume_joc_de2==false)
                            nume_2persoane();
                        strcpy(urmeaza1,s1);
                        strcpy(urmeaza2,s2);
                        if(romana.pressed==true)
                        {
                            strcat(urmeaza1," URMEAZA");
                            strcat(urmeaza2," URMEAZA");
                        }
                        else
                        {
                            strcat(urmeaza1," IS NEXT");
                            strcat(urmeaza2," IS NEXT");
                        }
                        joc_de2();
                    }
                    if(singlePlayer.pressed==true)
                    {
                        if(nume_joc_de1==false)
                            nume_1pers();
                        strcpy(urmeaza3,s3);
                        if(romana.pressed==true)
                            strcat(urmeaza3," A MUTAT");
                        else
                            strcat(urmeaza3," MOVED");
                        if(easy.pressed==true)
                            joc_de1_easy();
                        if(medium.pressed==true)
                            joc_de1_medium();
                        if(hard.pressed==true)
                            joc_de1_hard();
                    }
                }
            }
            if(a>=options.l&&a<=options.r&&b>=options.t&&b<=options.b)
            {
                clearviewport();
                nr_optiuni++;
                Option();
            }
            if(a>=hints.l&&a<=hints.r&&b>=hints.t&&b<=hints.b)
                reguli_de_joc();
            if(a>=score.l&&a<=score.r&&b>=score.t&&b<=score.b)
                ScoreF();
        }
    }
}
void reguli_de_joc() ///emilia
{
    int x,y;
    bool apasat=false;
    cleardevice();
    //citire din fisier a regulilor
    if(romana.pressed==true)
    {
        FILE *fisier;
        int randnou=0;
        fisier=fopen("reguliDeJocRomana.txt","r");
        while(!feof(fisier))
        {
            fgets(regulitext, sizeof(regulitext),fisier);
            settextstyle(10,0,3);
            outtextxy(200,randnou*30,regulitext);
            randnou++;
        }
        readimagefile("regulidejocpuncte.jpg",600,470,820,660); // x y x2 y2
        fclose(fisier);
    }
    else
    {
        FILE *fisier;
        int randnou=0;
        fisier=fopen("reguliDeJocEngleza.txt","r");
        while(!feof(fisier))
        {
            fgets(regulitext, sizeof(regulitext),fisier);
            settextstyle(10,0,3);
            outtextxy(200,randnou*30,regulitext);
            randnou++;
        }
        readimagefile("regulidejocpuncte.jpg",600,470,820,660); //left,top,right,bottom   AJUTOR
        fclose(fisier);
    }
    initializare_buton(backBUTON,20,670,35,30,200,100,10,5);
    if(romana.pressed==true)
        drawB(backBUTON, "INAPOI");
    else
        drawB(backBUTON, "BACK");
    while(!apasat)
    {
        clearmouseclick(WM_LBUTTONDOWN);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            x=mousex();
            y=mousey();
            if(x>=backBUTON.l&&x<=backBUTON.r&&y>=backBUTON.t&&y<=backBUTON.b)
            {
                apasat=true;
                setcolor(WHITE);
                MainMenu();
            }
        }
    }
}
void Option() /// toti
{
    int x,y;
    bool apasat=false;
    if(nr_optiuni==1)//verifica daca optiunile sunt apelate pentru prima data
    {

        //in caz afirmativ ele trebuie setate, fiind initilizate cu false
        settextstyle(10,0,4);
        setcolor(culoareBifaE);
        outtextxy(100,150,"LANGUAGE");
        initializare_buton(engleza,600,150,25,15,100,50,10,3);
        setcolor(culoare_subBifa);
        drawB(engleza, "ENG");
        setcolor(WHITE);
        engleza.pressed=true;
        initializare_buton(romana,850,150,30,15,100,50,10,3);
        drawB(romana, "RO");

        //tipul jocului
        settextstyle(10,0,4);
        outtextxy(100,300,"TYPE OF GAME");
        initializare_buton(singlePlayer,600,300,10,13,200,50,10,3);
        drawB(singlePlayer,"SINGLE PLAYER");
        initializare_buton(multiPlayer,850,300,25,13,200,50,10,3);
        drawB(multiPlayer,"MULTIPLAYER");

        //selectare nivel
        settextstyle(10,0,4);
        outtextxy(100,425,"LEVEL");
        initializare_buton(easy,600,425,13,10,100,50,10,4);
        initializare_buton(medium,850,425,13,10,150,50,10,4);
        initializare_buton(hard,1140,425,13,10,100,50,10,4);
        drawB(easy,"EASY");
        drawB(medium,"MEDIUM");
        drawB(hard,"HARD");

        //buton inapoi
        initializare_buton(backBUTON,20,690,35,30,200,100,10,5);
        drawB(backBUTON, "back");

        //culori
        settextstyle(10,0,4);
        outtextxy(100,550,"COLOUR OF PLAYER");
        setfillstyle(SOLID_FILL,4);//red
        fillellipse(600,570,30,30);

        setfillstyle(SOLID_FILL,1);//purple
        fillellipse(700,570,30,30);

        setfillstyle(SOLID_FILL,2); //green
        fillellipse(800,570,30,30);

        setfillstyle(SOLID_FILL,5); //blue
        fillellipse(1000,570,30,30);

        setfillstyle(SOLID_FILL,14); //yellow
        fillellipse(1100,570,30,30);

        setfillstyle(SOLID_FILL,15);//white
        fillellipse(1200,570,30,30);

        settextstyle(GOTHIC_FONT,HORIZ_DIR,4);
        outtextxy(590,610,"PLAYER 1");
        outtextxy(990,610,"PLAYER 2");
    }
    else             // e cazul in care sunt pastrate setarile anterioare
    {
        cleardevice();
        settextstyle(10,0,4);
        if(engleza.pressed==true)
        {

            //limba pentru modificare setari in engleza
            setcolor(culoareBifaE);
            outtextxy(100,150,"LANGUAGE");
            setcolor(culoare_subBifa);
            drawB(engleza, "ENG");
            setcolor(WHITE);
            drawB(romana, "RO");

            //tipul jocului pentru modificare setari in engleza
            settextstyle(10,0,4);
            if(singlePlayer.pressed==false&&multiPlayer.pressed==false)
                setcolor(WHITE);
            else
                setcolor(culoareBifaE);
            outtextxy(100,300,"TYPE OF GAME");
            if(singlePlayer.pressed==true)
            {
                setcolor(culoare_subBifa);
                drawB(singlePlayer,"SINGLE PLAYER");
                setcolor(WHITE);
                drawB(multiPlayer,"MULTIPLAYER");
            }
            if(multiPlayer.pressed==true)
            {
                setcolor(culoare_subBifa);
                drawB(multiPlayer,"MULTIPLAYER");
                setcolor(WHITE);
                drawB(singlePlayer,"SINGLE PLAYER");
            }
            if(singlePlayer.pressed==false&&multiPlayer.pressed==false)
            {
                setcolor(WHITE);
                drawB(singlePlayer,"SINGLE PLAYER");
                drawB(multiPlayer,"MULTIPLAYER");
            }
            //selectare nivel pentru modificare setari in engleza
            settextstyle(10,0,4);
            if(easy.pressed==false&&medium.pressed==false&&hard.pressed==false)
                setcolor(WHITE);
            else
                setcolor(culoareBifaE);
            outtextxy(100,425,"LEVEL");
            if(easy.pressed==false&&medium.pressed==false&&hard.pressed==false)
            {
                setcolor(WHITE);
                drawB(easy,"EASY");
                drawB(medium,"MEDIUM");
                drawB(hard,"HARD");
            }
            if(easy.pressed==true)
            {
                setcolor(culoare_subBifa);
                drawB(easy,"EASY");
                setcolor(WHITE);
                drawB(medium,"MEDIUM");
                drawB(hard,"HARD");
            }
            if(medium.pressed==true)
            {
                setcolor(culoare_subBifa);
                drawB(medium,"MEDIUM");
                setcolor(WHITE);
                drawB(easy,"EASY");
                drawB(hard,"HARD");
            }
            if(hard.pressed==true)
            {
                setcolor(culoare_subBifa);
                drawB(hard,"HARD");
                setcolor(WHITE);
                drawB(easy,"EASY");
                drawB(medium,"MEDIUM");
            }

            //buton inapoi
            initializare_buton(backBUTON,20,690,35,30,200,100,10,5);
            drawB(backBUTON, "back");

            //culori pt modificare setari in engleza
            settextstyle(10,0,4);
            if((C1==4||C1==1||C1==2)&&(C2==5||C2==14||C2==15))
                setcolor(culoareBifaE);
            else
                setcolor(WHITE);
            outtextxy(100,550,"COLOUR OF PLAYER");
            setfillstyle(SOLID_FILL,4);//red
            fillellipse(600,570,30,30);

            setfillstyle(SOLID_FILL,1);//purple
            fillellipse(700,570,30,30);

            setfillstyle(SOLID_FILL,2); //green
            fillellipse(800,570,30,30);

            setfillstyle(SOLID_FILL,5); //blue
            fillellipse(1000,570,30,30);

            setfillstyle(SOLID_FILL,14); //yellow
            fillellipse(1100,570,30,30);

            setfillstyle(SOLID_FILL,15);//white
            fillellipse(1200,570,30,30);

            //afisare text sub culori selectate pentru modifcare setari in engleza
            settextstyle(GOTHIC_FONT,HORIZ_DIR,4);
            setcolor(C1);
            outtextxy(590,610,"PLAYER 1");
            setcolor(C2);
            outtextxy(990,610,"PLAYER 2");
            setcolor(WHITE);
        }
        if(romana.pressed==true)
        {

            // limba pt modicare setari in romana
            setcolor(culoareBifaR);
            outtextxy(100,150,"LIMBA");
            setcolor(culoare_subBifa);
            drawB(romana, "RO");
            setcolor(WHITE);
            drawB(engleza, "ENG");

            //tipul jocului pt modificare setari in romana
            settextstyle(10,0,4);
            setcolor(culoareBifaR);
            if(singlePlayer.pressed==false&&multiPlayer.pressed==false)
                setcolor(WHITE);
            else
                setcolor(culoareBifaR);
            outtextxy(100,300,"TIPUL JOCULUI");
            if(singlePlayer.pressed==false&&multiPlayer.pressed==false)
            {
                setcolor(WHITE);
                drawB(singlePlayer,"UN JUCATOR");
                drawB(multiPlayer,"DOI JUCATORI");
            }
            if(singlePlayer.pressed==true)
            {
                setcolor(culoare_subBifa);
                drawB(singlePlayer,"UN JUCATOR");
                setcolor(WHITE);
                drawB(multiPlayer,"DOI JUCATORI");
            }
            if(multiPlayer.pressed==true)
            {
                setcolor(culoare_subBifa);
                drawB(multiPlayer,"DOI JUCATORI");
                setcolor(WHITE);
                drawB(singlePlayer,"UN JUCATOR");
            }

            //selectare nivel pentru modificare setari in romana
            settextstyle(10,0,4);
            if(easy.pressed==false&&medium.pressed==false&&hard.pressed==false)
                setcolor(WHITE);
            else
                setcolor(culoareBifaR);
            outtextxy(100,425,"NIVEL");
            if(easy.pressed==false&&medium.pressed==false&&hard.pressed==false)
            {
                setcolor(WHITE);
                drawB(easy,"USOR");
                drawB(medium,"MEDIU");
                drawB(hard,"DIFICIL");
            }
            if(easy.pressed==true)
            {
                setcolor(culoare_subBifa);
                drawB(easy,"USOR");
                setcolor(WHITE);
                drawB(medium,"MEDIU");
                drawB(hard,"DIFICIL");
            }
            if(medium.pressed==true)
            {
                setcolor(culoare_subBifa);
                drawB(medium,"MEDIU");
                setcolor(WHITE);
                drawB(easy,"USOR");
                drawB(hard,"DIFICIL");
            }
            if(hard.pressed==true)
            {
                setcolor(culoare_subBifa);
                drawB(hard,"DIFICIL");
                setcolor(WHITE);
                drawB(easy,"USOR");
                drawB(medium,"MEDIU");
            }

            initializare_buton(backBUTON,20,690,35,30,200,100,10,5); //buton inapoi
            drawB(backBUTON, "back");

            //culori pt modificare setari in romana
            settextstyle(10,0,4);
            if((C1==4||C1==1||C1==2)&&(C2==5||C2==14||C2==15))
                setcolor(culoareBifaR);
            else
                setcolor(WHITE);
            outtextxy(100,550,"CULOARE JUCATORI");
            setfillstyle(SOLID_FILL,4);//red
            fillellipse(600,570,30,30);

            setfillstyle(SOLID_FILL,1);//purple
            fillellipse(700,570,30,30);

            setfillstyle(SOLID_FILL,2); //green
            fillellipse(800,570,30,30);

            setfillstyle(SOLID_FILL,5); //blue
            fillellipse(1000,570,30,30);

            setfillstyle(SOLID_FILL,14); //yellow
            fillellipse(1100,570,30,30);

            setfillstyle(SOLID_FILL,15);//white
            fillellipse(1200,570,30,30);

            //text sub culori pentru modificare setari in romana
            settextstyle(GOTHIC_FONT,HORIZ_DIR,4);
            setcolor(C1);
            outtextxy(590,610,"JUCATOR 1");
            setcolor(C2);
            outtextxy(990,610,"JUCATOR 2");
            setcolor(WHITE);
        }
    }
    while(!apasat)
    {
        clearmouseclick(WM_LBUTTONDOWN);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            x=mousex();
            y=mousey();
            if(x>=engleza.l&&x<=engleza.r&&y>=engleza.t&&y<=engleza.b)//seteaza meniul de optiuni in engleza
            {
                cleardevice();
                settextstyle(10,0,4);
                setcolor(culoareBifaE);
                outtextxy(100,150,"LANGUAGE");
                setcolor(WHITE);
                initializare_buton(engleza,600,150,25,15,100,50,10,3);
                setcolor(culoare_subBifa);
                drawB(engleza, "ENG");
                setcolor(WHITE);
                engleza.pressed=true;
                initializare_buton(romana,850,150,30,15,100,50,10,3);
                drawB(romana, "RO");

                //tipul jocului
                settextstyle(10,0,4);
                outtextxy(100,300,"TYPE OF GAME");
                initializare_buton(singlePlayer,600,300,10,13,200,50,10,3);
                drawB(singlePlayer,"SINGLE PLAYER");
                initializare_buton(multiPlayer,850,300,25,13,200,50,10,3);
                drawB(multiPlayer,"MULTIPLAYER");

                //selectare nivel
                settextstyle(10,0,4);
                outtextxy(100,425,"LEVEL");
                initializare_buton(easy,600,425,13,10,100,50,10,4);
                initializare_buton(medium,850,425,13,10,150,50,10,4);
                initializare_buton(hard,1140,425,13,10,100,50,10,4);
                drawB(easy,"EASY");
                drawB(medium,"MEDIUM");
                drawB(hard,"HARD");

                //buton inapoi
                initializare_buton(backBUTON,20,690,35,30,200,100,10,5);
                drawB(backBUTON, "back");

                //culori
                C1=C2=8;///  !!!
                settextstyle(10,0,4);
                outtextxy(100,550,"COLOUR OF PLAYER");
                setfillstyle(SOLID_FILL,4);//red
                fillellipse(600,570,30,30);

                setfillstyle(SOLID_FILL,1);//purple
                fillellipse(700,570,30,30);

                setfillstyle(SOLID_FILL,2); //green
                fillellipse(800,570,30,30);

                setfillstyle(SOLID_FILL,5); //blue
                fillellipse(1000,570,30,30);

                setfillstyle(SOLID_FILL,14); //yellow
                fillellipse(1100,570,30,30);

                setfillstyle(SOLID_FILL,15);//white
                fillellipse(1200,570,30,30);

                settextstyle(GOTHIC_FONT,HORIZ_DIR,4);
                outtextxy(590,610,"PLAYER 1");
                outtextxy(990,610,"PLAYER 2");
            }
            if(x>=romana.l&&x<=romana.r&&y>=romana.t&&y<=romana.b)//seteaza meniul de optiuni in romana
            {
                cleardevice();
                settextstyle(10,0,4);
                setcolor(culoareBifaR);
                outtextxy(100,150,"LIMBA");
                setcolor(WHITE);
                initializare_buton(engleza,600,150,25,15,100,50,10,3);
                drawB(engleza, "ENG");
                initializare_buton(romana,850,150,30,15,100,50,10,3);
                setcolor(culoare_subBifa);
                drawB(romana, "RO");
                setcolor(WHITE);
                romana.pressed=true;

                //tipul jocului
                settextstyle(10,0,4);
                outtextxy(100,300,"TIPUL JOCULUI");
                initializare_buton(singlePlayer,600,300,30,13,200,50,10,3);
                drawB(singlePlayer,"UN JUCATOR");
                initializare_buton(multiPlayer,850,300,15,10,200,50,10,3);
                drawB(multiPlayer,"DOI JUCATORI");

                //selectare nivel
                settextstyle(10,0,4);
                outtextxy(100,425,"NIVEL");
                initializare_buton(easy,600,425,13,10,100,50,10,4);
                initializare_buton(medium,850,425,25,10,150,50,10,4);
                initializare_buton(hard,1140,425,13,10,155,50,10,4);
                drawB(easy,"USOR");
                drawB(medium,"MEDIU");
                drawB(hard,"DIFICIL");

                //buton inapoi
                initializare_buton(backBUTON,20,690,35,30,200,100,10,5);
                drawB(backBUTON, "INAPOI");

                //culori
                C1=C2=8;///   !!!
                settextstyle(10,0,4);
                outtextxy(100,550,"CULOARE JUCATORI");
                setfillstyle(SOLID_FILL,4);//red
                fillellipse(600,570,30,30);

                setfillstyle(SOLID_FILL,1);//purple
                fillellipse(700,570,30,30);

                setfillstyle(SOLID_FILL,2); //green
                fillellipse(800,570,30,30);

                setfillstyle(SOLID_FILL,5); //blue
                fillellipse(1000,570,30,30);

                setfillstyle(SOLID_FILL,14); //yellow
                fillellipse(1100,570,30,30);

                setfillstyle(SOLID_FILL,15);//white
                fillellipse(1200,570,30,30);

                settextstyle(GOTHIC_FONT,HORIZ_DIR,4);
                outtextxy(590,610,"JUCATOR 1");
                outtextxy(990,610,"JUCATOR 2");

            }

            if(x>=singlePlayer.l&&x<=singlePlayer.r&&y>=singlePlayer.t&&y<=singlePlayer.b)// tipul jocului: singlePlayer
            {
                singlePlayer.pressed=true;
                multiPlayer.pressed=false;
                LG=11;
                if(engleza.pressed==true)
                {
                    settextstyle(GOTHIC_FONT,HORIZ_DIR,2);
                    setcolor(BLACK);
                    outtextxy(singlePlayer.l-250,singlePlayer.t,"CHOOSE TYPE->");

                    setcolor(culoareBifaE);
                    settextstyle(10,0,4);
                    outtextxy(100,300,"TYPE OF GAME");
                    setcolor(culoare_subBifa);
                    drawB(singlePlayer,"SINGLE PLAYER");
                    setcolor(WHITE);
                    drawB(multiPlayer,"MULTIPLAYER");

                }
                if(romana.pressed==true)
                {
                    settextstyle(GOTHIC_FONT,HORIZ_DIR,2);
                    setcolor(BLACK);
                    outtextxy(singlePlayer.l-200,singlePlayer.t,"ALEGE TIP->");

                    settextstyle(10,0,4);
                    setcolor(culoareBifaR);
                    outtextxy(100,300,"TIPUL JOCULUI");
                    setcolor(culoare_subBifa);
                    drawB(singlePlayer,"UN JUCATOR");
                    setcolor(WHITE);
                    drawB(multiPlayer,"DOI JUCATORI");
                }

            }
            if(x>=multiPlayer.l&&x<=multiPlayer.r&&y>=multiPlayer.t&&y<=multiPlayer.b)//tipul jocului: multiPlayer
            {
                multiPlayer.pressed=true;
                singlePlayer.pressed=false;
                if(easy.pressed==true)
                    LG=9;
                if(medium.pressed==true)
                    LG=11;
                if(hard.pressed==true)
                    LG=13;
                if(engleza.pressed==true)
                {
                    settextstyle(GOTHIC_FONT,HORIZ_DIR,2);
                    setcolor(BLACK);
                    outtextxy(singlePlayer.l-250,singlePlayer.t,"CHOOSE TYPE->");

                    settextstyle(10,0,4);
                    setcolor(culoareBifaE);
                    outtextxy(100,300,"TYPE OF GAME");
                    setcolor(culoare_subBifa);
                    drawB(multiPlayer,"MULTIPLAYER");
                    setcolor(WHITE);
                    drawB(singlePlayer,"SINGLE PLAYER");
                }
                if(romana.pressed==true)
                {
                    settextstyle(GOTHIC_FONT,HORIZ_DIR,2);
                    setcolor(BLACK);
                    outtextxy(singlePlayer.l-200,singlePlayer.t,"ALEGE TIP->");

                    settextstyle(10,0,4);
                    setcolor(culoareBifaR);
                    outtextxy(100,300,"TIPUL JOCULUI");
                    setcolor(culoare_subBifa);
                    drawB(multiPlayer,"DOI JUCATORI");
                    setcolor(WHITE);
                    drawB(singlePlayer,"UN JUCATOR");
                }
            }
            if(x>=easy.l&&x<=easy.r&&y>=easy.t&&y<=easy.b)//nivel: easy
            {
                if(singlePlayer.pressed==true)
                {
                    LG=11;
                    easy.pressed=true;
                }
                if(multiPlayer.pressed==true )
                {
                    LG=9;
                    easy.pressed=true;
                }
                if(easy.pressed==true)
                {
                    medium.pressed=false;
                    hard.pressed=false;
                    if(engleza.pressed==true)
                    {
                        settextstyle(10,0,4);
                        setcolor(culoareBifaE);
                        outtextxy(100,425,"LEVEL");
                        setcolor(culoare_subBifa);
                        drawB(easy,"EASY");
                        setcolor(WHITE);
                        drawB(medium,"MEDIUM");
                        drawB(hard,"HARD");
                    }
                    if(romana.pressed==true)
                    {
                        settextstyle(10,0,4);
                        setcolor(culoareBifaR);
                        outtextxy(100,425,"NIVEL");
                        setcolor(culoare_subBifa);
                        drawB(easy,"USOR");
                        setcolor(WHITE);
                        drawB(medium,"MEDIU");
                        drawB(hard,"DIFICIL");
                    }
                    setcolor(WHITE);
                }
                if(singlePlayer.pressed==false &&multiPlayer.pressed==false)
                {
                    settextstyle(GOTHIC_FONT,HORIZ_DIR,2);
                    setcolor(RED);
                    if(romana.pressed==true)
                        outtextxy(singlePlayer.l-200,singlePlayer.t,"ALEGE TIP->");
                    if(engleza.pressed==true)
                        outtextxy(singlePlayer.l-250,singlePlayer.t,"CHOOSE TYPE->");
                }
            }
            if(x>=medium.l&&x<=medium.r&&y>=medium.t&&y<=medium.b)//:nivel:mediu
            {
                    LG=11;
                    medium.pressed=true;
                if(medium.pressed==true)
                {
                    easy.pressed=false;
                    hard.pressed=false;
                    if(engleza.pressed==true)
                    {
                        settextstyle(10,0,4);
                        setcolor(culoareBifaE);
                        outtextxy(100,425,"LEVEL");
                        setcolor(culoare_subBifa);
                        drawB(medium,"MEDIUM");
                        setcolor(WHITE);
                        drawB(easy,"EASY");
                        drawB(hard,"HARD");
                    }
                    if(romana.pressed==true)
                    {
                        settextstyle(10,0,4);
                        setcolor(culoareBifaR);
                        outtextxy(100,425,"NIVEL");
                        setcolor(culoare_subBifa);
                        drawB(medium,"MEDIU");
                        setcolor(WHITE);
                        drawB(easy,"USOR");
                        drawB(hard,"DIFICIL");
                    }
                    setcolor(WHITE);
                }
                if(singlePlayer.pressed==false &&multiPlayer.pressed==false)
                {
                    settextstyle(GOTHIC_FONT,HORIZ_DIR,2);
                    setcolor(RED);
                    if(romana.pressed==true)
                        outtextxy(singlePlayer.l-200,singlePlayer.t,"ALEGE TIP->");
                    if(engleza.pressed==true)
                        outtextxy(singlePlayer.l-250,singlePlayer.t,"CHOOSE TYPE->");
                }
            }
            if(x>=hard.l&&x<=hard.r&&y>=hard.t&&y<=hard.b)//nivel:hard
            {

                if(singlePlayer.pressed==true)
                {
                    LG=11;
                    hard.pressed=true;
                }
                if(multiPlayer.pressed==true)
                {
                    LG=13;
                    hard.pressed=true;
                }
                if(hard.pressed==true)
                {
                    setcolor(WHITE);
                    easy.pressed=false;
                    medium.pressed=false;
                    if(engleza.pressed==true)
                    {
                        settextstyle(10,0,4);
                        setcolor(culoareBifaE);
                        outtextxy(100,425,"LEVEL");
                        setcolor(culoare_subBifa);
                        drawB(hard,"HARD");
                        setcolor(WHITE);
                        drawB(easy,"EASY");
                        drawB(medium,"MEDIUM");

                    }
                    if(romana.pressed==true)
                    {
                        settextstyle(10,0,4);
                        setcolor(culoareBifaR);
                        outtextxy(100,425,"NIVEL");
                        setcolor(culoare_subBifa);
                        drawB(hard,"DIFICIL");
                        setcolor(WHITE);
                        drawB(easy,"USOR");
                        drawB(medium,"MEDIU");
                    }
                    setcolor(WHITE);
                }
                if(singlePlayer.pressed==false &&multiPlayer.pressed==false)
                {
                    settextstyle(GOTHIC_FONT,HORIZ_DIR,2);
                    setcolor(RED);
                    if(romana.pressed==true)
                        outtextxy(singlePlayer.l-200,singlePlayer.t,"ALEGE TIP->");
                    if(engleza.pressed==true)
                        outtextxy(singlePlayer.l-250,singlePlayer.t,"CHOOSE TYPE->");
                }
            }
            if(y>500)// selectare culori
            {
                if(C1==8||nr_optiuni!=1)  //culoare initiala de verificare gri=8
                {
                    if(getpixel(x,y)==4)
                        C1=4;
                    if(getpixel(x,y)==1)
                        C1=1;
                    if(getpixel(x,y)==2)
                        C1=2;
                    if(C1!=8)
                    {
                        settextstyle(GOTHIC_FONT,HORIZ_DIR,4);
                        setcolor(C1);
                        if(engleza.pressed==true)
                            outtextxy(590,610,"PLAYER 1");
                        if(romana.pressed==true)
                            outtextxy(590,610,"JUCATOR 1");
                        setcolor(WHITE);
                    }
                    if((C1==4||C1==1||C1==2)&&(C2==5||C2==14||C2==15))
                    {
                        if(romana.pressed==true)
                        {
                            setcolor(culoareBifaR);
                            settextstyle(10,0,4);
                            outtextxy(100,550,"CULOARE JUCATORI");
                            setcolor(WHITE);
                        }
                        if(engleza.pressed==true)
                        {
                            setcolor(culoareBifaE);
                            settextstyle(10,0,4);
                            outtextxy(100,550,"COLOUR OF PLAYER");
                            setcolor(WHITE);
                        }
                    }
                }
                if(C2==8||nr_optiuni!=1)
                {
                    if(getpixel(x,y)==5)
                        C2=5;
                    if(getpixel(x,y)==14)
                        C2=14;
                    if(getpixel(x,y)==15)
                        C2=15;
                    if(C2!=8)
                    {
                        settextstyle(GOTHIC_FONT,HORIZ_DIR,4);
                        setcolor(C2);
                        if(engleza.pressed==true)
                            outtextxy(990,610,"PLAYER 2");
                        if(romana.pressed==true)
                            outtextxy(990,610,"JUCATOR 2");
                        setcolor(WHITE);
                        if((C1==4||C1==1||C1==2)&&(C2==5||C2==14||C2==15))
                        {
                            if(romana.pressed==true)
                            {
                                setcolor(culoareBifaR);
                                settextstyle(10,0,4);
                                outtextxy(100,550,"CULOARE JUCATORI");
                            }
                            if(engleza.pressed==true)
                            {
                                setcolor(culoareBifaE);
                                settextstyle(10,0,4);
                                outtextxy(100,550,"COLOUR OF PLAYER");
                            }
                        }
                        setcolor(WHITE);
                    }
                }
            }
            if(x>=backBUTON.l&&x<=backBUTON.r&&y>=backBUTON.t&&y<=backBUTON.b)//buton back in optiuni
            {
                setcolor(WHITE);
                MainMenu();
            }
        }
    }
}
int main()
{
    initwindow(1600,800,"BriDG It");
    MainMenu();
    getch();
    closegraph();
    return 0;
}
