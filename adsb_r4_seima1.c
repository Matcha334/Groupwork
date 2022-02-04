#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50000

//簡単なマクロ
#define less(A, B) (A < B)
#define exch(A, B) { Item t = A; A = B; B = t; } 
#define compexch(A, B) if (less(B, A)) exch(A, B)
#define eq(A, B) (A == B)
#define min(A,B,C) (A>B ? (B>C ? C : B) : (A>C ? C : A))


//編集距離を求める関数
int Distance(char *a,char *b,int min,int max){
	int len=max-min+1;
	int i,j,k;

    //行列の動的確保
    int *matrix;
    matrix = malloc(sizeof(int)*len*len);
    
	for(i=0;i<len;i++){
        matrix[i*len]=i;
    }
    for(j=1;j<len;j++){
        matrix[j]=j;
    }

	for (i=1;i<len;i++) {
		for (j=1;j<len;j++) {
			k=(a[i-1+min]==b[j-1+min]?0:1);
			matrix[i*len+j]=min(matrix[(i-1)*len+j]+1,matrix[i*len+j-1]+1,matrix[(i-1)*len+j-1]+k);
		}
	}
    printf("finish\n");
    int ans=matrix[(len-1)*len+len-1];

    //メモリ解放
    free(matrix);
	return ans;
}

int main(int argc, char const *argv[])
{
    // ファイル入力
    // ファイル階層は各自で合わせてください
    FILE *ifile = fopen("Model1/dat0/idata","r");
    FILE *ofile = fopen("Model1/dat0/odata","r");
    if (ifile == NULL || ofile == NULL)
    {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }
    
    // idata, odata読込
    char x[N], y[N];
    fgets(x, N + 1, ifile);
    fscanf(ifile, "%c", y); // 改行削除
    fgets(y, N + 1, ifile);
    char s[N * 2], l[N * 2];
    fgets(s, N * 2, ofile);
    fgets(l, N * 2, ofile);

    // 改行を消す
    s[strlen(s) - 1] = '\0';
    l[strlen(l) - 1] = '\0';

    //pos:入れ替わった位置, err:間違いの数
    int pos = 0, err = 0;

    /* ここにプログラムを書く */

    int L=0,R=N-1,M;
    while(L<R){
        M=(L+R)/2;
        if(Distance(x,s,L,M)+Distance(y,l,L,M)<Distance(x,s,M+1,R)+Distance(y,l,M+1,R)){
            R=M;
        }else{
            L=M+1;
        }
        printf("half\n");
    }
    pos=L;
    int end=min(strlen(s),strlen(l),N);//最も短い配列の長さ
    printf("%d\n",end);
    printf("s=%d\n",(int)strlen(s));
    printf("l=%d\n",(int)strlen(l));
    printf("%d,%d\n",pos,err);//検証用出力
    err=Distance(x,s,0,pos)+Distance(y,l,0,pos)+Distance(x,l,pos+1,end)+Distance(y,s,pos+1,end);

    printf("%d,%d\n",pos,err);//検証用出力

    /*検証のため一旦封印

    // test
    printf("%s\n", x);
    printf("%s\n", y);
    printf("%s\n", s);
    printf("%s\n", l);

    // ファイル出力
    // 元のanswerと同階層で出力する際は上書きに注意
    FILE *afile = fopen("my answer", "w");
    if (afile == NULL)
    {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }
    fprintf(afile, "%d,%d\n", pos, err);

    // 各ファイルをクローズする
    fclose(ifile);
    fclose(ofile);
    fclose(afile);

    */

    return 0;
}
