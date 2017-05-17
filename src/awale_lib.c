#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include "awale_lib.h"

//=============================================================
//======================UTIL===================================
//=============================================================

int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

//renvoie une position que l'on peut jouer pour resoudre les problèmes quand l'IA net trouve pas de bonne position,(arrive quand il ne reste plus beaucoup de grène)
int debloquer_jeu(Position *pos, int ordi_joue){
	int i = 0;
	int m = TAILLE_CAMP;
	if(ordi_joue){
		i = TAILLE_CAMP;
		m = TAILLE_GRILLE;
	}
	for(i = i; i < m; i++){
		if(pos->cases_de_jeu[i] > 0) return i;
	}
	return -1;
}

int getMax(int * src, int size,int *index){
	int i;
	int res = src[0];
	*index = 0;
	for(i = 1; i < size; i++){
		if(src[i] > res) {
			res = src[i];
			*index = i;
		}	
	}
	return res;	
}

int getMin(int * src, int size,int *index){
	int i;
	int res = src[0];
	*index = 0;
	for(i = 1; i < size; i++){
		if(src[i] < res) {
			res = src[i];
			*index = i;
		}	
	}
	return res;	
}

void printCases(Position *pos){

	int i;
	printf("\nJ1[");
	for(i = 0; i < TAILLE_CAMP-1; i++){
		printf("%d,",pos->cases_de_jeu[i]);
	}
	printf("%d]%d\nJ2[",pos->cases_de_jeu[i],pos->pions_pris_joueur);
	for(i = TAILLE_GRILLE-1; i > TAILLE_CAMP; i--){
		printf("%d,",pos->cases_de_jeu[i]);
	}
	printf("%d]%d\n\n",pos->cases_de_jeu[i],pos->pions_pris_ordi);
}

void initPosition(Position *pos){
	int tmp[TAILLE_GRILLE];
	int i;
	for(i = 0; i < TAILLE_GRILLE; i++) tmp[i] = 4;
	memcpy(pos->cases_de_jeu,tmp,TAILLE_GRILLE*sizeof(int));
	pos->ordi_joue = FALSE;
	pos->pions_pris_joueur = 0;
	pos->pions_pris_ordi = 0;
}

int choix_coup(Position pINIT,int ordi_joue){
	int choix;
	char buffer[20];
	do{
		if(!ordi_joue){
			while(OK != getLine("J1 choix(1-n):",buffer,20));
		}
		else{
			while(OK != getLine("J2 choix(1-n):",buffer,20));
		}
		choix = atoi(buffer)-1;
	}while(!coupValide(pINIT,ordi_joue,choix));
	return choix;
}

//=============================================================
//======================MAIN LOOP==============================
//=============================================================

int main_loop(int ia1,int ia2,int verbose,int profMax_j1, int profMax_j2,FUNC_EVAL f_J1, FUNC_EVAL f_J2){

	Position pINIT;
	int ordi_joue = FALSE;
	initPosition(&pINIT);
	int choix = -1;
	int nb_tour = 0;
	if(verbose)printCases(&pINIT);
	while(1){
		if(!ordi_joue){
			if(randIA(pINIT,ordi_joue) != -1){
				if(!ia1){
						choix = choix_coup(pINIT,ordi_joue);
				}
				else {
					if(ia1 == IA_RANDOM){
						choix = randIA(pINIT,ordi_joue);
					}
					else if(ia1 == IA_MINMAX){
						choix = minmax_ia(pINIT,ordi_joue,profMax_j1, f_J1);
					}
					else if(ia1 == IA_ALPHA_BETA){
						choix = alpha_beta_ia(pINIT,ordi_joue,profMax_j1, f_J1);
					}
					else if(ia1 == IA_ALPHA_BETA_THREAD){
						choix = alpha_beta_ia_multi_thread(pINIT,ordi_joue,profMax_j1, f_J1);
					}
					else if(ia1 == IA_MULTI_EVAL){
						choix = multi_eval_ia(pINIT,ordi_joue,profMax_j1, f_J1, EVAL_LV1);
					}
					else{
						choix = minmax_ia_multi_thread(pINIT,ordi_joue,profMax_j1, f_J1);
					}
				}
				if(verbose) printf("J1 joue la case %d, profMax_j1=%d\n",choix,profMax_j1);
				if(!coupValide(pINIT,ordi_joue,choix)){
					printf("error! choix = %d\n",choix);		
					exit(1);
				}
			}
		}
		else{
			if(randIA(pINIT,ordi_joue) != -1){
				if(!ia2){
						choix = choix_coup(pINIT,ordi_joue);
				}
				else {
					if(ia2 == IA_RANDOM){
						choix = randIA(pINIT,ordi_joue);
					}
					else if(ia2 == IA_MINMAX){
						choix = minmax_ia(pINIT,ordi_joue,profMax_j2, f_J2);
					}
					else if(ia2 == IA_ALPHA_BETA){
						choix = alpha_beta_ia(pINIT,ordi_joue,profMax_j2, f_J2);
					}
					else if(ia2 == IA_ALPHA_BETA_THREAD){
						choix = alpha_beta_ia_multi_thread(pINIT,ordi_joue,profMax_j2, f_J2);
					}
					else if(ia2 == IA_MULTI_EVAL){
						choix = multi_eval_ia(pINIT,ordi_joue,profMax_j2, f_J2, EVAL_LV1);
					}
					else{
						choix = minmax_ia_multi_thread(pINIT,ordi_joue,profMax_j2, f_J2);
					}
				}
				if(verbose)printf("J2 joue la case %d, profMax_j2=%d\n",choix,profMax_j2);
				if(!coupValide(pINIT,ordi_joue,choix)){
					printf("error! choix = %d\n",choix);		
					exit(1);
				}				
			}
		}
		jouerCoup(&pINIT,pINIT,ordi_joue,choix);
		if(verbose)printCases(&pINIT);
		if(positionFinale(pINIT,ordi_joue,1)){
			if(verbose)printf("fin de partie\n");
			break;
		}
		ordi_joue = !ordi_joue;
		nb_tour++;
		/*if(profMax_j1 < 10){
			profMax_j1 += nb_tour % 20 == 0 ? 1 : 0;
		}
		if(profMax_j2 < 10){
			profMax_j2 += nb_tour % 20 == 0 ? 1 : 0;
		}*/
	}
	return pINIT.pions_pris_joueur * 100 + pINIT.pions_pris_ordi;
}	


//=============================================================
//======================ALGO===================================
//=============================================================

//function MinMax==============================================
int valeurMinMax(Position* pos_courante,int ordi_joue,int prof,int profMax,FUNC_EVAL f){

	// ordi_joue est un booleen qui est vrai si l'ordi joue
	int tab_valeurs[TAILLE_CAMP];
	int res,posFinal,i;		
	Position pos_next; // En C on crée dans la pile = TRES rapide
 	posFinal = positionFinale(*pos_courante, ordi_joue,prof);
	if (posFinal){
		switch(posFinal){
			case IA_WIN:
				return VALMAX;
				break;
			case J1_WIN:
				return VALMIN;
				break;
			case EGALITE:
				return 0;
				break;
			default:
				break;
		}
		// on retourne VALMAX (=48) si l'ordi gagne et -48 si l'ordi perd  et 0 si nul
	}
	if (prof == profMax) {
		return f(*pos_courante, ordi_joue,prof);
		// dans un premier temps l'évaluation sera la
		// différence du nb de pions pris
	}
	for(i=0; i<TAILLE_CAMP; i++){

		// on joue le coup i, elle teste si on peut prendre les pions dans la case i et les jouer (si pas de pion alors elle retourne invalide). La position de départ,		// est pos_courante
		if (coupValide(*pos_courante,ordi_joue,i)){
			//jouerCoup(&pos_next,pos_courante, ordi_joue-i), on joue le coup i a partir de la position, pos_courante et on met le résultat, dans pos_next
			jouerCoup(&pos_next,*pos_courante,ordi_joue,i);
			// pos_next devient la position courante, et on change le joueur
			tab_valeurs[i]=valeurMinMax(&pos_next, !ordi_joue,prof+1,profMax,f);
		} 
		else {
			if (ordi_joue) tab_valeurs[i]= -100+prof;
			else tab_valeurs[i]= 100-prof;
		}
	}
       
	if (ordi_joue){
		res = getMax(tab_valeurs,TAILLE_CAMP,&i);
		// ecrire le code: res contient le MAX de tab_valeurs
	} else {
		res = getMin(tab_valeurs,TAILLE_CAMP,&i);
		// ecrire le code : res contient le MIN de tab_valeurs
	}
	if(prof == 0) return i;

	//postfix ==================================================================================
	//aucun coup n'est jouable je renvoie qui gagne. pas de famine!
	if(res == -100+prof || res == 100-prof){
		if(pos_courante->pions_pris_ordi > pos_courante->pions_pris_joueur) return VALMAX;
		else if(pos_courante->pions_pris_ordi < pos_courante->pions_pris_joueur) return VALMIN;
		else return EGALITE;
	}
	//==========================================================================================

	return res;
}

int alpha_beta(Position* pos_courante,int ordi_joue,int prof,int profMax, int alpha, int beta,FUNC_EVAL f){

	// ordi_joue est un booleen qui est vrai si l'ordi joue
	int tab_valeurs[TAILLE_CAMP];
	int posFinal,i,best,tmp;
	Position pos_next; 
 	posFinal = positionFinale(*pos_courante, ordi_joue,prof);
	if (posFinal){
		switch(posFinal){
			case IA_WIN:
				return VALMAX;
				break;
			case J1_WIN:
				return VALMIN;
				break;
			case EGALITE:
				return 0;
				break;
			default:
				break;
		}
	}
	if (prof == profMax) {
		//fonction d'evaluation
		return f(*pos_courante, ordi_joue,prof);
	}
	best = !ordi_joue ? INF : -INF;
	for(i=0; i<TAILLE_CAMP; i++){
		if (coupValide(*pos_courante,ordi_joue,i)){
			jouerCoup(&pos_next,*pos_courante,ordi_joue,i);
			tab_valeurs[i]=alpha_beta(&pos_next, !ordi_joue,prof+1,profMax,alpha,beta,f);
		} 
		else {
			if (ordi_joue) tab_valeurs[i]=-100+prof;
			else tab_valeurs[i]=+100-prof;
		}
		if(!ordi_joue){			//min
			if(tab_valeurs[i] < best){
				best = tab_valeurs[i];
				if(best < beta){
					beta = best;
					if(alpha > beta) break;
				}
			}
		}
		else if(tab_valeurs[i] > best){ //max
			best = tab_valeurs[i];
			if(best > alpha){
				alpha = best;
				if(alpha > beta) break;
			}
		}
	}
	if(prof == 0){
		for(i = 0; i < TAILLE_CAMP; i++){
			if(tab_valeurs[i] == best) return i;
		}
	}

	//postfix ==================================================================================
	//aucun coup n'est jouable je renvoie qui gagne. pas de famine! 
	if(best == -100+prof || best == 100-prof){
		if(pos_courante->pions_pris_ordi > pos_courante->pions_pris_joueur) return VALMAX;
		else if(pos_courante->pions_pris_ordi < pos_courante->pions_pris_joueur) return VALMIN;
		else return EGALITE;
	}
	//==========================================================================================

	return best;
}
//=============================================================
//======================IA FUNCTION============================
//=============================================================

int randIA(Position pos,int ordi_joue){
	int valpossible[TAILLE_CAMP];
	int index = -1;	
	int i = 0;
	int j = TAILLE_CAMP;
	int r;
	srand(time(NULL));
	if(ordi_joue){
		i = TAILLE_CAMP;
		j = TAILLE_GRILLE;
	}
	for(i = i; i < j; i++){
		if(pos.cases_de_jeu[i]){
			index++;
			valpossible[index] = ordi_joue ? i-TAILLE_CAMP : i;
		}
	}
	if(index == -1)return -1;
	r = rand()%(index+1);
	return valpossible[r]; 
}

int minmax_ia(Position pINIT, int ordi_joue, int prof,FUNC_EVAL f){
	//printf("minmax_ia\n");
	int choix;
	choix = valeurMinMax(&pINIT,ordi_joue,0,prof,f);
	if(!coupValide(pINIT,ordi_joue,choix)){
		choix = debloquer_jeu(&pINIT, ordi_joue);
		if(ordi_joue){choix -= TAILLE_CAMP;}
		//printf("DEBUG debloquer_jeu: %d\n",choix);
	}
	return choix;
}

int multi_eval_ia(Position pINIT, int ordi_joue, int prof,FUNC_EVAL f1,FUNC_EVAL f2){

	//printf("minmax_ia_multi_thread\n");
	int tab_valeurs_F1[TAILLE_CAMP];
	int tab_valeurs_F2[TAILLE_CAMP];
	int tab_valeurs_Final[TAILLE_CAMP];
	Position pos_next[TAILLE_CAMP];

	int i;
	for(i = 0; i < TAILLE_CAMP; i++){
		if(coupValide(pINIT,ordi_joue,i)){
			jouerCoup(&pos_next[i], pINIT, ordi_joue, i);
			#pragma omp parallel sections
			{
    			#pragma omp section
    			{ 
					tab_valeurs_F1[i] = alpha_beta(&pos_next[i], !ordi_joue, 1,prof,-INF,INF,f1);
				}
    			#pragma omp section
    			{ 
					tab_valeurs_F2[i] = alpha_beta(&pos_next[i], !ordi_joue, 1,prof,-INF,INF,f2);
				}
			}
		}
		else{
			//printf("orJ: %d\n",arg2->ordi_joue);
			if(ordi_joue)tab_valeurs_Final[i] = -100;
			else tab_valeurs_Final[i] = 100;
		}
	}
	for(i = 0; i < TAILLE_CAMP; i++){
		tab_valeurs_Final[i] = tab_valeurs_F1[i] + tab_valeurs_F2[i];
	}

	if(ordi_joue){
		getMax(tab_valeurs_Final,12,&i);
	}
	else{
		getMin(tab_valeurs_Final,12,&i);
	}
	if(!coupValide(pINIT,ordi_joue,i)){
		i = debloquer_jeu(&pINIT, ordi_joue);
		if(ordi_joue){i -= 12;}
		//printf("DEBUG debloquer_jeu: %d\n",i);
	}
	//printf("[%d,%d,%d,%d,%d,%d]\n",val[0],val[1],val[2],val[3],val[4],val[5]);
	return i;

}

int alpha_beta_ia(Position pINIT, int ordi_joue, int prof,FUNC_EVAL f){
	//printf("alpha_beta_ia\n");
	int choix;
	choix = alpha_beta(&pINIT,ordi_joue,0,prof,-INF,INF,f);
	if(!coupValide(pINIT,ordi_joue,choix)){
		choix = debloquer_jeu(&pINIT, ordi_joue);
		if(ordi_joue){choix -= TAILLE_CAMP;}
		//printf("DEBUG debloquer_jeu: %d\n",choix);
	}
	return choix;
}

int minmax_ia_multi_thread(Position pINIT, int ordi_joue, int prof,FUNC_EVAL f){
	//printf("minmax_ia_multi_thread\n");
	int tab_valeurs[TAILLE_CAMP];
	Position pos_next[TAILLE_CAMP];

	int i;
	#pragma omp parallel for
	for(i = 0; i < TAILLE_CAMP; i++){
		if(coupValide(pINIT,ordi_joue,i)){
			jouerCoup(&pos_next[i], pINIT, ordi_joue, i);
			tab_valeurs[i] = valeurMinMax(&pos_next[i], !ordi_joue, 1,prof,f);
		}
		else{
			//printf("orJ: %d\n",arg2->ordi_joue);
			if(ordi_joue)tab_valeurs[i] = -100;
			else tab_valeurs[i] = 100;
		}
	}

	if(ordi_joue){
		getMax(tab_valeurs,12,&i);
	}
	else{
		getMin(tab_valeurs,12,&i);
	}
	if(!coupValide(pINIT,ordi_joue,i)){
		i = debloquer_jeu(&pINIT, ordi_joue);
		if(ordi_joue){i -= 12;}
		//printf("DEBUG debloquer_jeu: %d\n",i);
	}
	//printf("[%d,%d,%d,%d,%d,%d]\n",val[0],val[1],val[2],val[3],val[4],val[5]);
	return i;
}

int alpha_beta_ia_multi_thread(Position pINIT, int ordi_joue, int prof,FUNC_EVAL f){
	//printf("minmax_ia_multi_thread\n");
	int tab_valeurs[TAILLE_CAMP];
	Position pos_next[TAILLE_CAMP];

	int i;
	#pragma omp parallel for
	for(i = 0; i < TAILLE_CAMP; i++){
		if(coupValide(pINIT,ordi_joue,i)){
			jouerCoup(&pos_next[i], pINIT, ordi_joue, i);
			tab_valeurs[i] = alpha_beta(&pos_next[i],!ordi_joue,1,prof,-INF,INF,f);
		}
		else{
			//printf("orJ: %d\n",arg2->ordi_joue);
			if(ordi_joue)tab_valeurs[i] = -100;
			else tab_valeurs[i] = 100;
		}
	}

	if(ordi_joue){
		getMax(tab_valeurs,12,&i);
	}
	else{
		getMin(tab_valeurs,12,&i);
	}
	if(!coupValide(pINIT,ordi_joue,i)){
		i = debloquer_jeu(&pINIT, ordi_joue);
		if(ordi_joue){i -= 12;}
		//printf("DEBUG debloquer_jeu: %d\n",i);
	}
	return i;
}

//=============================================================
//======================CONSTRAINT=============================
//=============================================================

int positionFinale(Position Pcourante,int ordi_joue,int prof){

	if(Pcourante.pions_pris_joueur > 48){
		return 1;
	}
	else if(Pcourante.pions_pris_ordi > 48){
		return 2;
	}
	else if(Pcourante.pions_pris_ordi >= 42 && Pcourante.pions_pris_joueur >= 42){
		return 3;
	}
	return 0;
}

int capturerPion(Position *pos,int ordi_joue,int last_case){

	if(ordi_joue && last_case < TAILLE_CAMP){
		while(last_case >= 0 && pos->cases_de_jeu[last_case] < 4 && pos->cases_de_jeu[last_case] > 1 && pos->cases_de_jeu[last_case] != 0){
			pos->pions_pris_ordi += pos->cases_de_jeu[last_case];
			pos->cases_de_jeu[last_case] = 0;
			last_case--;
		}
		return TRUE;
	}
	else if(!ordi_joue && last_case > TAILLE_CAMP-1){
		while(last_case > TAILLE_CAMP-1 && pos->cases_de_jeu[last_case] < 4 && pos->cases_de_jeu[last_case] > 1 && pos->cases_de_jeu[last_case] != 0){
			pos->pions_pris_joueur += pos->cases_de_jeu[last_case];
			pos->cases_de_jeu[last_case] = 0;
			last_case--;
		}
		return TRUE;
	}
	return FALSE;
}

void jouerCoup(Position * pos_next,Position pos_courante,int ordi_joue,int i){
	int iter,add;
	i += ordi_joue ? TAILLE_CAMP : 0;
	iter = pos_courante.cases_de_jeu[i];
	if(iter >= TAILLE_GRILLE){
		//pour ne pas mettre un pion dans la case que l'on à vidée
		add = iter / TAILLE_GRILLE;
		pos_courante.cases_de_jeu[i] = 0 - add;
		iter += add;
	}
	else pos_courante.cases_de_jeu[i] = 0;
	//semer n pions sur le plateau de jeu
	while(iter != 0){
		i = i == TAILLE_GRILLE-1 ? 0 : i+1;
		pos_courante.cases_de_jeu[i]++;
		iter--;
	}
	*pos_next = pos_courante;
	capturerPion(pos_next,ordi_joue,i);
}

int coupValide(Position pos_courante,int ordi_joue,int i){
	if(ordi_joue) return pos_courante.cases_de_jeu[i+TAILLE_CAMP];
	return pos_courante.cases_de_jeu[i];
}

//=============================================================
//======================EVAL FUNCTION========================== 
//=============================================================

int evaluation(Position pos_courante, int ordi_joue,int i){

	return pos_courante.pions_pris_ordi - pos_courante.pions_pris_joueur;
}

int evaluation2(Position pos_courante, int ordi_joue,int i){

	int n,eval;
	//on cherche à trouver une eval qui cherche comme meilleur coup de bloquer les cases de 'autre' joueur
	eval = 0;
	for(n = 0; n < TAILLE_GRILLE; n++){
		if(pos_courante.cases_de_jeu[n] == 0 && n < TAILLE_CAMP){
			eval = ordi_joue ? eval + 1 : eval - 1; 
		}
		if(pos_courante.cases_de_jeu[n] == 0 && n >= TAILLE_CAMP){
			eval = ordi_joue ? eval - 1 : eval + 1; 
		}
	}
	if(!ordi_joue) eval = -eval;
	eval += pos_courante.pions_pris_ordi - pos_courante.pions_pris_joueur;
	return eval;
}

// egale à lv2 si lv3 commence (prof = 8)
// meilleur que lv2 si lv3 en deux (prof = 8)
//lv2 à peut près égale à lv3
int evaluation3(Position pos_courante, int ordi_joue,int i){

	int n,eval;
	//on cherche à trouver une eval qui cherche comme meilleur coup de bloquer les cases de 'autre' joueur
	eval = 0;
	for(n = 0; n < TAILLE_GRILLE; n++){
		if(pos_courante.cases_de_jeu[n] < 2 && n < TAILLE_CAMP){
			eval = ordi_joue ? eval + 1 : eval - 1; 
		}
		if(pos_courante.cases_de_jeu[n] < 2 && n >= TAILLE_CAMP){
			eval = ordi_joue ? eval - 1 : eval + 1; 
		}
	}
	if(!ordi_joue) eval = -eval;
	eval += pos_courante.pions_pris_ordi - pos_courante.pions_pris_joueur;
	return eval;
}

int evaluation4(Position pos_courante, int ordi_joue,int i){

	int n,eval,count;
	//on cherche à trouver une eval qui cherche comme meilleur coup de bloquer les cases de 'autre' joueur
	eval = 0;
	count = 1;
	for(n = 0; n < TAILLE_GRILLE; n++){
		if(pos_courante.cases_de_jeu[n] == 0 && n < TAILLE_CAMP){
			eval = ordi_joue ? eval + count : eval - count;
			count++;
			if(i+1 >= TAILLE_CAMP) count = 0;
		}
		else if(pos_courante.cases_de_jeu[n] == 0 && n >= TAILLE_CAMP){
			eval = ordi_joue ? eval - count : eval + count; 
			count++;
		}
		else{
			count == 0;
		}
	}
	if(!ordi_joue) eval = -eval;
	eval += pos_courante.pions_pris_ordi - pos_courante.pions_pris_joueur;
	return eval;
}

int benchmark(){

	int prof_j1,prof_j2,scorej1,scorej2,res;

	for(prof_j1 = 1; prof_j1 < 8; prof_j1++){
		for(prof_j2 = 1; prof_j2 < 8; prof_j2++){
			
			res = main_loop( IA_ALPHA_BETA, IA_ALPHA_BETA, FALSE, prof_j1, prof_j2, EVAL_LV1, EVAL_LV2);
			scorej1 = res/100;
			scorej2 = res-(res/100)*100;
			printf("%d%d %d\n", prof_j1, prof_j2, scorej2>scorej1);
		}
	}
	return 0;
}
