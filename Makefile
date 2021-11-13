all:
	mpicc -o s.out sequential.c gaussian.c -Wall
	mpicc -o p.out parallel.c gaussian.c -Wall
	mpicc -o c.out collective.c gaussian.c -Wall