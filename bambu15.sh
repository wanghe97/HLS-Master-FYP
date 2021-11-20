#!/bin/bash
#Firstly use this for -O3, when need flags, refer to bambu11 (add extra loop)
#polybench synthesis--combine flags with positive affect for 3 metrics separately for each polybench.

#alias bambu=/opt/panda/bin/bambu

#-fguess-branch-probability -ftree-ccp -ftree-ch -ftree-dominator-opts -ftree-forwprop -ftree-fre -ftree-slsr -finline-small-functions -fipa-cp -fcode-hoisting -ftree-pre -ftree-vrp -fpredictive-commoning -ftree-loop-distribution -ftree-loop-distribute-patterns -fpeel-loops

examples=(heat-3d jacobi-1d seidel-2d jacobi-2d nussinov floyd-warshall 3mm 2mm doitgen bicg mvt atas syrk gemver symm gesummv gemm trmm syr2k cholesky trisolv lu ludcmp covariance)
resultfilename=/home/edana/HLS_output/polybench_o0.txt
foldername=(stencils
stencils
stencils
stencils
medley
medley
linear-algebra/kernels
linear-algebra/kernels
linear-algebra/kernels
linear-algebra/kernels
linear-algebra/kernels
linear-algebra/kernels
linear-algebra/blas
linear-algebra/blas
linear-algebra/blas
linear-algebra/blas
linear-algebra/blas
linear-algebra/blas
linear-algebra/blas
linear-algebra/solvers
linear-algebra/solvers
linear-algebra/solvers
linear-algebra/solvers
data-mining)
#filename=(module.c module.c Keccak.c module.c)
#topname=icrc
#tbfile=(test.xml test.xml test.xml test.xml)

#change to counter=0 to access the first element in array
counter=0
length=${#examples[*]}
#length=1
while [ $counter -lt $length ]
do

cd /home/edana/polybench-syn
cd ${foldername[$counter]}
mkdir defaulto3
cd defaulto3
#/opt/panda/bin/bambu ../$filename --top-fname=$topname --generate-tb=../$tbfile --evaluation --no-iob

#Slices=$(/opt/panda/bin/bambu ../$filename --top-fname=$topname --generate-tb=../$tbfile --evaluation --no-iob | tail -n 18 | grep 'Slices' | cut -d ":" -f 2)
#echo $Slices
#result=$(/opt/panda/bin/bambu ../${examples[$counter]}.c --simulate --simulator=VERILATOR --accept-nonzero-return --evaluation --compiler=I386_GCC8 -O0 -ftree-ccp -ftree-ch -ftree-dominator-opts -ftree-forwprop -ftree-fre -ftree-slsr -finline-small-functions -fcode-hoisting -ftree-pre -ftree-vrp -fpredictive-commoning|& tail -n 16)

#result=$(/opt/panda/bin/bambu ../${examples[$counter]}.c --simulate --simulator=VERILATOR --accept-nonzero-return --evaluation --compiler=I386_GCC8 -O0 -fauto-inc-dec -fbranch-count-reg -fcombine-stack-adjustments -fcompare-elim -fcprop-registers -fdce -fdefer-pop -fdelayed-branch -fdse -fforward-propagate -fguess-branch-probability -fif-conversion2 -fif-conversion -finline-functions-called-once -fipa-pure-const -fipa-profile -fipa-reference -fmerge-constants -fmove-loop-invariants -fomit-frame-pointer -freorder-blocks -fshrink-wrap -fshrink-wrap-separate -fsplit-wide-types -fssa-backprop -fssa-phiopt -ftree-bit-ccp -ftree-ccp -ftree-ch -ftree-coalesce-vars -ftree-copy-prop -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-sink -ftree-slsr -ftree-sra -ftree-pta -ftree-ter -funit-at-a-time -fthread-jumps -falign-functions -falign-jumps -falign-loops -falign-labels -fcaller-saves -fcrossjumping -fcse-follow-jumps -fcse-skip-blocks -fdelete-null-pointer-checks -fdevirtualize -fdevirtualize-speculatively -fexpensive-optimizations -fgcse -fgcse-lm -fhoist-adjacent-loads -finline-small-functions -findirect-inlining -fipa-cp -fipa-bit-cp -fipa-vrp -fipa-sra -fipa-icf -fisolate-erroneous-paths-dereference -flra-remat -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining -fpeephole2 -freorder-blocks-algorithm=stc -freorder-blocks-and-partition -freorder-functions -frerun-cse-after-loop -fsched-interblock -fsched-spec -fschedule-insns -fschedule-insns2 -fstore-merging -fstrict-aliasing -ftree-builtin-call-dce -ftree-switch-conversion -ftree-tail-merge -fcode-hoisting -ftree-pre -ftree-vrp -fipa-ra -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-loop-vectorize -ftree-loop-distribution -ftree-loop-distribute-patterns -floop-interchange -floop-unroll-and-jam -fsplit-paths -ftree-slp-vectorize -fvect-cost-model -ftree-partial-pre -fpeel-loops -fipa-cp-clone|& tail -n 16)
# -O0 ${flags[$counter1]} ${BAMBU_OPTION[$counter]}

#86
#result=$(/opt/panda/bin/bambu ../${examples[$counter]}.c --simulate --simulator=VERILATOR --accept-nonzero-return --evaluation --compiler=I386_GCC8 -O0 -falign-labels -fbranch-count-reg -fcaller-saves -fcode-hoisting -fcombine-stack-adjustments -fcompare-elim -fcprop-registers -fcrossjumping -fcse-follow-jumps -fdefer-pop -fdevirtualize -fdevirtualize-speculatively -fexpensive-optimizations -fforward-propagate -fgcse -fgcse-after-reload -fguess-branch-probability -fhoist-adjacent-loads -fif-conversion -fif-conversion2 -findirect-inlining -finline-functions -finline-functions-called-once -finline-small-functions -fipa-bit-cp -fipa-cp -fipa-cp-clone -fipa-icf -fipa-icf-functions -fipa-icf-variables -fipa-profile -fipa-pure-const -fipa-ra -fipa-reference -fipa-sra -fipa-vrp -fisolate-erroneous-paths-dereference -floop-interchange -floop-unroll-and-jam -flra-remat -fmove-loop-invariants -fomit-frame-pointer -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining -fpeel-loops -fpeephole2 -fpredictive-commoning -freorder-blocks -freorder-blocks-and-partition -freorder-functions -frerun-cse-after-loop -fschedule-insns2 -fshrink-wrap -fsplit-loops -fsplit-paths -fsplit-wide-types -fssa-phiopt -fstore-merging -fstrict-aliasing -fthread-jumps -ftree-bit-ccp -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars -ftree-copy-prop -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-fre -ftree-loop-distribute-patterns -ftree-loop-distribution -ftree-loop-vectorize -ftree-partial-pre -ftree-pre -ftree-pta -ftree-sink -ftree-slp-vectorize -ftree-slsr -ftree-sra -ftree-switch-conversion -ftree-tail-merge -ftree-ter -ftree-vrp -funswitch-loops|& tail -n 16)

#91
#result=$(/opt/panda/bin/bambu ../${examples[$counter]}.c --simulate --simulator=VERILATOR --accept-nonzero-return --evaluation --compiler=I386_GCC8 -O0 -falign-functions=16 -falign-jumps=16 -falign-loops=16 -falign-labels -fbranch-count-reg -fcaller-saves -fcode-hoisting -fcombine-stack-adjustments -fcompare-elim -fcprop-registers -fcrossjumping -fcse-follow-jumps -fdefer-pop -fdevirtualize -fdevirtualize-speculatively -fexpensive-optimizations -fforward-propagate -fgcse -fgcse-after-reload -fguess-branch-probability -fhoist-adjacent-loads -fif-conversion -fif-conversion2 -findirect-inlining -finline-functions -finline-functions-called-once -finline-small-functions -fipa-bit-cp -fipa-cp -fipa-cp-clone -fipa-icf -fipa-icf-functions -fipa-icf-variables -fipa-profile -fipa-pure-const -fipa-ra -fipa-reference -fipa-sra -fipa-vrp -fisolate-erroneous-paths-dereference -floop-interchange -floop-unroll-and-jam -flra-remat -fmove-loop-invariants -fomit-frame-pointer -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining -fpeel-loops -fpeephole2 -fpredictive-commoning -freorder-blocks -freorder-blocks-algorithm=stc -freorder-blocks-and-partition -freorder-functions -frerun-cse-after-loop -fschedule-insns2 -fshrink-wrap -fsplit-loops -fsplit-paths -fsplit-wide-types -fssa-phiopt -fstore-merging -fstrict-aliasing -fthread-jumps -ftree-bit-ccp -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars -ftree-copy-prop -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-fre -ftree-loop-distribute-patterns -ftree-loop-distribution -ftree-loop-vectorize -ftree-partial-pre -ftree-pre -ftree-pta -ftree-sink -ftree-slp-vectorize -ftree-slsr -ftree-sra -ftree-switch-conversion -ftree-tail-merge -ftree-ter -ftree-vrp -funswitch-loops -fvect-cost-model=dynamic|& tail -n 16)

#88
#result=$(/opt/panda/bin/bambu ../${examples[$counter]}.c --simulate --simulator=VERILATOR --accept-nonzero-return --evaluation --compiler=I386_GCC8 -O0 -falign-labels -fbranch-count-reg -fcaller-saves -fcode-hoisting -fcombine-stack-adjustments -fcompare-elim -fcprop-registers -fcrossjumping -fcse-follow-jumps -fdefer-pop -fdevirtualize -fdevirtualize-speculatively -fexpensive-optimizations -fforward-propagate -fgcse -fgcse-after-reload -fguess-branch-probability -fhoist-adjacent-loads -fif-conversion -fif-conversion2 -findirect-inlining -finline-functions -finline-functions-called-once -finline-small-functions -fipa-bit-cp -fipa-cp -fipa-cp-clone -fipa-icf -fipa-icf-functions -fipa-icf-variables -fipa-profile -fipa-pure-const -fipa-ra -fipa-reference -fipa-sra -fipa-vrp -fisolate-erroneous-paths-dereference -floop-interchange -floop-unroll-and-jam -flra-remat -fmove-loop-invariants -fomit-frame-pointer -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining -fpeel-loops -fpeephole2 -fpredictive-commoning -freorder-blocks -freorder-blocks-algorithm=stc -freorder-blocks-and-partition -freorder-functions -frerun-cse-after-loop -fschedule-insns2 -fshrink-wrap -fsplit-loops -fsplit-paths -fsplit-wide-types -fssa-phiopt -fstore-merging -fstrict-aliasing -fthread-jumps -ftree-bit-ccp -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars -ftree-copy-prop -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-fre -ftree-loop-distribute-patterns -ftree-loop-distribution -ftree-loop-vectorize -ftree-partial-pre -ftree-pre -ftree-pta -ftree-sink -ftree-slp-vectorize -ftree-slsr -ftree-sra -ftree-switch-conversion -ftree-tail-merge -ftree-ter -ftree-vrp -funswitch-loops -fvect-cost-model=dynamic|& tail -n 16)

#all+fno
#result=$(/opt/panda/bin/bambu ../${examples[$counter]}.c --simulate --simulator=VERILATOR --accept-nonzero-return --evaluation --compiler=I386_GCC8 -O0 -falign-functions=16 -falign-jumps=16 -falign-loops=16 -finline -fno-branch-count-reg -fno-function-cse -fno-zero-initialized-in-bss -fauto-inc-dec -fbranch-count-reg -fcombine-stack-adjustments -fcompare-elim -fcprop-registers -fdce -fdefer-pop -fdelayed-branch -fdse -fforward-propagate -fguess-branch-probability -fif-conversion2 -fif-conversion -finline-functions-called-once -fipa-pure-const -fipa-profile -fipa-reference -fmerge-constants -fmove-loop-invariants -fomit-frame-pointer -freorder-blocks -fshrink-wrap -fshrink-wrap-separate -fsplit-wide-types -fssa-backprop -fssa-phiopt -ftree-bit-ccp -ftree-ccp -ftree-ch -ftree-coalesce-vars -ftree-copy-prop -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-sink -ftree-slsr -ftree-sra -ftree-pta -ftree-ter -funit-at-a-time -fthread-jumps -falign-functions -falign-jumps -falign-loops -falign-labels -fcaller-saves -fcrossjumping -fcse-follow-jumps -fcse-skip-blocks -fdelete-null-pointer-checks -fdevirtualize -fdevirtualize-speculatively -fexpensive-optimizations -fgcse -fgcse-lm -fhoist-adjacent-loads -finline-small-functions -findirect-inlining -fipa-cp -fipa-bit-cp -fipa-vrp -fipa-sra -fipa-icf -fisolate-erroneous-paths-dereference -flra-remat -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining -fpeephole2 -freorder-blocks-algorithm=stc -freorder-blocks-and-partition -freorder-functions -frerun-cse-after-loop -fsched-interblock -fsched-spec -fschedule-insns -fschedule-insns2 -fstore-merging -fstrict-aliasing -ftree-builtin-call-dce -ftree-switch-conversion -ftree-tail-merge -fcode-hoisting -ftree-pre -ftree-vrp -fipa-ra -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-loop-vectorize -ftree-loop-distribution -ftree-loop-distribute-patterns -floop-interchange -floop-unroll-and-jam -fsplit-paths -ftree-slp-vectorize -fvect-cost-model -ftree-partial-pre -fpeel-loops -fipa-cp-clone|& tail -n 16)

result=$(/opt/panda/bin/bambu ../${examples[$counter]}.c --simulate --simulator=VERILATOR --accept-nonzero-return --evaluation --compiler=I386_GCC8 -O3|& tail -n 16)

echo "${examples[$counter]}" >> $resultfilename
echo "$result" >> $resultfilename
echo "" >> $resultfilename
echo "Finish ${examples[$counter]}"

echo "Finish example ${examples[$counter]}"
((counter++))

done


echo "Finish!"


