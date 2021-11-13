rm -f dcollective_results.txt 
mpirun -np 2 -hosts 10.7.57.250,10.7.57.142 ./c.out input_file100.txt >> dcollective_results.txt
mpirun -np 3 -hosts 10.7.57.250,10.7.57.142 ./c.out input_file100.txt >> dcollective_results.txt
mpirun -np 1 -hosts 10.7.57.250,10.7.57.142 ./c.out input_file100.txt >> dcollective_results.txt 
mpirun -np 4 -hosts 10.7.57.250,10.7.57.142 ./c.out input_file100.txt >> dcollective_results.txt
mpirun -np 5 -hosts 10.7.57.250,10.7.57.142 ./c.out input_file100.txt >> dcollective_results.txt
mpirun -np 6 -hosts 10.7.57.250,10.7.57.142 ./c.out input_file100.txt >> dcollective_results.txt
mpirun -np 7 -hosts 10.7.57.250,10.7.57.142 ./c.out input_file100.txt >> dcollective_results.txt
mpirun -np 8 -hosts 10.7.57.250,10.7.57.142 ./c.out input_file100.txt >> dcollective_results.txt
mpirun -np 9 -hosts 10.7.57.250,10.7.57.142 ./c.out input_file100.txt >> dcollective_results.txt
mpirun -np 10 -hosts 10.7.57.250,10.7.57.142 ./c.out input_file100.txt >> dcollective_results.txt

rm -f dparallel_results.txt
mpirun -np 2 -hosts 10.7.57.250,10.7.57.142 ./p.out input_file100.txt >> dparallel_results.txt
mpirun -np 3 -hosts 10.7.57.250,10.7.57.142 ./p.out input_file100.txt >> dparallel_results.txt
mpirun -np 1 -hosts 10.7.57.250,10.7.57.142 ./p.out input_file100.txt >> dparallel_results.txt 
mpirun -np 4 -hosts 10.7.57.250,10.7.57.142 ./p.out input_file100.txt >> dparallel_results.txt
mpirun -np 5 -hosts 10.7.57.250,10.7.57.142 ./p.out input_file100.txt >> dparallel_results.txt
mpirun -np 6 -hosts 10.7.57.250,10.7.57.142 ./p.out input_file100.txt >> dparallel_results.txt
mpirun -np 7 -hosts 10.7.57.250,10.7.57.142 ./p.out input_file100.txt >> dparallel_results.txt
mpirun -np 8 -hosts 10.7.57.250,10.7.57.142 ./p.out input_file100.txt >> dparallel_results.txt
mpirun -np 9 -hosts 10.7.57.250,10.7.57.142 ./p.out input_file100.txt >> dparallel_results.txt
mpirun -np 10 -hosts 10.7.57.250,10.7.57.142 ./p.out input_file100.txt >> dparallel_results.txt