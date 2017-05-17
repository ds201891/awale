#include <limits.h>

#define FALSE 0
#define TRUE 1

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

#define J1_WIN 1
#define IA_WIN 2
#define EGALITE 3

#define VALMAX 48 //IA
#define VALMIN -48 //J1
#define PROFMAX 8
#define PROFMAX_J1 7
#define PROFMAX_J2 8
#define INF INT_MAX

#define EVAL_LV1 evaluation
#define EVAL_LV2 evaluation2
#define EVAL_LV3 evaluation3
#define EVAL_LV4 evaluation4


#define IA_RANDOM 55
#define IA_MINMAX 56
#define IA_MINMAX_THREAD 57
#define IA_ALPHA_BETA 58
#define IA_ALPHA_BETA_THREAD 59
#define IA_MULTI_EVAL 60

#define NB_THREAD 4

#define TAILLE_GRILLE 24
#define TAILLE_CAMP 12

//function for the awale game
typedef struct Position {
	int cases_de_jeu[TAILLE_GRILLE];		
	int ordi_joue; // boolean vrai si c'est à l'ordi de jouer et faux sinon
	int pions_pris_joueur; // pions pris par le joueur
	int pions_pris_ordi; // pions pris par l'ordi
}Position;

typedef int (*FUNC_EVAL)(Position , int ,int );

int randIA(Position pos,int ordi_joue);
int getLine (char *prmpt, char *buff, size_t sz);
int getMin(int * src, int size,int *index);
int getMax(int * src, int size,int *index);

int debloquer_jeu(Position *pos, int ordi_joue);
int main_loop(int ia1,int ia2,int verbose,int profMax_j1, int profMax_j2,FUNC_EVAL f_J1, FUNC_EVAL f_J2);
int capturerPion(Position *pos,int ordi_joue,int last_case);
void printCases(Position *pos);
void initPosition(Position *pos);
int positionFinale(Position Pcourante,int ordi_joue,int prof);
void jouerCoup(Position * pos_next,Position pos_courante,int ordi_joue,int i);
int choix_coup(Position pINIT,int ordi_joue);

int coupValide(Position pos_courante,int ordi_joue,int i);

//MinMax function

int valeurMinMax(Position* pos_courante,int ordi_joue,int prof,int profMax,FUNC_EVAL f);
int alpha_beta(Position* pos_courante,int ordi_joue,int prof,int profMax, int alpha, int beta,FUNC_EVAL f);

int minmax_ia_multi_thread(Position pINIT, int ordi_joue,int prof,FUNC_EVAL f);
int minmax_ia(Position pINIT, int ordi_joue,int prof,FUNC_EVAL f);
int alpha_beta_ia_multi_thread(Position pINIT, int ordi_joue, int prof,FUNC_EVAL f);
int alpha_beta_ia(Position pINIT, int ordi_joue, int prof,FUNC_EVAL f);
int multi_eval_ia(Position pINIT, int ordi_joue, int prof,FUNC_EVAL f1,FUNC_EVAL f2);

int evaluation(Position pos_courante, int ordi_joue,int i);
int evaluation2(Position pos_courante, int ordi_joue,int i);
int evaluation3(Position pos_courante, int ordi_joue,int i);
int evaluation4(Position pos_courante, int ordi_joue,int i);

int benchmark();

