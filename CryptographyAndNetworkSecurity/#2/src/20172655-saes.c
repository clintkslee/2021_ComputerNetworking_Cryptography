//20172655 LEE KANG SAN
#include <stdio.h>
#include <string.h>

void key_expansion(int *key0, int *key1, int *key2);
void input(int (*arr)[8], int *key);
void addkey(int (*arr)[8], int *key);
void nibble_substitution(int (*arr)[8], int inverse);
void shift_row(int (*arr)[8]);
void mix_column(int (*arr)[8], int inverse);
void printarr(int (*arr)[8]);

int s1[4][4]={9, 4, 10, 11, 
	      13, 1, 8, 5, 
	      6, 2, 0, 3, 
	      12, 14, 15, 7}; //s-box 
int s2[4][4]={10, 5, 9, 11, 
              1, 7, 8, 15, 
              6, 0, 2, 3, 
              12, 4, 13, 14}; //ivnerse s-box

int main() {
	int plain[2][8]; //plain = n0 n1 n2 n3
	int key0[16], key1[16], key2[16]; //key0=w0w1, key1=w2w3, key2=w4w5
	//using int type array, assume that each cell represents 1 bit.

	printf("S-AES ENCRPYT and DECRYPT program\n");
	
	/* input a 16 bits length binary plaintext and key. */
	input(plain, key0); //input plain text, key
	/* key expansion */
	key_expansion(key0, key1, key2); //key expansion
///////////////////////////////////////////////	
	printf("\n<< ENCRPYPTION >>\n");
	
	printf("\n[ Round 0 ]\n");
		
	addkey(plain, key0); //xor with key 0
	printf("add key 0  : ");
	printarr(plain);
	
	printf("\n[ Round 1 ]\n");
	
	nibble_substitution(plain, 0); //nibble substitution using s-box
	printf("nibble sub : ");
	printarr(plain);
	
	shift_row(plain); //shift row(2 bottom nibbles, n1 and n3)
	printf("shift row  : "); 
	printarr(plain);
	
	mix_column(plain, 0); //mix column
	printf("mix column : ");
	printarr(plain);
	
	addkey(plain, key1); //xor with key 1
	printf("add key 1  : ");	
	printarr(plain);
	
	printf("\n[ Round 2 ]\n");
	
	printf("nibble sub : ");
	nibble_substitution(plain, 0); //nibble substitution using s-box
	printarr(plain);
	
	shift_row(plain); //shift row(2 bottom nibbles, n1 and n3)
	printf("shift row  : ");
	printarr(plain);	
	
	addkey(plain, key2); //xor with key 2
	printf("add key 2  : ");
	printarr(plain);

	printf("\nCIPHERTEXT : ");
	printarr(plain);
	printf("\n");

	printf("<< DECRPYPTION >>\n");
	
	printf("\n[ Round 0 ]\n");
	
	addkey(plain, key2); //xor with key 2
	printf("add key 2          : ");
	printarr(plain);
	
	printf("\n[ Round 1 ]\n");
	
	shift_row(plain); //inverse shift row(2 bottom nibbles, n1 and n3)
	printf("inverse shift row  : ");
	printarr(plain);
	
	nibble_substitution(plain, 1); //inverse nibble substitution using inverse s-box
	printf("inverse nibble sub : ");
	printarr(plain);
	
	addkey(plain, key1); //xor with key 1
	printf("add key 1          : ");
	printarr(plain);
	
	mix_column(plain, 1); //inverse mix column
	printf("inverse mix column : ");
	printarr(plain);
	
	printf("\n[ Round 2 ]\n");
	
	shift_row(plain); //inverse shift row(2 bottom nibbles, n1 and n3)
	printf("inverse shift row  : ");
	printarr(plain);
	
	nibble_substitution(plain, 1); //inverse nibble substitution using inverse s-box
	printf("inverse nibble sub : ");
	printarr(plain);
		
	addkey(plain, key0); //xor with key 0
	printf("add key 0          : ");
	printarr(plain);
	
	printf("\nDECRYPTED TEXT    : ");
	printarr(plain);
	return 0;
}

void key_expansion(int *key0, int *key1, int *key2) {
//make key1 and key2 by calculating key0
	int rcon1[8]={1,0,0,0,0,0,0,0};
	int rcon2[8]={0,0,1,1,0,0,0,0};
	int w0[8], w1[8], w2[8], w3[8], w4[8], w5[8], temp, a, b, n1, n2;	
	int x1[8], x3[8]; //for rotate and substitute w1, w3

	//get w0, w1 from key0
	for(int i=0; i<8; i++) {
		w0[i]=key0[i];
		x1[i]=w1[i]=key0[i+8];
	}
	
	/* key 1 */
	//x1 = rot(w1)
	for(int i=0; i<4; i++) { 
		temp=x1[i];
		x1[i]=x1[i+4];
		x1[i+4]=temp;
	}	
	//x1 = sub(rot(w1))
	a=x1[0]; a=a<<1; a=a|x1[1];
	b=x1[2]; b=b<<1; b=b|x1[3];
	n1=s1[a][b];
	a=x1[4]; a=a<<1; a=a|x1[5];
	b=x1[6]; b=b<<1; b=b|x1[7];
	n2=s1[a][b]; //get nible value from s-box
	
	//write binary format in x1 array
	for(int i=3; i>=0; i--) {
		x1[i]=n1;
		x1[i]=x1[i]&1;
		n1=n1>>1;
	}
	for(int i=7; i>=4; i--) {
		x1[i]=n2;
		x1[i]=x1[i]&1;
		n2=n2>>1;
	}	
	
	//get w2 = w1 ^ rcon(1) ^ sub(rot(w1))
	for(int i=0; i<8; i++)
		w2[i]=w0[i]^rcon1[i]^x1[i]; 

	//get w3 = w2 ^ w1
	for(int i=0; i<8; i++)
		w3[i]=w2[i]^w1[i];
		
	//get key 1
	for(int i=0; i<8; i++) {
		key1[i]=w2[i];
		key1[i+8]=w3[i];
	}
//////////////////////////////////////////////
	/* key 2 */		

	//get w3
	for(int i=0; i<8; i++)
		x3[i]=w3[i];
		
	//x3 = rot(w3)
	for(int i=0; i<4; i++) {
		temp=x3[i];
		x3[i]=x3[i+4];
		x3[i+4]=temp;
	}
	
	//x3 = sub(rot(w3))
	a=x3[0]; a=a<<1; a=a|x3[1];
	b=x3[2]; b=b<<1; b=b|x3[3];
	n1=s1[a][b];
	a=x3[4]; a=a<<1; a=a|x3[5];
	b=x3[6]; b=b<<1; b=b|x3[7];
	n2=s1[a][b]; //get nible value from s-box
	
	//write binary format in x3 array
	for(int i=3; i>=0; i--) {
		x3[i]=n1;
		x3[i]=x3[i]&1;
		n1=n1>>1;
	}
	for(int i=7; i>=4; i--) {
		x3[i]=n2;
		x3[i]=x3[i]&1;
		n2=n2>>1;
	}
	
	//get w4 = w2 ^ rcon(2) ^ sub(rot(w3))	
	for(int i=0; i<8; i++)
		w4[i]=w2[i]^rcon2[i]^x3[i];

	//get w5 = w4 ^ w3
	for(int i=0; i<8; i++)
		w5[i]=w4[i]^w3[i];
		
	//get key 2
	for(int i=0; i<8; i++) {
		key2[i]=w4[i];
		key2[i+8]=w5[i];
	}
	printf("\n<< Key Expansion >>\n");
	printf("key0 : ");
	for(int i=0; i<16; i++) {
		printf("%d", key0[i]);
		if(i%4==3)
			printf(" ");
	}
	printf("\n");
	printf("key1 : ");
	for(int i=0; i<16; i++) {
		printf("%d", key1[i]);
		if(i%4==3)
			printf(" ");
	}
	printf("\n");
	printf("key2 : ");
	for(int i=0; i<16; i++) {
		printf("%d", key2[i]);
		if(i%4==3)
			printf(" ");
	}
	printf("\n");
}
void input(int (*arr)[8], int *key) { 
//input function
	char input[50];
	int trim[16], n=0;
	printf("\ninput a 16 bits binary plaintext.\n");
	printf(">> ");
	fgets(input, 49, stdin);
	
	//remove white space
	for(int i=0, j=0; j<16; i++)
		if(input[i]=='0' || input[i]=='1')
			trim[j++]=input[i]-'0';
	
	//set plain text = N0 N1 N2 N3
	for(int i=0; i<4; i++) //N0
		arr[0][i]=trim[n++];
	for(int i=0; i<4; i++) //N1
		arr[1][i]=trim[n++];
	for(int i=4; i<8; i++) //N2
		arr[0][i]=trim[n++];
	for(int i=4; i<8; i++) //N3
		arr[1][i]=trim[n++];

	printf("input a 16 bits binary key.\n");
	printf(">> ");
	fgets(input, 49, stdin);
	
	//remove white space
	for(int i=0, j=0; j<16; i++)
		if(input[i]=='0' || input[i]=='1')
			trim[j++]=input[i]-'0';
			
	//set key0
	for(int i=0, n=0; i<16; i++)
			key[i]=trim[n++];
}

void addkey(int (*arr)[8], int *key) {
//xor with key 16 bit key
	int n=0;
	//N0
	for(int i=0; i<4; i++)
		arr[0][i]=arr[0][i]^key[n++];
	//N1
	for(int i=0; i<4; i++)
		arr[1][i]=arr[1][i]^key[n++];	
	//N2
	for(int i=4; i<8; i++)
		arr[0][i]=arr[0][i]^key[n++];	
	//N3
	for(int i=4; i<8; i++)
		arr[1][i]=arr[1][i]^key[n++];	
}
void nibble_substitution(int (*arr)[8], int inverse) {
//using nibble bits as a index of s-box, substitute with the s-box value.
	int (*sbox)[4];
	int nib[4];
	int i, j, temp;
	
	//select sbox
	if(inverse==0) sbox=s1;
	else if(inverse==1) sbox=s2;

	//get nibble value
	for(int k=0, m=0, n=0; k<4; k++)
	{
		if(k==2) {n=0; m++;}
		temp=0;
		temp=temp|arr[m][n++];
		temp=temp<<1;
		temp=temp|arr[m][n++];
		i=temp;
		
		temp=0;
		temp=temp|arr[m][n++];
		temp=temp<<1;
		temp=temp|arr[m][n++];
		j=temp;
		nib[k]=sbox[i][j];
	}

	//substitution(calc bit by bit)
	for(int i=3; i>=0; i--) {
		arr[0][i]=nib[0];
		arr[0][i]=arr[0][i]&1;
		nib[0]=nib[0]>>1;
	}
	for(int i=7; i>=4; i--) {
		arr[0][i]=nib[1];
		arr[0][i]=arr[0][i]&1;
		nib[1]=nib[1]>>1;
	}
	for(int i=3; i>=0; i--) {
		arr[1][i]=nib[2];
		arr[1][i]=arr[1][i]&1;
		nib[2]=nib[2]>>1;
	}
	for(int i=7; i>=4; i--) {
		arr[1][i]=nib[3];
		arr[1][i]=arr[1][i]&1;
		nib[3]=nib[3]>>1;
	}
}
void shift_row(int (*arr)[8]) {
//shift the bottom nibbles, n1 and n3
	int temp;
	for(int i=0; i<4; i++) {
		temp=arr[1][i];
		arr[1][i]=arr[1][i+4];
		arr[1][i+4]=temp;
	}
}
void mix_column(int (*arr)[8], int inverse) {
	int s00=0, s01=0, s10=0, s11=0;
	int mul2[16]={0,2,4,6,8,10,12,14,3,1,7,5,11,9,15,13};
	int mul4[16]={0,4,8,12,3,7,11,15,6,2,14,10,5,1,13,9};
	int mul9[16]={0,9,1,8,2,11,3,10,4,13,5,12,6,15,7,14};

	for(int i=0; i<4; i++) { //get s00, s10
		s00=s00<<1;
		s00=s00|arr[0][i];
		s10=s10<<1;
		s10=s10|arr[1][i];
	}
	for(int i=4; i<8; i++) { //get s01, s11
		s01=s01<<1;
		s01=s01|arr[0][i];
		s11=s11<<1;
		s11=s11|arr[1][i];
	}
	//hear	
	int x00, x01, x10, x11;
	if(inverse==0) { //calculate s'00, s'01, s'10, s'11
		x00=s00^mul4[s10];
		x10=mul4[s00]^s10;
		x01=s01^mul4[s11];
		x11=mul4[s01]^s11;
	}
	else if(inverse==1) { //calculate when inverse
		x00=mul9[s00]^mul2[s10];
		x10=mul2[s00]^mul9[s10];
		x01=mul9[s01]^mul2[s11];
		x11=mul2[s01]^mul9[s11];
	}
	//
	for(int i=3; i>=0; i--) { //put s'00 into arr 
		arr[0][i]=x00;
		arr[0][i]=arr[0][i]&1;
		x00=x00>>1;
	}
	for(int i=7; i>=4; i--) { //put s'01 into arr 
		arr[0][i]=x01;
		arr[0][i]=arr[0][i]&1;
		x01=x01>>1;
	}
	for(int i=3; i>=0; i--) { //put s'10 into arr 
		arr[1][i]=x10;
		arr[1][i]=arr[1][i]&1;
		x10=x10>>1;
	}
	for(int i=7; i>=4; i--) { //put s'11 into arr 
		arr[1][i]=x11;
		arr[1][i]=arr[1][i]&1;
		x11=x11>>1;
	}
}
void printarr(int (*arr)[8]) {
//print current state to see the process of s-aes.
	for(int i=0; i<4; i++) //N0
		printf("%d", arr[0][i]);
	printf(" ");
	for(int i=0; i<4; i++) //N1
		printf("%d", arr[1][i]);
	printf(" ");
	for(int i=4; i<8; i++) //N2
		printf("%d", arr[0][i]);
	printf(" ");		
	for(int i=4; i<8; i++) //N3
		printf("%d", arr[1][i]);
	printf("\n");
}














	
