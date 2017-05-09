 set term postscript portrait enhanced
set output 'result_time.ps'
#set term gif transparent interlace small size 500, 707 # xFFFFFF x333333 x333333 x0055FF x005522 x660000 xFF0000 x00FF00 x0000FF
#set output 'mesure_L3_cache.gif'
set key below
set title 'minmax alpha_beta time'
set xlabel 'profMax'
set ylabel 'nanosecs'
plot "result.dat" using 5:1 with line title "min max EVAL_LV1", "result.dat" using 5:2 with line title "alpha beta EVAL_LV1", "result.dat" using 5:3 with line title "min max EVAL_LV2", "result.dat" using 5:4 with line title "alpha beta EVAL_LV2"
set nolabel
set noarrow
