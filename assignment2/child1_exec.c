#include <stdio.h>
#include <unistd.h>

int main(void)
{
	FILE *fp;

	for (int i=0;i<12;i++) {

		sleep(1);
		fp=fopen("test.txt","w");
		fprintf(fp,"%d\n",i);
		fclose(fp);

		printf("Child1 wrote %d.\n",i);
	}

	return 0;
}
