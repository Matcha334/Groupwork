#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LEN 100

int min(int a, int b, int c){
    return a>b?(b>c?c:b):(a>c?c:a);
}

int ld(char *x, int m, char *y, int n){
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
        return min(ld(x,m-1,y,n-1)+change,ld(x,m-1,y,n)+1,ld(x,m,y,n-1)+1);
    }
}

static int mn[LEN][LEN] = {0};

int ld_memo(char *x, int m, char *y, int n){
    int change=0;

    if( mn[m][n] != -1 ){
        return mn[m][n];
    }

    if(n==0){
        mn[n][m] = m;
        return mn[n][m];
    }
    else if(m==0){
        mn[n][m] = n;
        return mn[n][m];
    }else{
        change = x[m-1]==y[n-1] ? 0 : 1;

        mn[m-1][n-1] = ld(x, m-1,y, n-1) + change;
        mn[m-1][n] = ld(x, m-1,y, n) + 1;
        mn[m][n-1] = ld(x, m, y, n-1) + 1;

        return min(mn[m-1][n-1], mn[m-1][n], mn[m][n-1]);
    }
}

void init_mn(void){
    int i, j;
    for(i=0; i<LEN; i++){
        for(j=0; j<LEN; j++){
            mn[i][j] = -1;
        }
    }
    return;
}

int main(void){
    char x[LEN], y[LEN];
    int m, n;

    printf("str1:");
    scanf("%s", x);
    printf("str2:");
    scanf("%s", y);
    printf("\n");

    m = strlen(x);
    n = strlen(y);

    printf("no memo\n");
    printf("%d\n", ld(x,m,y,n));
    printf("\n");

    printf("memoize\n");
    init_mn();
    printf("%d\n", ld_memo(x,m,y,n));

    return 0;
}