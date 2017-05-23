#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "awale_lib.h"

#ifdef BENCH
#include "benchmark.h"
#endif

//test jouerCoup
int test1(){
	printf("test1\n");
	Position pINIT,p2,p3;
	initPosition(&pINIT);
	jouerCoup(&p2,pINIT,TRUE,4);
	jouerCoup(&p3,pINIT,FALSE,4);
	printCases(&pINIT);
	printCases(&p2);
	printCases(&p3);
	return 0;
}

int test2(){
	printf("test2\n");
	Position pINIT,p2,p3;
	initPosition(&pINIT);
	pINIT.cases_de_jeu[8] = 12;
	jouerCoup(&p2,pINIT,FALSE,4);
	pINIT.cases_de_jeu[8] = 24;
	jouerCoup(&p3,pINIT,FALSE,4);
	printCases(&pINIT);
	printCases(&p2);
	printCases(&p3);
	return 0;
}

//test coupValide
int test3(){
	printf("test3\n");
	Position pINIT,p2;
	initPosition(&pINIT);
	jouerCoup(&p2,pINIT,FALSE,4);
	printf("%d\n",coupValide(p2,FALSE,4));
	printf("%d\n",coupValide(p2,TRUE,4));
	return 0;
}

//test capturerPion

int test4(){
	int i;
	printf("test4\n");
	Position pINIT,p2,p3;
	initPosition(&pINIT);
	for(i = 0; i < 24; i++)pINIT.cases_de_jeu[i] = 1;
	pINIT.cases_de_jeu[0] = 23;
	printCases(&pINIT);
	jouerCoup(&p2,pINIT,FALSE,0);
	pINIT.cases_de_jeu[17] = 3;
	jouerCoup(&p3,pINIT,FALSE,0);
	printCases(&p2);
	printCases(&p3);
	return 0;
}

//test positionFinale

int test5(){

	printf("test5\n");
	Position pINIT;
	initPosition(&pINIT);
	pINIT.pions_pris_joueur = 25;
	pINIT.pions_pris_ordi = 24;
	printf("%d\n",positionFinale(pINIT,TRUE,1));
	pINIT.pions_pris_joueur = 24;
	pINIT.pions_pris_ordi = 25;
	printf("%d\n",positionFinale(pINIT,TRUE,1));
	pINIT.pions_pris_joueur = 23;
	pINIT.pions_pris_ordi = 23;
	printf("%d\n",positionFinale(pINIT,TRUE,1));
	pINIT.pions_pris_joueur = 24;
	pINIT.pions_pris_ordi = 24;
	printf("%d\n",positionFinale(pINIT,TRUE,1));
	return 0;
}
//test valeurMinMax
int test6(){

	printf("test4\n");
	Position pINIT;
	initPosition(&pINIT);
	for(int i = 12; i < 24; i++){pINIT.cases_de_jeu[i] = 0;}
	printCases(&pINIT);
	printf("%d\n",alpha_beta_ia_multi_thread(pINIT, TRUE, PROFMAX,evaluation2));
	//for(i = 0; i < 6; i++)pINIT.cases_de_jeu[i] = 1;
	return 0;
}
//alpha beta == minmax 
int test7(){
	printf("test7\n");
	int listIA[] = {IA_RANDOM, IA_MINMAX_THREAD, IA_ALPHA_BETA};
	int i,j;
	for(i = 2; i < 7;i++){
		for(j = 2; j < 7;j++){

			if(main_loop(listIA[2],listIA[1],FALSE,i,j,evaluation2,evaluation2) != main_loop(listIA[1],listIA[2],FALSE,i,j,evaluation2,evaluation2)){

				printf("Fail: profj1=%d profj2=%d\n",i,j);
			}
		}
	}
	return 0;
}

int main (int argc, char *argv[]){

	int runTest = FALSE;
	int ia1 = IA_ALPHA_BETA_THREAD;
	int ia2 = IA_ALPHA_BETA_THREAD;
	//int bench = FALSE;
	int i;
	int arg_j1 = FALSE;
	FUNC_EVAL tab_eval[4] = {EVAL_LV1,EVAL_LV2,EVAL_LV3,EVAL_LV4};
	FUNC_EVAL eval_J1 = EVAL_LV2;
	FUNC_EVAL eval_J2 = EVAL_LV2;
	for(i = 0; i < argc; i++){

		if(!strcmp(argv[i],"-t"))runTest = TRUE;
		else if(!strcmp(argv[i],"-p2_random")) ia2 = IA_RANDOM;
		else if(!strcmp(argv[i],"-p1_random")) ia1 = IA_RANDOM;
		else if(!strcmp(argv[i],"-p1_minmax")) ia1 = IA_MINMAX;
		else if(!strcmp(argv[i],"-p2_minmax")) ia2 = IA_MINMAX;
		else if(!strcmp(argv[i],"-p1_no_ia")) ia1 = FALSE;
		else if(!strcmp(argv[i],"-p2_no_ia")) ia2 = FALSE;
		else if(!strcmp(argv[i],"-p1_minmax_thread")) ia1 = IA_MINMAX_THREAD;
		else if(!strcmp(argv[i],"-p2_minmax_thread")) ia2 = IA_MINMAX_THREAD;
		else if(!strcmp(argv[i],"-p1_alpha_beta_thread")) ia1 = IA_ALPHA_BETA_THREAD;
		else if(!strcmp(argv[i],"-p2_alpha_beta_thread")) ia2 = IA_ALPHA_BETA_THREAD;
		else if(!strcmp(argv[i],"-p1_alpha_beta")) ia1 = IA_ALPHA_BETA;
		else if(!strcmp(argv[i],"-p2_alpha_beta")) ia2 = IA_ALPHA_BETA;
		//else if(!strcmp(argv[i],"-benchmark")) bench = TRUE;
		else if(!strcmp(argv[i],"-p2_minmax_thread")) ia2 = IA_MINMAX_THREAD;
		else if(!strcmp(argv[i],"-p1_alpha_beta")) ia1 = IA_ALPHA_BETA;
		else if(!strcmp(argv[i],"-p2_alpha_beta")) ia2 = IA_ALPHA_BETA;
		else if(!strcmp(argv[i],"-p1_multi_eval")) ia1 = IA_MULTI_EVAL;
		else if(!strcmp(argv[i],"-p2_multi_eval")) ia2 = IA_MULTI_EVAL;	
#ifdef BENCH
		//else if(!strcmp(argv[i],"-benchmark")) bench = TRUE;
#endif
		else if(!strcmp(argv[i],"-p2_eval_lv1")) eval_J2 = EVAL_LV1;
		else if(!strcmp(argv[i],"-p2_eval_lv2")) eval_J2 = EVAL_LV2;
		else if(!strcmp(argv[i],"-p1_eval_lv1")) eval_J1 = EVAL_LV1;
		else if(!strcmp(argv[i],"-p1_eval_lv2")) eval_J1 = EVAL_LV2;
		else if(!strcmp(argv[i],"-p1_eval_lv3")) eval_J1 = EVAL_LV3;
		else if(!strcmp(argv[i],"-p2_eval_lv3")) eval_J2 = EVAL_LV3;
		else if(!strcmp(argv[i],"-p1_eval_lv4")) eval_J1 = EVAL_LV4;
		else if(!strcmp(argv[i],"-p2_eval_lv4")) eval_J2 = EVAL_LV4;
	}
	if(runTest){
		/*test1();
		test2();
		test3();
		test4();
		test5();
		test6();*/
		test7();
	}
#ifdef BENCH
	else if(bench){
		benchmark_time();
	}
#endif
	else{
		//test6();
		main_loop(ia1,ia2,TRUE,PROFMAX_J1,PROFMAX_J2,eval_J1,eval_J2);
	}
	return 0;
}


