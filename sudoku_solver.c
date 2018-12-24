#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int arr[10][10][10];
int A[10][10];
int a[729];
void rowunion(int rowidx,int k){
    FILE *fid;
    fid=fopen("sat_input.txt","a");
      for(int j=1;j<10;j++){
           fprintf(fid,"%d ",arr[rowidx][j][k]);
    }
    fprintf(fid,"0\n");
    fclose(fid);
}
void rowintersect(int rowidx){
    FILE *fid;
    fid=fopen("sat_input.txt","a");
    for(int k=1;k<10;k++){
    for(int i=1;i<9;i++){
    	for(int j=i+1;j<10;j++){
             fprintf(fid,"-%d -%d 0\n",arr[rowidx][i][k],arr[rowidx][j][k]);
    	}
    }
}
    fclose(fid);
}
void colintersect(int colidx){
    FILE *fid;
    fid=fopen("sat_input.txt","a");
    for(int k=1;k<10;k++){
    for(int i=1;i<9;i++){
    	for(int j=i+1;j<10;j++){
             fprintf(fid,"-%d -%d 0\n",arr[i][colidx][k],arr[j][colidx][k]);
    	}
    }
}
    fclose(fid);
}
void colunion(int colidx,int k){
    FILE *fid;
    fid=fopen("sat_input.txt","a");
      for(int j=1;j<10;j++){
           fprintf(fid,"%d ",arr[j][colidx][k]);
    }
    fprintf(fid,"0\n");
    fclose(fid);
}
int diagonal(int k)
{
	FILE *fid;
    fid=fopen("sat_input.txt","a");
	for(int i=1;i<10;i++)
	{
		fprintf(fid,"%d ",arr[i][i][k]);
	}
	fprintf(fid,"0\n");
	for(int i=1;i<10;i++)
	{
		fprintf(fid,"%d ",arr[i][10-i][k]);
	}
	fprintf(fid,"0\n");
	for(int i=1;i<9;i++){
    	for(int j=i+1;j<10;j++){
             fprintf(fid,"-%d -%d 0\n",arr[i][i][k],arr[j][j][k]);
    	}
    }
    for(int i=1;i<9;i++){
    	for(int j=i+1;j<10;j++){
             fprintf(fid,"-%d -%d 0\n",arr[i][10-i][k],arr[j][10-j][k]);
    	}
    }
	fclose(fid);
}
int smallsquare(int startrow,int startcol)
{
	FILE *fid;
    fid=fopen("sat_input.txt","a");
	for(int k=1;k<10;k++){
    for(int i=startrow;i<startrow+3;i++){
    	for(int j=startcol;j<startcol+3;j++){
    		fprintf(fid,"%d ",arr[i][j][k]);
    	}
    }
    fprintf(fid,"0\n");
    }
    for(int k=1;k<10;k++){
    for(int i=1;i<10;i++){
    	for(int j=1;j<10;j++){
    		fprintf(fid,"-%d -%d 0\n",arr[startrow+i/4][startcol+i%4][k],arr[startrow+j/4][startcol+j%4][k]);
    	}
    }
}
fclose(fid);
}
int main(){   
   int p=1;
   char x;
   FILE *fid;
   fid=fopen("sat_input.txt","w");
   fclose(fid);
   fid=fopen("sat_input.txt","a");
   fprintf(fid,"c\np cnf 729 2000\n");
   for(int i=1;i<=9;i++){
   	for(int j=1;j<=9;j++){
        scanf("\n%c",&x);
        for(int k=1;k<10;k++){
        arr[i][j][k]=p++;
        a[p-1]=100*i+10*j+k;
        if(x!='.'){
        if(k==x-'0')
        fprintf(fid,"%d 0\n",arr[i][j][k]);
        if(k!=x-'0')
        fprintf(fid,"-%d 0\n",arr[i][j][k]);
        A[i][j]=x-'0';
    }
        if(x=='.')
        	{
        		A[i][j]=0;
        		printf("\n");
        	}    
    }
}
   }
   fclose(fid);
   for(int i=1;i<10;i++){
     for(int k=1;k<10;k++){
       rowunion(i,k);
   }
   }
   for(int i=1;i<10;i++){
     for(int k=1;k<10;k++){
       colunion(i,k);
   }
   }
   for(int i=1;i<10;i++){
   	  rowintersect(i);
   }
   for(int j=1;j<10;j++){
   	  colintersect(j);
   }
   for(int k=1;k<10;k++)
   {
       diagonal(k);
   }
   smallsquare(1,1);
   smallsquare(1,4);
   smallsquare(1,7);
   smallsquare(2,1);
   smallsquare(2,4);
   smallsquare(2,7);
   smallsquare(3,1);
   smallsquare(3,4);
   smallsquare(3,7);
   

   system("minisat sat_input.txt output.txt");

 }