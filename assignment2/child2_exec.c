#include <stdio.h>
#include <unistd.h>

int main(void)
{
	FILE *fp;
	for (int i=0;i<6;i++) {
		sleep(2);

		fp=fopen("test.txt","r");
		char str[10];
		fseek(fp,0,SEEK_SET);
		fgets(str,sizeof(str),fp);
		fclose(fp);

		printf("%s",str);
	}
	return 0;
}
