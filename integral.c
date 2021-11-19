#include <stdio.h>
#include <omp.h>

static long num_paso = 100000000; 
double paso;
#define NUM_THREADS 32	




void main(int argc, char const *argv[])
{
	int i, nthrads; double pi, sum[NUM_THREADS];
	paso = 1.0/num_paso; 
	omp_set_num_threads(NUM_THREADS);
	const double start = omp_get_wtime();
	#pragma omp parallel
	{
		int i, id, nthrds; 
		double x;
		id = omp_get_thread_num(); 
		nthrds = omp_get_num_threads();
		if (id == 0)
		{
			nthrads = nthrds;
		}
		for (i = id, sum[id] = 0.0; i < num_paso;i=i+nthrds)
		{
			x = (i+0.5)*paso;
			sum[id]+=4.0/(1.0+x*x);
		}
	}
	for (i=0,pi=0.0;i<nthrads;i++) {
		pi+=sum[i]*paso;
	}
	const double end = omp_get_wtime();
	printf("pi = (%lf)\n", pi);
	printf("tomo (%lf) segundos\n", (end - start));
}