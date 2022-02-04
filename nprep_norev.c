#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50000
#define TMP 200
#define P_ARR N / TMP + 1
#define min2(a, b) (a < b ? a : b)
#define max2(a, b) (a > b ? a : b)
#define min3(a, b, c) (a > b ? (b > c ? c : b) : (a > c ? c : a))
#define max3(a, b, c) (a < b ? (b < c ? c : b) : (a < c ? c : a))

// 座標
typedef struct
{
    int x, y;
} Point;

// 斜め線の点と長さ（マス数）
typedef struct
{
    int x, y, r;
} PointWithRoute;

// SES記録用
typedef struct
{
    char c;
    char type;
} SesElem;

// 返り値用構造体
typedef struct
{
    int d;
    int ed;
    int rep;
    int offset;
    int D[P_ARR]; // 200個ずつの編集距離の配列
} DiffTypes;

// 2文字列間編集距離の格納用
typedef struct
{
    char *str1, *str2;
    int len1, len2;
    int flag;  // idata<=odataなら1,それ以外なら0
    int d, ed; // D(=delta+2*P), レーベンシュタイン距離、置換回数
    int seslen, pwrlen;
    DiffTypes difftypes;
    // 以下の変数の領域はcompose内にしかないのでNewDiffから参照できないことに注意（NewDiff内で領域確保すれば参照可）
    SesElem *ses;
    int *path;
    PointWithRoute *pwr;
} Diff;

void printSES(Diff *diff, int seslen)
{
    SesElem *ses = diff->ses;
    char prev = ' ';
    for (int i = 0; i < seslen; i++)
    {
        switch (ses[i].type)
        {
        case '+':
            printf("+%c", ses[i].c);
            if (prev == '-')
                diff->difftypes.rep++;
            prev = '+';
            break;
        case '-':
            printf("-%c", ses[i].c);
            if (prev == '+')
                diff->difftypes.rep++;
            prev = '-';
            break;
        case ' ':
            printf("%c", ses[i].c);
            prev = ' ';
            break;
        default:
            printf("exception has occured in PrintSES_1.\n");
            break;
        }
    }

    // オフセットを求める
    if (ses[0].type == '+')
    {
        int i = 1;
        while (ses[i].type == '+')
        {
            diff->difftypes.offset++;
            i++;
        }
    }
    else if (ses[0].type == '-')
    {
        int i = 1;
        while (ses[i].type == '-')
        {
            diff->difftypes.offset--;
            i++;
        }
    }
    diff->difftypes.ed = diff->difftypes.d - diff->difftypes.rep;
    // printf("\n");
}

void recordSeq(Diff *diff, Point *epc, int epclen)
{
    int px = 0, py = 0;                   // エディットグラフを(0,0)からたどるための座標
    int j = 0, m = 0;                     // index入れ
    char prev = ' ';                      // 前の操作
    int cont = 0;                         // 前の操作が連続した回数
    for (int i = epclen - 1; i >= 0; i--) // epc(i)は大きいほうから（変化点の座標が小さい順になる）、ses(j)は小さいほうから
    {
        if (diff->flag)
        {
            while (px < epc[i].x || py < epc[i].y)
            {
                // str1に挿入 対象はstr2[py]
                // エディットグラフでは横方向(y)移動
                if ((epc[i].y - epc[i].x) > (py - px))
                {
                    diff->ses[j].c = diff->str2[py];
                    diff->ses[j].type = '+';
                    if (prev == '+')
                    {
                        diff->difftypes.D[m / TMP] += 1;
                        cont++;
                    }
                    else if (prev == '-')
                    {
                        diff->difftypes.D[m / TMP] -= cont;
                        cont = 0;
                    }
                    else
                    {
                        diff->difftypes.D[m / TMP] += 1;
                        cont = 0;
                    }
                    prev = '+';
                    j++;
                    py++;
                }
                // str1から削除 対象はstr1[px]
                // エディットグラフでは縦方向(x)移動
                else if ((epc[i].y - epc[i].x) < (py - px))
                {
                    diff->ses[j].c = diff->str1[px];
                    diff->ses[j].type = '-';
                    if (prev == '-')
                    {
                        diff->difftypes.D[m / TMP] += 1;
                        cont++;
                    }
                    else if (prev == '+')
                    {
                        diff->difftypes.D[m / TMP] -= cont;
                        cont = 0;
                    }
                    else
                    {
                        diff->difftypes.D[m / TMP] += 1;
                        cont = 0;
                    }
                    prev = '-';
                    j++;
                    m++;
                    px++;
                }
                // その文字はそのまま
                // エディットグラフでは斜め移動
                else
                {
                    diff->ses[j].c = diff->str1[px];
                    diff->ses[j].type = ' ';
                    prev = ' ';
                    cont = 0;
                    j++;
                    m++;
                    px++;
                    py++;
                }
            }
        }
        else
        {
            while (px < epc[i].x || py < epc[i].y)
            {
                if ((epc[i].y - epc[i].x) > (py - px))
                {
                    diff->ses[j].c = diff->str2[py];
                    diff->ses[j].type = '+';
                    if (prev == '+')
                    {
                        diff->difftypes.D[m / TMP] += 1;
                        cont++;
                    }
                    else if (prev == '-')
                    {
                        diff->difftypes.D[m / TMP] -= cont;
                        cont = 0;
                    }
                    else
                    {
                        diff->difftypes.D[m / TMP] += 1;
                        cont = 0;
                    }
                    prev = '+';
                    j++;
                    m++;
                    py++;
                }
                else if ((epc[i].y - epc[i].x) < (py - px))
                {
                    diff->ses[j].c = diff->str1[px];
                    diff->ses[j].type = '-';
                    if (prev == '-')
                    {
                        diff->difftypes.D[m / TMP] += 1;
                        cont++;
                    }
                    else if (prev == '+')
                    {
                        diff->difftypes.D[m / TMP] -= cont;
                        cont = 0;
                    }
                    else
                    {
                        diff->difftypes.D[m / TMP] += 1;
                        cont = 0;
                    }
                    prev = '-';
                    j++;
                    px++;
                }
                else
                {
                    diff->ses[j].c = diff->str1[px];
                    diff->ses[j].type = ' ';
                    prev = ' ';
                    cont = 0;
                    j++;
                    m++;
                    px++;
                    py++;
                }
            }
        }
    }
    diff->seslen = j;
}

// 開始地点から斜めに行ける最大を求める。返り値は位置y
int snake(int k, int p1, int p2, Diff *diff)
{
    int offset = diff->len1 + 1;
    int r = (p1 > p2) ? diff->path[k - 1 + offset] : diff->path[k + 1 + offset];
    int y = max2(p1, p2);
    int x = y - k;

    while (x < diff->len1 && y < diff->len2 && diff->str1[x] == diff->str2[y])
    {
        x++;
        y++;
    }
    diff->path[k + offset] = diff->pwrlen;
    diff->pwr[diff->pwrlen].x = x;
    diff->pwr[diff->pwrlen].y = y;
    diff->pwr[diff->pwrlen].r = r;
    diff->pwrlen++;
    // printf("pwrlen=%d,\tk=%d,\t(x,y)=(%d,%d)\tr=%d\n", diff->pwrlen - 1, k, x, y, r);
    return y;
}

// 編集距離の計算
void compose(Diff *diff)
{
    char *str1 = diff->str1, *str2 = diff->str2;
    int len1 = diff->len1, len2 = diff->len2;
    int delta = len2 - len1,              // 文字数の差
        offset = len1 + 1,                // 添字がマイナスになるのを防ぐ
        max = len1 + len2 + 3,            // 添字最大+1
        worst = (len1 + len2) * len1 / 5; // 最悪計算量の想定

    int *fp = (int *)malloc(max * sizeof(int)), *path = (int *)malloc(max * sizeof(int));
    SesElem *ses = (SesElem *)malloc(max * sizeof(SesElem));
    PointWithRoute *pwr = (PointWithRoute *)malloc(worst * sizeof(PointWithRoute));

    diff->ses = &ses[0];
    diff->path = &path[0];
    diff->pwr = &pwr[0];

    // 初期化
    for (int i = 0; i < max; i++)
    {
        fp[i] = -1;
        diff->path[i] = -1;
    }
    for (int i = 0; i < max; i++)
    {
        diff->ses[i].c = 'x';
        diff->ses[i].type = 'n';
    }

    // あるpのときの取りうる点のうちk上に斜めに進める限り進んだものを求める
    // p（削除回数）は少ない程よい
    int k, l;
    for (int p = 0;; p++)
    {
        for (int k = -p; k <= delta - 1; k++)
        {
            // printf("k:%d\t", k);
            // このループのpの値ではfp[k+1]はまだ求められていないので、一つ前のpループでの値を参照できる
            // 削除したときにp値が増え、挿入は削除した分のdeltaへの復帰なのでpが増えない
            l = k + offset;

            fp[l] = snake(k, fp[l - 1] + 1, fp[l + 1], diff);
        }

        // ループの方向を逆にしている。挿入と削除で増減するpの値が逆のため
        for (int k = delta + p; k >= delta + 1; k--)
        {
            // printf("k:%d\t", k);
            // 上に同じ
            l = k + offset;
            fp[l] = snake(k, fp[l - 1] + 1, fp[l + 1], diff);
        }

        l = delta + offset;
        fp[l] = snake(delta, fp[l - 1] + 1, fp[l + 1], diff);
        if (diff->pwrlen < worst + 100000000)
        {
            worst += 100000000;
            pwr = (PointWithRoute *)realloc(pwr, sizeof(PointWithRoute) * worst);
            diff->pwr = pwr;
        }

        if (fp[l] == len2) // k=delta上でy=len2までたどり着く、つまり点(len1,len2)に到着していた場合
        {
            // printf("D=%d+2*%d\n", delta, p);
            diff->difftypes.d = delta + 2 * p;
            break;
        }
    }

    int r = diff->path[l];
    Point epc[len2 + 1];
    int i = 0;
    while (r != -1)
    {
        epc[i].x = diff->pwr[r].x;
        epc[i].y = diff->pwr[r].y;
        r = diff->pwr[r].r;
        // printf("(%d,%d),%d\n", epc[i].x, epc[i].y, r);
        i++;
    }

    // epcには座標が大きい（右下にある）順に点が格納されている
    recordSeq(diff, epc, i);

    // printSES(diff, diff->seslen);

    free(fp);
    free(path);
    free(ses);
    free(pwr);
}

// 編集距離を求める(Diffはポインタ型)
DiffTypes NewDiff(char *str1, int len1, char *str2, int len2)
{
    Diff *diff = (Diff *)malloc(sizeof(Diff));
    if (len1 <= len2)
    {
        diff->str1 = str1, diff->str2 = str2;
        diff->len1 = len1, diff->len2 = len2;
        diff->flag = 1;
    }
    else
    {
        diff->str1 = str2, diff->str2 = str1;
        diff->len1 = len2, diff->len2 = len1;
        diff->flag = 0;
    }
    diff->seslen = 0, diff->pwrlen = 0;
    diff->difftypes.d = 0, diff->difftypes.ed = 0;
    diff->difftypes.rep = 0, diff->difftypes.offset = 0;
    for (int i = 0; i < P_ARR; i++)
    {
        diff->difftypes.D[i] = 0;
    }

    compose(diff);
    DiffTypes difftypes = diff->difftypes;
    free(diff);
    return difftypes;
}

// 予測値推定＋位置推定
int estPos(int *A, int *B)
{
    // 推定位置格納
    int est = -1;
    // A+B推定値格納(D[250]は切り捨てる)
    int est_AB[P_ARR - 1];
    // 残差の2乗格納
    int res2_AB[P_ARR - 1];

    // est_AB[2]を使うので作っておく
    est_AB[2] = (int)(0.50 * (A[0] + A[1]) + 0.50 * (B[0] + B[1]));

    for (int i = 3; i < P_ARR - 1; i++)
    {
        est_AB[i] = (int)(0.05 * (A[i - 1] + B[i - 1]) + 0.30 * (A[i - 2] + B[i - 2]) + 0.30 * (A[i - 3] + B[i - 3]) + 0.35 * est_AB[i - 1]);
        res2_AB[i] = (A[i] + B[i] - est_AB[i]) * (A[i] + B[i] - est_AB[i]);

        // printf("[%d]\ta=%d,b=%d:\t\te=%d,\tr=%d\n", i, A[i], B[i], est_AB[i], res2_AB[i]);
    }

    // 最大残差*3とindex
    int res_max = -1, res_index = 0;
    int res3_tmp = 0;
    // 残差から変化点推定
    for (int i = 3; i < P_ARR - 4; i++)
    {
        res3_tmp = res2_AB[i] + res2_AB[i + 1] + res2_AB[i + 2];
        if (res_max < res3_tmp)
        {
            res_max = res3_tmp;
            res_index = i;
        }
    }
    // 最後に変化点が来た時の検出処理がまだ
    int tmp = res_index * TMP;
    // printf("a:%d,b:%d,est=%d～%d\n", res_index, b_index, tmp, tmp + 200);

    est = tmp;

    return est;
}

int estErr(int *A, int *B, int pos)
{
    int err = 0;
    for (int i = 0; i <= pos / TMP; i++)
    {
        err += A[i];
    }
    for (int i = P_ARR - 1; i >= P_ARR - pos + 1 / TMP; i--)
    {
        err += B[i];
    }

    return err;
}

int main(int argc, char *argv[])
{
    // ファイル入力
    char*filename_in=argv[1];
    char*filename_out=argv[2];
    FILE *ifile = fopen(filename_in,"r");
    FILE *ofile = fopen(filename_out,"r");

    if (ifile == NULL || ofile == NULL)
    {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }

    // idata, odata読込
    char x[N], y[N];
    if (fgets(x, N + 1, ifile) == NULL || fscanf(ifile, "%c", y) == 0 || fgets(y, N + 1, ifile) == NULL)
    {
        fprintf(stderr, "cannot open i file\n");
        exit(1);
    }

    char s[N * 2], l[N * 2];
    if (fgets(s, N * 2, ofile) == NULL || fgets(l, N * 2, ofile) == NULL)
    {
        fprintf(stderr, "cannot open o file\n");
        exit(1);
    }

    // 文字数（改行の分-1する）
    int len_s = strlen(s) - 1;
    int len_l = strlen(l) - 1;

    // 改行を消す
    s[len_s] = '\0';
    l[len_l] = '\0';

    // pos:入れ替わった位置、err:間違いの数
    int pos = 0, err = 0;

    DiffTypes a = NewDiff(x, N, s, len_s);
    DiffTypes b = NewDiff(y, N, l, len_l);

    pos = estPos(a.D, b.D);

    printf("%d,%d\n", pos, err); //検証用出力

    // ファイル出力
    // 元のanswerと同階層で出力する際は上書きに注意
    FILE *afile = fopen("answer", "w");
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

    return 0;
}
