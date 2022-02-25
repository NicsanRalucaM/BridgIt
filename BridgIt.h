#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
//////////////////////////////////////////////////////// MENIU ////////////////////////////////////

void initializare_buton(struct buton &m,int left,int top,int xtext,int ytext,int brsize,int bbsize,int font,int fontsize);
void drawB(buton a, char nume[]);
void Option();
void timer();
void MainMenu();
////////////////////////////
void DFS(int x,int v[200],int mat[200][200]);
void creare_matrice1_graf();
void creare_matrice2_graf();
int nrcol(int x);
int nrlin(int x);
void creareTablaDeJoc();
void desenareTablaDeJoc();
void initializare();
int castigator1();
int castigator2();
void verificare_castigator();
int exceptie(int l,int c);
void punerePiesa();
void punerePiesaCalculatorEasy();
void punerePiesaCalculatorHard();
void punerePiesaCalculatorMedium();
void joc_de1_easy();
void joc_de1_hard();
void joc_de1_medium();
void joc_de2();
void citesteSir(char text[20],char s[20],int x,int y,bool stergere);
void nume_2persoane();
void nume_1pers();
void reguli_de_joc();

#endif // HEADER_H_INCLUDED
