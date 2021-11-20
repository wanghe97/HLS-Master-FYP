#polybench

examples=(heat-3d fdtd-2d jacobi-1d seidel-2d jacobi-2d nussinov floyd-warshall 3mm 2mm doitgen bicg mvt atas syrk gemver symm gesummv gemm trmm syr2k cholesky trisolv lu ludcmp durbin covariance)
#resultfilename=benchmarks/polybench-syn/results/polybench_3flags_const_inl_cse.txt
#resultfilename=benchmarks/polybench-syn/results/polybench_3flags_const_cse_inl.txt
#resultfilename=benchmarks/polybench-syn/results/polybench_3flags_cse_inl_const.txt
#resultfilename=benchmarks/polybench-syn/results/polybench_3flags_cse_const_inl.txt
resultfilename=benchmarks/polybench-syn/results/polybench_2flags_const_if.txt

foldername=(stencils
stencils
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
linear-algebra/solvers
data-mining)


cd vericert2

counter=0
length=${#examples[*]}
while [ $counter -lt $length ]
do

./bin/vericert benchmarks/polybench-syn/${foldername[$counter]}/${examples[$counter]}.c -o benchmarks/polybench-syn/results/vericert.v -DSYNTHESIS -O0 -finline -fconst-prop -fif-conversion
#-fcse
echo "${examples[$counter]}" >> $resultfilename
iverilog -o vericert -- benchmarks/polybench-syn/results/vericert.v
./vericert >> $resultfilename
echo "" >> $resultfilename

echo "Finish example ${examples[$counter]}"
((counter++))

done

#iverilog -o vericert -- benchmarks/polybench-syn/results/vericert.v
#./vericert

#cd benchmarks/polybench-syn/results
#mkdir ${examples[$counter]}
#cd ${examples[$counter]}
#. /opt/Xilinx/Vivado/2019.2/settings64.sh >& /dev/null; 
#vivado -mode batch -source ../synth.tcl
#vivado -mode batch -source synth_vericert.tcl

