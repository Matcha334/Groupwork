#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50000
#define K 250 //刻み値
#define min2(a, b) (a < b ? a : b)
#define max2(a, b) (a > b ? a : b)
#define min3(a, b, c) (a > b ? (b > c ? c : b) : (a > c ? c : a))
#define max3(a, b, c) (a < b ? (b < c ? c : b) : (a < c ? c : a))


//一般的な方法で編集距離を求める関数
//idata:xかy, odata1:先に動作させるsかl, odata2:後に動作させるsかl,
//ist:idataについて何番目の塊か, ost:odataについて開始インデックス, pos:odata1とodata2の入れ替え位置
static int* Distance (char * idata,char * odata1,char * odata2, unsigned short ist, unsigned short ost, unsigned short pos){
    unsigned short matrix[K+1][2*K+1];//DP行列
    unsigned short i,j;//i方向がidata,j方向がodata

    //配列の端を埋める
    for (i = 0; i <= K; i++) {
        matrix[i][0] = i;
    }
    for (i = 1; i <= 2*K; i++) {
        matrix[0][i] = i;
    }

    //DP行列を埋める
    for (i = 1; i <= K; i++) {
        char c1,c2;//c1,c2:比較文字
        c1 = idata[i-1+K*ist];

        for (j = 1; j <= 2*K; j++) {
            //どちらのodataを採用するか
            if (j-1+ost < pos) {
                c2 = odata1[j-1+ost];//入れ替えないとき
            } else {
                c2 = odata2[j-1+ost];//入れ替えるとき
            }

            unsigned short cost=(c1==c2?0:1);//文字が同じかどうかのコスト
            matrix[i][j]=min3(matrix[i-1][j]+1,matrix[i][j-1]+1,matrix[i-1][j-1]+cost);
        }
    }

    //idataの長さKに対応するodataの長さを求める->j
    for (i = 0; i < K;) {
        if (matrix[i][j] == matrix[i+1][j+1]) {//一致
            i++;
            j++;
        } else if (matrix[i][j+1] <= matrix[i+1][j]) {//挿入
            j++;
        } else {//削除
            i++;
        }
    }

    unsigned short ans[2];//返り値用
    ans[0] = j;//返り値1
    ans[1] = matrix[K][j];//返り値2
    return ans;
}

//答えを求める関数
//sw:スイッチ; sw=1:pos用, sw=2:err用
//idata:xかy, odata1:先に動作させるsかl, odata2:後に動作させるsかl, pos:odata1とodata2の入れ替え位置
unsigned short run (unsigned short sw,char * idata,char * odata1,char * odata2, unsigned short pos) {
    unsigned short a[2];
    unsigned short d = 0, q =0, ave =0 ,i;
    for (i = 0; i <N/K; i++) {
        a = Distance (idata, odata1, odata2, i, q, pos);

        q = q + a[0];
        d = d + a[1];
        ave = d/(i+1);

        //posのときの返り値
        if (sw == 1){
            if (a[1] > ave+(K/10)) {
                return i*K;
            }
        }

    }
    if (sw == 2) return d;//errのときの返り値

    return -1;//エラーのときの返り値
}

unsigned short main (int argc, char const *argv[])
{
    // ファイル入力
    // ファイル階層は各自で合わせてください
    FILE *ifile = fopen("Model2/dat7/idata","r");
    FILE *ofile = fopen("Model2/dat7/odata","r");
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

    unsigned short a,b;//posとerr計算のための記録用

    //posを探す
    a = run(1,x, s, l, N);//xとsを比較してpos候補を返す
    b = run(1,y, l, s, N);//yとlを比較してpos候補を探す
    pos = (a+b)/2;//posを候補の平均で求める


    //errを探す
    //引数(a+b)/2をposにすると謎にバグる->解決求む
    a = run(2,x, s, l, (a+b)/2);//xに対するerrを求める
    b = run(2,y, l, s, (a+b)/2);//yに対するerrを求める
    err = a+b;

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

