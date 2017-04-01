
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE	1000

int main( int argc, char* argv[ ] ){
	int i;
	int number_of_points;
	long random_num;

	if( argc > 1 )
	{
		number_of_points = atoi( argv[ 1 ] );
	}
	else
	{
		i = scanf("%d", &number_of_points);
	}

	printf("%d\n",number_of_points);
	for(i=0;i<number_of_points;i++){
		random_num = random();
		printf("%ld\n", (random_num % MAX_SIZE));
	}

	return 0;
}
