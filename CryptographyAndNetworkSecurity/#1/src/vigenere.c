//20172655 LEE KANG SAN
//vigenere.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char** argv) {
	if(argc!=3) {
		printf("Usage : %s (datafile) (key)\n", argv[0]);
		exit(1);
	}
	
	FILE *fp1, *fp2;
	int c, i=0;
	int keylength=strlen(argv[2]);
	char outfile[100]="decrypted";
	
	strcat(outfile, argv[1]);
	fp1=fopen(argv[1], "r");
	fp2=fopen(outfile, "w");
	
	while((c=fgetc(fp1))!=EOF) {
		if('A'<=c&&c<='Z') {
			c=c-argv[2][i];
			c=c<0?'A'+26+c:'A'+c;
		}
		fputc(c, fp2);
		i=(i+1)%keylength;
	}
	
	fclose(fp1);
	fclose(fp2);
	return 0;
}

