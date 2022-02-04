#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50000
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

// 2文字列間編集距離の格納用
typedef struct
{
    char *str1, *str2;
    int len1, len2;
    int d, rep, ed; // D(=delta+2*P), レーベンシュタイン距離、置換回数
    int seslen, pwrlen;
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
            // printf("+ %c", ses[i].c);
            if (prev == '-')
                diff->rep++;
            prev = '+';
            break;
        case '-':
            // printf("- %c", ses[i].c);
            if (prev == '+')
                diff->rep++;
            prev = '-';
            break;
        case ' ':
            // printf("  %c", ses[i].c);
            prev = ' ';
            break;
        default:
            break;
        }
        // printf("\n");
    }
    diff->ed = diff->d - diff->rep;
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
    // printf("(x,y)=(%d,%d)\n", x, y);
    return y;
}

void recordSeq(Diff *diff, Point *epc, int epclen)
{
    int x = 1, y = 1;
    int px = 0, py = 0;
    for (int i = epclen - 1; i >= 0; i--)
    {
        while (px < epc[i].x || py < epc[i].y)
        {
            if ((epc[i].y - epc[i].x > py - px))
            {
                diff->ses[diff->seslen].c = diff->str2[py];
                diff->ses[diff->seslen].type = '+';
                diff->seslen++;
                y++;
                py++;
            }
            else if ((epc[i].y - epc[i].x) < (py - px))
            {
                diff->ses[diff->seslen].c = diff->str1[px];
                diff->ses[diff->seslen].type = '-';
                diff->seslen++;
                x++;
                px++;
            }
            else
            {
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
void compose(Diff *diff)
{
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
    for (int i = 0; i <= max; i++)
    {
        fp[i] = -1;
        diff->path[i] = -1;
    }

    // あるpのときの取りうる点のうちk上に斜めに進める限り進んだものを求める
    // p（削除回数）は少ない程よい
    int k, l;
    for (int p = 0;; p++)
    {
        if (p % 1000 == 0)
            printf("p=%d\tpwrlen=%d\n", p, diff->pwrlen);

        for (int k = -p; k <= delta - 1; k++)
        {
            // printf("k:%d\t", k);
            // このループのpの値ではfp[k+1]はまだ求められていないので、一つ前のpループでの値を参照できる
            // 削除したときにp値が増え、挿入は削除した分のdeltaへの復帰なのでpが増えない
            l = k + offset;

            fp[l] = snake(k, fp[l - 1] + 1, fp[l + 1], diff);

            if (diff->pwrlen == worst)
            {
                worst *= 2;
                pwr = (PointWithRoute *)realloc(pwr, sizeof(PointWithRoute) * worst);
                diff->pwr = pwr;
            }
        }

        // ループの方向を逆にしている。挿入と削除で増減するpの値が逆のため
        for (int k = delta + p; k >= delta + 1; k--)
        {
            // printf("k:%d\t", k);
            // 上に同じ
            l = k + offset;
            fp[l] = snake(k, fp[l - 1] + 1, fp[l + 1], diff);

            if (diff->pwrlen == worst)
            {
                worst *= 2;
                pwr = (PointWithRoute *)realloc(pwr, sizeof(PointWithRoute) * worst);
                diff->pwr = pwr;
            }
        }

        l = delta + offset;
        fp[l] = snake(delta, fp[l - 1] + 1, fp[l + 1], diff);
        if (diff->pwrlen == worst)
        {
            worst *= 2;
            pwr = (PointWithRoute *)realloc(pwr, sizeof(PointWithRoute) * worst);
            diff->pwr = pwr;
        }

        if (fp[l] == len2) // k=delta上でy=len2までたどり着く、つまり点(len1,len2)に到着していた場合
        {
            printf("\nD=%d+2*%d\n", delta, p);
            diff->d = delta + 2 * p;
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
        i++;
    }
    recordSeq(diff, epc, i);
    printSES(diff, diff->seslen);
}

// 編集距離を求める(Diffはポインタ型)
int NewDiff(char *str1, int len1, char *str2, int len2)
{
    Diff *diff = (Diff *)malloc(sizeof(Diff));
    if (len1 < len2)
    {
        diff->str1 = str1, diff->str2 = str2;
        diff->len1 = len1, diff->len2 = len2;
    }
    else
    {
        diff->str1 = str2, diff->str2 = str1;
        diff->len1 = len2, diff->len2 = len1;
    }
    diff->d = 0, diff->rep = 0, diff->ed = 0;
    diff->seslen = 0, diff->pwrlen = 0;
    compose(diff);
    int ed = diff->ed;

    free(diff);
    return ed;
}

int main(int argc, char const *argv[])
{
    // ファイル入力
    // ファイル階層は各自で合わせてください
    FILE *ifile = fopen("Model2/dat0/idata","r");
    FILE *ofile = fopen("Model2/dat0/odata","r");

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

    // 文字数（改行の分-1する）
    int len_s = strlen(s) - 1;
    int len_l = strlen(l) - 1;

    // 改行を消す
    s[len_s] = '\0';
    l[len_l] = '\0';

    // pos:入れ替わった位置、err:間違いの数
    int pos = 0, err = 0;

    // err = NewDiff(x, N, s, len_s);
    // printf("completed\n");
    // printf("leven dist = %d\n", err);
    
    unsigned short L=0,R=N-1;
    unsigned short mll = L;
    unsigned short mrr = R;
    unsigned short mc = (mll+mrr)/2;
    unsigned short ml = (mll+mc)/2;
    unsigned short mr = (mc+mrr)/2;

    while (ml < mc && mc < mr)
    {
        printf("%d,%d,%d\n", ml, mc, mr);
        printf("Edl\n");
        double Edl = (double)NewDiff(x, ml, s, ml) / (ml + 1); //第1四分位数の平均編集距離
        printf("Edc\n");
        double Edc = (double)NewDiff(x, mc, s, mc) / (mc + 1); //中央の平均編集距離
        printf("Edr\n");
        double Edr = (double)NewDiff(x, mr, s, mr) / (mr + 1); //第3四分位数の平均編集距離
        printf("%lf,%lf,%lf\n", Edl, Edc, Edr);

        if (min3(Edl, Edr, Edc) == Edl)
        {
            mrr = mc; // mll不変
            mc = ml;
            ml = (mll + mc) / 2;
            mr = (mc + mrr) / 2;
        }
        else if (min3(Edl, Edr, Edc) == Edc)
        {
            mll = ml;
            mrr = mr; // mc不変
            ml = (mll + mc) / 2;
            mr = (mc + mrr) / 2;
        }
        else
        {
            mll = mc; // mrr不変
            mc = mr;
            ml = (mll + mc) / 2;
            mr = (mc + mrr) / 2;
        }
    }
    pos = mc;
    int end = min3(len_s, len_l, N); //最も短い配列の長さ
    err = 2 * (NewDiff(x, pos, s, pos) + NewDiff(&y[pos], N - pos, &l[pos], len_l - pos));
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
