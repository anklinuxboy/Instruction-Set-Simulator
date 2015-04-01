#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct store
{
	int linenumber;
	char instruc[10];
	char dest[10];
	char src[10];
}iss[100];

int main(int argc, char *argv[])
{  
 	int mv,mvreg;
	int8_t localmem[256][2];
	memset(localmem, 0, sizeof(localmem[0][0]) * 256 * 2);
	char line[100];
	int8_t r[6] = {0,0,0,0,0,0};
	int i,j;
	int clok=0;
	int lscount =0;
	int numhit=0;	
	int cmp,jnum;
	int count=0;
	int exec=0;
	FILE *f;
	f = fopen(argv[1],"r");
	if (f!=NULL)
	{
		while(fgets(line,100,f)!=NULL)
		{
			sscanf(line,"%d %s %s %s", &iss[count].linenumber, iss[count].instruc, iss[count].dest, iss[count].src);
			count ++;
		}
		fclose(f);
	}
	else
	{
		printf("error");
	}
	printf("Total number of instructions in the code: %d\n",count);	
	for(i=0; i<count; i++)
	{		
		if (iss[i].instruc[0]=='S')
		{
			iss[i].dest[0] = iss[i].dest[1];
			iss[i].dest[1] = iss[i].dest[2];
			iss[i].dest[2] = '\0';
		}	
		else if (iss[i].instruc[0]=='L')
		{
			iss[i].dest[2] = '\0';
			iss[i].src[0] = iss[i].src[1];
			iss[i].src[1] = iss[i].src[2];
			iss[i].src[2] = '\0';
		}
		else if (iss[i].instruc[0]!='J')
		{			
			iss[i].dest[2] = '\0';
		}
	}

	for (j=0; j<count; j++)
	{
		if (iss[j].instruc[0]=='M')
		{
			//printf("im in MOV\n");
			mv = atoi(iss[j].src);
			mvreg = iss[j].dest[1] - '0';
			r[mvreg-1] = mv;
			exec++;
			clok++;
		}
		else if (iss[j].instruc[0]=='A')
		{
			//printf("im in ADD\n");
			int ad1;
			int adreg1,adreg2;
			if (iss[j].src[0]=='R')
			{
				adreg1 = iss[j].dest[1] - '0';
				adreg2 = iss[j].src[1] - '0';
				r[adreg1-1] += r[adreg2-1];
				exec++;
				clok++;
			}
			else
			{
				//printf("source is not R\n");
				ad1 = atoi(iss[j].src);
				adreg1 = iss[j].dest[1] - '0';
				r[adreg1 - 1] += ad1;
				exec++;
				clok++;
			}
		}
		else if (iss[j].instruc[0]=='C')
		{			
			//printf("Im in CMP\n");
			int creg1,creg2;
			creg1 = iss[j].dest[1] - '0';
			creg2 = iss[j].src[1] - '0';
			cmp = r[creg1-1] - r[creg2-1];
			exec++;
			clok += 2;
		}
		else if (iss[j].instruc[0]=='J')
		{
			//printf("Im in Jump\n");
			if (iss[j].instruc[1]=='E')
			{
				if (cmp == 0)
				{
					jnum = atoi(iss[j].dest);
					j = jnum - iss[0].linenumber - 1;
				}
				exec++;
				clok++;
			}
			else if (iss[j].instruc[1]=='M')
			{
				jnum = atoi(iss[j].dest);
				j = jnum - iss[0].linenumber - 1;
				exec++;
				clok++;
			}
		}
		else if (iss[j].instruc[0]=='S')
		{
			//printf("im in STORE\n");
			int sregs,sregd;
			sregs = iss[j].src[1] - '0';
			sregd = iss[j].dest[1] - '0';
			if (localmem[r[sregd-1]][0] == 1)
			{
				localmem[r[sregd-1]][1] = r[sregs-1];
				numhit++;
				clok +=3;
				exec++;
				lscount++;				
			}
			else
			{
				localmem[r[sregd-1]][1] = r[sregs-1];
				exec++;
				clok += 50;
				lscount++;
				localmem[r[sregd-1]][0] = 1;
			}		
		}
		else if (iss[j].instruc[0]=='L')
		{
			//printf("in in LOAD\n");
			int lregs,lregd;
			lregs = iss[j].src[1] - '0';
			lregd = iss[j].dest[1] - '0';
			if (localmem[r[lregs-1]][0] == 1)
			{
				r[lregd-1] = localmem[r[lregs-1]][1];
				numhit++;
				exec++;
				clok += 3;
				lscount++;
			}
			else
			{
				r[lregd-1] = localmem[r[lregs-1]][1];
				exec++;
				clok += 50;	
				lscount++;
				localmem[r[lregs-1]][0] = 1;
			}
		}
	}
	printf("Total number of executed instructions: %d\n", exec);
	printf("Total number of clock cycles: %d\n",clok);
	printf("Number of hits to local memory: %d\n",numhit);
	printf("Total number of executed LD/ST instructions: %d\n",lscount);
	return 0;
}
