#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50000
#define TMP 500
#define min2(a, b) (a < b ? a : b)
#define max2(a, b) (a > b ? a : b)
#define min3(a, b, c) (a > b ? (b > c ? c : b) : (a > c ? c : a))
#define max3(a, b, c) (a < b ? (b < c ? c : b) : (a < c ? c : a))

// 座標
typedef struct{
    int x, y;
} Point;

// 斜め線の点と長さ（マス数）
typedef struct{
    int x, y, r;
} PointWithRoute;

// SES記録用
typedef struct{
    char c;
    char type; // +-
} SesElem;

// 返り値用構造体
typedef struct{
    int d;
    int ed;
    int ins; // 挿入
    int del; // 削除
    int rep; // 交換
    int offset;
} DiffTypes;

// 2文字列間編集距離の格納用
typedef struct{
    char *str1, *str2;
    int len1, len2;
    int d, ed; // D(=delta+2*P), レーベンシュタイン距離、置換回数
    int seslen, pwrlen;
    DiffTypes difftypes;
    // 以下の変数の領域はcompose内にしかないのでNewDiffから参照できないことに注意（NewDiff内で領域確保すれば参照可）
    SesElem *ses;
    int *path;
    PointWithRoute *pwr;
} Diff;

void printSES(Diff *diff, int seslen){
    SesElem *ses = diff->ses;
    char prev = ' ';
    for (int i = 0; i < seslen; i++){
        switch (ses[i].type){
        case '+':
            // printf("+ %c", ses[i].c);
            diff->difftypes.ins++;
            if (prev == '-')
                diff->difftypes.rep++;
            prev = '+';
            break;
        case '-':
            // printf("- %c", ses[i].c);
            diff->difftypes.del++;
            if (prev == '+')
                diff->difftypes.rep++;
            prev = '-';
            break;
        case ' ':
            // printf("  %c", ses[i].c);
            prev = ' ';
            break;
        default:
            break;
        }
    }

    // オフセットを求める
    if (ses[0].type == '+'){
        int i = 1;
        while (ses[i].type == '+'){
            diff->difftypes.offset++;
            i++;
        }
    }else if (ses[0].type == '-'){
        int i = 1;
        while (ses[i].type == '-'){
            diff->difftypes.offset--;
            i++;
        }
    }
    diff->difftypes.ed = diff->difftypes.d - diff->difftypes.rep;
}

// 開始地点から斜めに行ける最大を求める。返り値は位置y
int snake(int k, int p1, int p2, Diff *diff){
    int offset = diff->len1 + 1;
    int r = (p1 > p2) ? diff->path[k - 1 + offset] : diff->path[k + 1 + offset];
    int y = max2(p1, p2);
    int x = y - k;

    while (x < diff->len1 && y < diff->len2 && diff->str1[x] == diff->str2[y]){
        x++;
        y++;
    }
    diff->path[k + offset] = diff->pwrlen;
    diff->pwr[diff->pwrlen].x = x;
    diff->pwr[diff->pwrlen].y = y;
    diff->pwr[diff->pwrlen].r = r;
    diff->pwrlen++;
    // printf("(x,y)=(%d,%d)\n", x, y);
    return y;
}

void recordSeq(Diff *diff, Point *epc, int epclen){
    int x = 1, y = 1;
    int px = 0, py = 0;
    for (int i = epclen - 1; i >= 0; i--){
        while (px < epc[i].x || py < epc[i].y){
            if ((epc[i].y - epc[i].x > py - px)){
                diff->ses[diff->seslen].c = diff->str2[py];
                diff->ses[diff->seslen].type = '+';
                diff->seslen++;
                y++;
                py++;
            }
            else if ((epc[i].y - epc[i].x) < (py - px)){
                diff->ses[diff->seslen].c = diff->str1[px];
                diff->ses[diff->seslen].type = '-';
                diff->seslen++;
                x++;
                px++;
            }
            else{
                diff->ses[diff->seslen].c = diff->str1[px];
                diff->ses[diff->seslen].type = ' ';
                diff->seslen++;
                x++;
                y++;
                px++;
                py++;
            }
        }
    }
}

// 編集距離の計算
void compose(Diff *diff){
    char *str1 = diff->str1, *str2 = diff->str2;
    int len1 = diff->len1, len2 = diff->len2;
    int delta = len2 - len1,              // 文字数の差
        offset = len1 + 1,                // 添字がマイナスになるのを防ぐ
        max = len1 + len2 + 3,            // 添字最大+1
        worst = (len1 + len2) * len1 / 3; // 最悪計算量の想定(適当)
    int fp[max], path[max];
    SesElem ses[max];
    PointWithRoute *pwr = (PointWithRoute *)malloc(worst * sizeof(PointWithRoute));
    diff->ses = ses, diff->path = path, diff->pwr = pwr;

    // 初期化
    for (int i = 0; i <= max; i++){
        fp[i] = -1;
        diff->path[i] = -1;
    }

    // あるpのときの取りうる点のうちk上に斜めに進める限り進んだものを求める
    // p（削除回数）は少ない程よい
    int k, l;
    for (int p = 0;; p++){

        for (int k = -p; k <= delta - 1; k++){
            // printf("k:%d\t", k);
            // このループのpの値ではfp[k+1]はまだ求められていないので、一つ前のpループでの値を参照できる
            // 削除したときにp値が増え、挿入は削除した分のdeltaへの復帰なのでpが増えない
            l = k + offset;

            fp[l] = snake(k, fp[l - 1] + 1, fp[l + 1], diff);

            if (diff->pwrlen == worst){
                worst *= 2;
                pwr = (PointWithRoute *)realloc(pwr, sizeof(PointWithRoute) * worst);
                diff->pwr = pwr;
            }
        }

        // ループの方向を逆にしている。挿入と削除で増減するpの値が逆のため
        for (int k = delta + p; k >= delta + 1; k--){
            // printf("k:%d\t", k);
            // 上に同じ
            l = k + offset;
            fp[l] = snake(k, fp[l - 1] + 1, fp[l + 1], diff);

            if (diff->pwrlen == worst){
                worst *= 2;
                pwr = (PointWithRoute *)realloc(pwr, sizeof(PointWithRoute) * worst);
                diff->pwr = pwr;
            }
        }

        l = delta + offset;
        fp[l] = snake(delta, fp[l - 1] + 1, fp[l + 1], diff);
        if (diff->pwrlen == worst){
            worst *= 2;
            pwr = (PointWithRoute *)realloc(pwr, sizeof(PointWithRoute) * worst);
            diff->pwr = pwr;
        }

        if (fp[l] == len2){ // k=delta上でy=len2までたどり着く、つまり点(len1,len2)に到着していた場合
            // printf("D=%d+2*%d\n", delta, p);
            diff->difftypes.ins = p;
            diff->difftypes.del = p;
            diff->difftypes.d = delta + 2 * p;
            break;
        }
    }

    int r = diff->path[l];
    Point epc[len2 + 1];
    int i = 0;
    while (r != -1){
        epc[i].x = diff->pwr[r].x;
        epc[i].y = diff->pwr[r].y;
        r = diff->pwr[r].r;
        i++;
    }
    recordSeq(diff, epc, i);
    printSES(diff, diff->seslen);
}

// 編集距離を求める(Diffはポインタ型)
DiffTypes NewDiff(char *str1, int len1, char *str2, int len2){
    Diff *diff = (Diff *)malloc(sizeof(Diff));
    if (len1 <= len2){
        diff->str1 = str1, diff->str2 = str2;
        diff->len1 = len1, diff->len2 = len2;
    }
    else{
        diff->str1 = str2, diff->str2 = str1;
        diff->len1 = len2, diff->len2 = len1;
    }
    diff->seslen = 0, diff->pwrlen = 0;
    diff->difftypes.d = 0, diff->difftypes.ed = 0;
    diff->difftypes.ins = 0, diff->difftypes.del = 0, diff->difftypes.rep = 0, diff->difftypes.offset = 0;
    compose(diff);
    DiffTypes difftypes = diff->difftypes;

    free(diff);
    return difftypes;
}

int main(int argc, char const *argv[]){
    // ファイル入力
    // ファイル階層は各自で合わせてください
    FILE *ifile = fopen("Model2/dat0/idata","r");
    FILE *ofile = fopen("Model2/dat0/odata","r");

    if (ifile == NULL || ofile == NULL){
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

    // 文字数（改行の分-1する）
    int len_s = strlen(s) - 1;
    int len_l = strlen(l) - 1;

    // 改行を消す
    s[len_s] = '\0';
    l[len_l] = '\0';

    // y,lを逆順にする
    char tmp;
    for (int i = 0; i < N / 2; i++){
        tmp = y[0];
        y[0] = y[N - 1 - i];
        y[N - 1 - i] = tmp;
    }
    for (int i = 0; i < len_l / 2; i++){
        tmp = l[0];
        l[0] = l[len_l - 1 - i];
        l[len_l - 1 - i] = tmp;
    }

    // pos:入れ替わった位置、err:間違いの数
    int pos = 0, err = 0;

    // 一回の探索幅は定数TMPで規定（いまTMP=500）
    int longest = max3(len_s, len_l, N),                   // 最も長い配列の長さ
        times = longest / TMP / 2 + 1,                     // ed配列の長さ
        remain = max2(0, longest - (longest / TMP) * TMP); // 余り(あれば)
    DiffTypes A[times], B[times];                          // 結果(ed)格納
    int eval = TMP * 0.40;

    int index = 0, gapa = 0, gapb = 0;
    for (int i = 0; i < times + 1; i++){
        A[i] = NewDiff(&x[TMP * i - gapa/2], TMP, &s[TMP * i], TMP);
        // printf("A%d: %d\t", i, A[i].ed);
        gapa += A[i].offset;

        B[i] = NewDiff(&y[TMP * i - gapb/2], TMP, &l[TMP * i], TMP);
        // printf("B%d: %d\n", i, B[i].ed);
        gapb += B[i].offset;

        if (i < 3)
            continue;
        if (A[i - 2].ed + A[i - 3].ed > eval * 2 && A[i].ed + A[i - 1].ed > eval * 2){
            index = (i - 3) * TMP;
            break;
        }
        if (B[i - 2].ed + B[i - 3].ed > eval * 2 && B[i].ed + B[i - 1].ed > eval * 2){
            index = longest - i * TMP;
            break;
        }

        // もしbreakが無かったらの処理
        index = N / 2 - TMP;
    }

    // printf("%d ~ %d\n", index, index + 3 * TMP);

    // (index-2)*TMP~(index)*TMP(-1)あたり？という目途がつく


    pos = index + TMP;
    err += NewDiff(x, pos, s, pos).ed;
    err += NewDiff(y, N - pos, l, len_l - pos).ed;

    // printf("%d,%d\n", pos, err); //検証用出力

    // ファイル出力
    // 元のanswerと同階層で出力する際は上書きに注意
    FILE *afile = fopen("answer", "w");
    if (afile == NULL){
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }
    fprintf(afile, "%d,%d\n", pos, err);

    // 各ファイルをクローズする
    fclose(ifile);
    fclose(ofile);
    fclose(afile);

    return 0;
}
