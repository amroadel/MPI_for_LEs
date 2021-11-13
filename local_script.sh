rm -f collective_results.txt
mpirun -np 2 ./c.out input_file100.txt >> collective_results.txt
mpirun -np 3 ./c.out input_file100.txt >> collective_results.txt
mpirun -np 1 ./c.out input_file100.txt >> collective_results.txt 
mpirun -np 4 ./c.out input_file100.txt >> collective_results.txt
mpirun -np 5 ./c.out input_file100.txt >> collective_results.txt
mpirun -np 6 ./c.out input_file100.txt >> collective_results.txt
mpirun -np 7 ./c.out input_file100.txt >> collective_results.txt
mpirun -np 8 ./c.out input_file100.txt >> collective_results.txt
mpirun -np 9 ./c.out input_file100.txt >> collective_results.txt
mpirun -np 10 ./c.out input_file100.txt >> collective_results.txt

rm -f parallel_results.txt
mpirun -np 2 ./p.out input_file100.txt >> parallel_results.txt
mpirun -np 3 ./p.out input_file100.txt >> parallel_results.txt
mpirun -np 1 ./p.out input_file100.txt >> parallel_results.txt 
mpirun -np 4 ./p.out input_file100.txt >> parallel_results.txt
mpirun -np 5 ./p.out input_file100.txt >> parallel_results.txt
mpirun -np 6 ./p.out input_file100.txt >> parallel_results.txt
mpirun -np 7 ./p.out input_file100.txt >> parallel_results.txt
mpirun -np 8 ./p.out input_file100.txt >> parallel_results.txt
mpirun -np 9 ./p.out input_file100.txt >> parallel_results.txt
mpirun -np 10 ./p.out input_file100.txt >> parallel_results.txt