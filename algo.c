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
unsigned short Distance(char *a,char *b,unsigned short min,unsigned short max){
	unsigned short len=max-min+1;
	unsigned short i,j,k;

    //行列の動的確保
    unsigned short *matrix;
    matrix = malloc(sizeof(unsigned short)*len*len);
    if(matrix==NULL){
        i=888;
    }
    
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
    unsigned short ans=matrix[(len-1)*len+len-1];

    //メモリ解放
    free(matrix);
	return ans;
}

int main(int argc, char const *argv[])
{
    // ファイル入力
    // ファイル階層は各自で合わせてください
    FILE *ifile = fopen(argv[1], "r");
    FILE *ofile = fopen(argv[2], "r");
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
    unsigned short pos = 0, err = 0;

    fclose(ifile);
    fclose(ofile);


    /* ここにプログラムを書く */
    
    unsigned short L=0,R=N-1;
    unsigned short mll = L;
    unsigned short mrr = R;
    unsigned short mc = (mll+mrr)/2;
    unsigned short ml = (mll+mc)/2;
    unsigned short mr = (mc+mrr)/2;
 
    while(ml<mc&&mc<mr){ 
        double Edl = (double)Distance(x,s,L,ml)/(ml+1); //第1四分位数の平均編集距離
        double Edc = (double)Distance(x,s,L,mc)/(mc+1); //中央の平均編集距離
        double Edr = (double)Distance(x,s,L,mr)/(mr+1); //第3四分位数の平均編集距離
   
        if(min(Edl,Edr,Edc)==Edl){
            mrr = mc; // mll不変
            mc = ml;
            ml = (mll+mc)/2;
            mr = (mc+mrr)/2;
        }else if(min(Edl,Edr,Edc)==Edc){
            mll = ml;
            mrr = mr; // mc不変
            ml = (mll+mc)/2;
            mr = (mc+mrr)/2;
        }else{
            mll = mc; // mrr不変
            mc = mr;
            ml = (mll+mc)/2;
            mr = (mc+mrr)/2;
        }
    }
    pos = mc;
    unsigned short end=min(strlen(s),strlen(l),N);//最も短い配列の長さ
    err=2*(Distance(x,s,0,pos)+Distance(y,l,0,pos));
    printf("%d,%d\n",pos,err);//検証用出力
    
    
    // test
    //printf("%s\n", x);
    //printf("%s\n", y);
    //printf("%s\n", s);
    //printf("%s\n", l);


    // ファイル出力
    // 元のanswerと同階層で出力する際は上書きに注意
    FILE *afile = fopen("our answer", "w");
    if (afile == NULL)
    {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }
    fprintf(afile, "%d,%d\n", pos, err);

    // 各ファイルをクローズする

    fclose(afile);


    return 0;
}