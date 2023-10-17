//20172655 LEE KANG SAN
//caesar.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char** argv) {
	if(argc!=3) {
		printf("Usage : %s (datafile) (key)\n", argv[0]);
		exit(1);
	}
	
	FILE *fp1, *fp2;
	int key=atoi(argv[2]);
	int c;
	char outfile[100]="decrypted";
	strcat(outfile, argv[1]);
	fp1=fopen(argv[1], "r");
	fp2=fopen(outfile, "w");

	while((c=fgetc(fp1))!=EOF) {
		if('A'<=c&&c<='Z')
			c=(c-key<'A')?26+(c-key):(c-key);
		fputc(c, fp2);
	}

	fclose(fp1);
	fclose(fp2);
	return 0;
}

