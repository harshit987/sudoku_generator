#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Create a SUDOKU+ Solver and SUDOKU+ puzzle generator
//Shashank Kumar 170652
//Harshit Kumar 170293

int ARR[10][10][10]; //arr[i][j][k] denotes the logical proposition that (i, j) entry in sudoku matrix has value k
void exactlyone(int i,int j) //Creates clauses to ensure (i, j) entry of sudoku matrix is assigned one and only one (unique) value from the set [1,9]
{
    FILE *fid;
    fid=fopen("sat_input.txt","a");
    for(int p=1; p<10; p++)
        fprintf(fid,"%d ",ARR[i][j][p]);
    fprintf(fid,"0\n");
    for(int p=1; p<9; p++)
        for(int q=p+1; q<10; q++)
            fprintf(fid,"-%d -%d 0\n",ARR[i][j][p],ARR[i][j][q]);
    fclose(fid);
}
void rowintersect(int rowidx) //Creates clauses to ensure that no two entries in the row with index "rowidx" have the same value
{
    FILE *fid;
    fid=fopen("sat_input.txt","a");
    for(int k=1; k<10; k++) {
        for(int i=1; i<9; i++) {
            for(int j=i+1; j<10; j++) {
                fprintf(fid,"-%d -%d 0\n",ARR[rowidx][i][k],ARR[rowidx][j][k]);
            }
        }
    }
    fclose(fid);
}
void colintersect(int colidx) //Creates clauses to ensure that no two entries in the column with index "colidx" have the same value
{
    FILE *fid;
    fid=fopen("sat_input.txt","a");
    for(int k=1; k<10; k++) {
        for(int i=1; i<9; i++) {
            for(int j=i+1; j<10; j++) {
                fprintf(fid,"-%d -%d 0\n",ARR[i][colidx][k],ARR[j][colidx][k]);
            }
        }
    }
    fclose(fid);
}
int diagonal(int k) //Creates clauses to ensure that no two entries in a body diagonal have the same value
{
    FILE *fid;
    fid=fopen("sat_input.txt","a");
    for(int i=1; i<9; i++) {
        for(int j=i+1; j<10; j++) {
            fprintf(fid,"-%d -%d 0\n",ARR[i][i][k],ARR[j][j][k]);
        }
    }
    for(int i=1; i<10; i++) {
        for(int j=i+1; j<10; j++) {
            fprintf(fid,"-%d -%d 0\n",ARR[i][10-i][k],ARR[j][10-j][k]);
        }
    }
    fclose(fid);
}
int smallsquare(int startrow,int startcol) //Creates clauses to ensure that no two entries in "the 3 x 3 block matrix having topleft corner (startrow, startcol)" have same value
{
    FILE *fid;
    fid=fopen("sat_input.txt","a");
    for(int k=1; k<10; k++) {
        for(int i=0; i<9; i++) {
            for(int j=i+1; j<9; j++) {
                fprintf(fid,"-%d -%d 0\n",ARR[startrow+i/3][startcol+i%3][k],ARR[startrow+j/3][startcol+j%3][k]);
            }
        }
    }
    fclose(fid);
}
void solve() //This function creates the input code (having all required constraints) for minisat SAT solver. It writes the code to the file "sat_input.txt".
{
    char x;
    int p=1;
    FILE *fid;
    FILE *fid2;
    fid2=fopen("in.txt","r");
    fid=fopen("sat_input.txt","w"); //sat_input.txt will be the input file for minisat
    fprintf(fid,"c\np cnf 729 20000\n"); //Even if the actual number of clauses that are used is less than this number, minisat won't give any error
    for(int i=1; i<=9; i++)
    {
        for(int j=1; j<=9; j++)
        {
            do
            {
                x = fgetc(fid2);
            } while(x==' ' || x=='\n');
            for(int k=1; k<10; k++)
            {
                ARR[i][j][k]=p++;
                if(x!='.') //Following loop ensures that the values of "fixed" entries are not modified
                {
                    if(k==x-'0')
                        fprintf(fid,"%d 0\n",ARR[i][j][k]);
                    if(k!=x-'0')
                        fprintf(fid,"-%d 0\n",ARR[i][j][k]);
                }
            }
        }
    }
    fclose(fid);
    for(int i=1; i<10; i++) {
        for(int k=1; k<10; k++) {
            exactlyone(i,k);
        }
    }
    for(int i=1; i<10; i++) {
        rowintersect(i);
    }
    for(int j=1; j<10; j++) {
        colintersect(j);
    }
    for(int k=1; k<10; k++)
    {
        diagonal(k);
    }
    fid=fopen("sat_input.txt","a");
    fclose(fid);
    smallsquare(1,1);
    smallsquare(1,4);
    smallsquare(1,7);
    smallsquare(4,1);
    smallsquare(4,4);
    smallsquare(4,7);
    smallsquare(7,1);
    smallsquare(7,4);
    smallsquare(7,7);
}

int check_and_put_sol() {  //This function reads the minisat output from the file "output".txt and translates it into SUDOKU solution. It stores the sudoku in the file "answer.txt"
    int rem;
    char x;
    FILE *f;
    f = fopen("output.txt","r");
    char ch = fgetc(f);
    fclose(f);
    if(ch == 'S') {
        FILE *fid3;
        FILE *fid4;
        fid3 = fopen("answer.txt","w"); //answer.txt will store the sudoku solution
        fid4 = fopen("output.txt","r");
        int t = 0;
        for(int i = 1; i<=729; i++)
        {
            do
            {
                x = fgetc(fid4);
            }
            while(x!=' ' && x!= '\n');
            x = fgetc(fid4);
            if(x !='-')
            {
                t++;
                rem = i%9;
                fprintf(fid3,"%d ",rem==0?9:rem);
                if(t % 9 == 0)
                    fprintf(fid3,"\n");
            }
        }
        fclose(fid3);
        fclose(fid4);
        return 1;
    }
    else
        return 0;
}
int check_unique(int* arr) //This function determines whether the SUDOKU+ puzzle stored in the array arr has "unique solution", "no solution" or "more than one solution"
{
    int x;
    solve();
    system("minisat sat_input.txt output.txt 1> /dev/null 2> /dev/null"); //This command invokes the minisat SAT solver
    if(check_and_put_sol()) {
        FILE* fid2;
        fid2 = fopen("answer.txt","r");
        FILE* fid1;
        fid1 = fopen("sat_input.txt","a"); //In the subsequent piece of code, we are adding more restrictions to input code of minisat, so that to check for a solution different from the one obtained earlier
        for(int i=1; i<=9; i++) {
            for(int j=1; j<=9; j++)
            {
                do
                {
                    x = fgetc(fid2);
                } while(x==' ' || x=='\n');
                fprintf(fid1,"-%d ",ARR[i][j][x-'0']);
            }
        }
        fprintf(fid1,"0\n");
        fclose(fid1);
        fclose(fid2);
        system("minisat sat_input.txt output.txt 1> /dev/null 2> /dev/null");
        if(check_and_put_sol()) //checks for second solution
        {
            return 0; //puzzle has more than one solution
        }
        else
        {
            return 1; //puzzle has unique solution
        }
    }
    else
    {
        return -1; //puzzle has no solution
    }
}
void copy(int*C2) { //function translates the SUDOKU+ puzzle from array format to the requisite format in the "in.txt" file
    FILE  *in;
    in = fopen("in.txt","w");
    for(int i = 0; i<9; i++)
    {
        for(int j = 0; j<9; j++)
        {
            if(*(C2 + 9*i + j) == 0)
                fprintf(in,"%c%c",'.',j==8?'\n':' ');
            else
                fprintf(in,"%d%c",*(C2 +9*i + j),j==8?'\n':' ');
        }
    }
    fclose(in);
}
int main() {
    printf("Choose the appropriate option (1 or 2):\n");
    printf("1.Solve SUDOKU+ puzzle\n2.Generate minimal SUDOKU+ puzzle having unique solution\n");
    int choice;
    char x;
    scanf("%d",&choice);
    switch(choice) {
    case 1:
    {
        solve();
        system("minisat sat_input.txt output.txt 1> /dev/null 2> /dev/null");
        if(check_and_put_sol())
        {
            printf("\nA solution to the puzzle given in \"in.txt\" file is:\n\n");
            FILE *ans;
            ans = fopen("answer.txt","r");
            for(int i=0; i<9; i++)
                for(int j=0; j<9; j++) {
                    do
                    {
                        x = fgetc(ans);
                    } while(x==' ' || x=='\n');
                    printf("%c%c",x,j==8?'\n':' ');
                }
            fclose(ans);
            printf("\nSolution has been saved in \"answer.txt\"\n");
        }
        else
            printf("\nNo solution to given SUDOOU+ puzzle exists\n");
    }
    break;
    case 2:
    {
        int u,temp,m,flag = 0, added;
        int*ar;
        char ch;
        FILE* sud;
        int*C1 = (int*)malloc(sizeof(int) * 81);
        int*C2 = (int*)malloc(sizeof(int) * 81);
        sud = fopen("sudoku.txt","r"); 
//The file "sudoku.txt" has a pre solved sudoku in it. We will use this SUDOKU to create random SUDOKU+ puzzles ( but all will have the same solution). 
        for(int i = 0; i< 9; i++)
        {
            for(int j =0; j<9; j++)
            {
                do
                {
                    ch = fgetc(sud);
                    *(C1+9*i + j) = ch -'0';
                    *(C2+9*i + j) = ch-'0';
                } while(ch=='\n'||ch==' ');
            }
        }
        while(1)
        {
            srand(time(0));
            do
            {
                temp = 9*(rand()%9) + (rand()%9);
            } while(*(C2 + temp) == 0);  //This loop and the  next line randomly remove an element from the sudoku in C2
            *(C2 + temp) = 0;
            copy(C2);
            u = check_unique(C2); //Check status of current sudoku in C2

//if it does not have a unique solution (which means more than one solution here) we enter the next if statement, otherwise the while loop reiterates. In the susbsequent code we put back the removed element removed and one by one keep removing andn putting back various nonzero elements of sudoku until we get a sudoku with a unique solution. If no such sudoku is found, this means we have reached the minimal sudoku ( that also has a unique solution ) because removing any one element creates multiple solutions.

            if(u!=1)
            {
                *(C2 + temp) = *(C1 + temp);
                int flag = 0;
                for(int a = 0; a< 9; a++)
                {
                    for(int b = 0; b<9; b++)
                    {
                        if(*(C2 + 9*a + b) != 0)
                        {
                            *(C2 + 9*a + b) = 0;
                            copy(C2);
                            u = check_unique(C2);
                            if(u==1)
                            {

                                flag = 1; //a sudoku with a unique solution found on removing some element
                                break;
                            }
                            if(u!=1)
                            {

                                *(C2 + 9*a + b) = *(C1 + 9*a + b);
                            }
                        }
                        if(flag ==1) //while loop reiterates
                            break;
                    }
                    if(flag ==1) //while loop reiterates
                        break;

                }

                if(flag == 0) //minimal solution has been reached
                {
                    copy(C2);
                    break;
                }
            }

        }
        printf("\nA minimal SUDOKU+ puzzle having unique solution is:\n\n");
        for(int i=0; i<9; i++)
            for(int j=0; j<9; j++)
                printf("%c%c",*(C2 + 9*i + j)==0?'.':*(C2 + 9*i + j)+'0',j==8?'\n':' ');
        printf("\nPuzzle has been saved in \"in.txt\"\n");
    }
    }
}
