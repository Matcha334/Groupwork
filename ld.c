#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int min(int a, int b, int c){
    return a>b?(b>c?c:b):(a>c?c:a);
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

int main(){
    char x[] = "TTTBBBN";
    char y[] = "BNTNBTT";
    int m,n;
    printf("%s\n",x);
    printf("%s\n",y);
    m=strlen(x);
    n=strlen(y);
    printf("%d\n",Ed(x,m,y,n));
    return 0;
}