#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

char buf[500][99];
char cmd[99];
char ecmd[99];

int  elinefix = FALSE;
int  emode = FALSE;
int  eline = 0;
int  lastl = 0;

void editmode() {
	while(emode) {
		printf("0x%0.3x < ",eline);
		fgets(ecmd,99,stdin);
		if (strcmp(ecmd,"q\n") == 0) {
			emode = FALSE;
			return;
		}
		else {
			strcpy(buf[eline],ecmd);
			if (eline > lastl) {
				lastl = eline;
			}
			eline++;
		}
	}
}

int main()
{
	printf("teddy (Tommy's EDitor.)\n");
	while(1) {
		printf("teddy> ");
		fgets(cmd,99,stdin);
		if (strcmp(cmd,"?\n") == 0) {
			printf("the 'teddy>' prompt is a single character prompt.\n");
			printf("e = enter edit mode\n");
			printf("l = list buffer\n");
			printf("g = goto line (in hex!)\n");
			printf("w = write out to file\n");
			printf("o = open from filebuffer\n");
			printf("q = exit edit mode\n");
			printf("\n? = show this helptext\n");
			printf("this is teddy (Tommy's EDitor)!\n");
		}
		if (strcmp(cmd,"e\n") == 0) {
			strcpy(cmd,"");
			emode = TRUE;
			editmode(); 
		}
		if (strcmp(cmd,"l\n") == 0) {
			strcpy(cmd,"");
			int le = 0;
			for (int lb = 0; lb < lastl+1; lb++) {
				printf("0x%0.3x > %s",lb,buf[lb]);
				le = lb;	
			}
			printf("End of buffer @ %0.3x\n",le);
		}
		if (strcmp(cmd,"g\n") == 0) {
			char goline[12];
			printf("goto line: ");
			scanf("%s",goline);
			eline = (int)strtol(goline,NULL,16);
		}
		if (strcmp(cmd,"w\n") == 0) {
			int blen = 0;
			char fname[99];
			
			eline = 0;

			printf("writeout to file: ");
			fgets(fname,99,stdin);
			fname[strlen(fname)-1] = 0;

			FILE *fp = fopen(fname, "w");
			for (int wbl = 0; wbl < lastl+1; wbl++) {
				fputs(buf[wbl],fp);
				blen += strlen(buf[wbl]);
			}
			fclose(fp);
			printf("wrote %0.4x bytes\n",blen);
		}
		if (strcmp(cmd,"o\n") == 0) {
			int rlen = 0;
			int rlines = 0;
			char rbuf[99];
			char fname[99];

			lastl = 0;
			eline = 0;

			printf("read from file: ");
			fgets(fname,99,stdin);
			fname[strlen(fname)-1] = 0;

			FILE *fp = fopen(fname,"r");
			while (!feof(fp)) {
				fgets(rbuf,99,fp);
				if (feof(fp)) {
					goto doneread;
				}
				rlen += strlen(rbuf);
				strcpy(buf[rlines],rbuf);
				rlines++;
				if (rlines > lastl) {
					lastl = rlines;
				}
			}
doneread:
			fclose(fp);
			lastl--;

			printf("read %0.4x bytes\n",rlen);			
		}
	}
}
