#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 50000

// 最小関数(int版)
int min_int(int a, int b, int c){
    return a>b?(b>c?c:b):(a>c?c:a);
}

// 最小関数(引数double型の戻り値変数名版)
char *min_double(double a, double b, double c){
    return a>b?(b>c?"Edc":"Edr"):(a>c?"Edc":"Edl");
}

int Ed(char *x, int m, char *y, int n){
    int change=0;
    if(n==0){
        return m;
    }else if(m==0){
        return n;
    }else{
        if(x[m-1]==y[n-1]){
            change=0;
        }else{
            change=1;
        }
        return min_int(Ed(x,m-1,y,n-1)+change, Ed(x,m-1,y,n)+1, Ed(x,m,y,n-1)+1);
    }
}

int main(int argc, char const*argv[]){

    FILE *ifile = fopen("Model1/dat9/idata","r");
    FILE *ofile = fopen("Model1/dat9/odata","r");
    if(ifile==NULL||ofile==NULL){
        fprintf(stderr, "cannot open read file\n");
        exit(1);
    }

    // 入力ファイル読み込み
    char x[N], y[N];

    fgets(x, N+1, ifile);
    fscanf(ifile, "%c", y); // 改行削除
    fgets(y, N+1, ifile);

    char s[N*2], l[N*2];
    fgets(s, N*2, ofile);
    fgets(l, N*2, ofile);

    // 改行を消す
    s[strlen(s)-1] = '\0';
    l[strlen(l)-1] = '\0';

    int pos=0, err=0;

    // プログラム
    
    int left=0, right=N-1;
    int mc = (left+right)/2;
    int ml = (left+mc)/2;
    int mr = (mc+right)/2;
    int mll = left;
    int mrr = right;
    int exchange; // 交換した場所
    /*
    while(1){
        if(mc-ml<1||mr-mc<1){
            exchange = mc;
            break;
        }
        double Edw = Ed(x,N-1,s,strlen(s))/(N-1); //全体の平均編集距離
        double Edc = Ed(x,mc+1,s,mc+1)/(mc+1); //中央の平均編集距離
        double Edl = Ed(x,ml+1,s,ml+1)/(ml+1); //第1四分位数の平均編集距離
        double Edr = Ed(x,mr+1,s,mr+1)/(mr+1); //第3四分位の平均編集距離
        if(min_double(Edl,Edr,Edc)=="Edl"){
            mrr = mc; // mll不変
            mc = ml;
            ml = (mll+mc)/2;
            mr = (mc+mrr)/2;
        }else if(min_double(Edl,Edr,Edc)=="Edc"){
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
    printf("%d",exchange);
    */
    
    //test
    printf("%s\n",x);
    //printf("%s\n",y);
    //printf("%s\n",s);
    //printf("%s\n",l);
    

    FILE *afile = fopen("answer", "w");
    if(afile==NULL){
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }

    fclose(ifile);
    fclose(ofile);
    fclose(afile);

    return 0;
}