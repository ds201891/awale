#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "awale_lib.h"
#include "benchmark.h"

struct timeval timer_start, timer_end;

void start_timer(){
	
	if(-1 == clock_gettime(NULL, &timer_start)){
		start_timer();
	}
}

void stop_timer(){
	if(-1 == clock_gettime(NULL, &timer_end)){
		stop_timer();
	}
}

long get_elapse_time(){
	
	long res = (timer_end.tv_sec - timer_start.tv_sec) *1.0e9 + (timer_end.tv_usec - timer_start.tv_usec);
	return res;
}

int benchmark_time(){

	int profMax = 9;
	int i;
	long res_minmax_eval_lv1[10],res_alpha_beta_eval_lv1[10],res_minmax_eval_lv2[10],res_alpha_beta_eval_lv2[10];
	Position p;
	initPosition(&p);
	for(i = 0; i <= profMax; i++){
		start_timer();
		valeurMinMax(&p,TRUE,0,i,EVAL_LV1);
		stop_timer();
		res_minmax_eval_lv1[i] = get_elapse_time();

		start_timer();
		alpha_beta(&p,TRUE,0,i,-INF,INF,EVAL_LV1);
		stop_timer();
		res_alpha_beta_eval_lv1[i] = get_elapse_time();

		start_timer();
		valeurMinMax(&p,TRUE,0,i,EVAL_LV2);
		stop_timer();
		res_minmax_eval_lv2[i] = get_elapse_time();

		start_timer();
		alpha_beta(&p,TRUE,0,i,-INF,INF,EVAL_LV2);
		stop_timer();
		res_alpha_beta_eval_lv2[i] = get_elapse_time();
				
	}
	for(i = 0; i < profMax; i++) printf("%ld %ld %ld %ld %d\n", res_minmax_eval_lv1[i],res_alpha_beta_eval_lv1[i],res_minmax_eval_lv2[i],res_alpha_beta_eval_lv2[i],i);
	return 0;
}





