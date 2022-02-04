#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define N 50000
#define TMP 100
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
    int pos, err;
} DiffTypes;

// 2文字列間編集距離の格納用
typedef struct
{
    char *str1, *str2;
    int len1, len2;
    int flag; // idata<=odataなら1,それ以外なら0
    int delta, offset, max, worst;
    int pwrlen;
    int d;
    int ed;
    int rep;
    DiffTypes difftypes;
    int *fp;
    int *path;
    PointWithRoute *pwr;
} Diff;

int recordSeq(Diff *diff, Point *epc, int epclen, int xindex)
{
    int px = 0, py = 0;                   // エディットグラフを(0,0)からたどるための座標
    int j = 0, m = 0;                     // index入れ
    char prev = ' ';                      // 前の操作
    int cont = 0;                         // 前の操作が連続した回数
    int result = 0;                       // result
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
                    if (prev == '+')
                    {
                        result += 1;
                        cont++;
                    }
                    else if (prev == '-')
                    {
                        result -= cont / 2;
                        cont = 0;
                    }
                    else
                    {
                        result += 1;
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
                    if (prev == '-')
                    {
                        result += 1;
                        cont++;
                    }
                    else if (prev == '+')
                    {
                        result -= cont / 2;
                        cont = 0;
                    }
                    else
                    {
                        result += 1;
                        cont = 0;
                    }
                    prev = '-';
                    j++;

                    px++;
                }
                // その文字はそのまま
                // エディットグラフでは斜め移動
                else
                {
                    prev = ' ';
                    cont = 0;
                    j++;

                    px++;
                    py++;
                }
                if (j >= xindex)
                    break;
            }
        }
        else
        {
            while (px < epc[i].x || py < epc[i].y)
            {
                if ((epc[i].y - epc[i].x) > (py - px))
                {
                    if (prev == '+')
                    {
                        result += 1;
                        cont++;
                    }
                    else if (prev == '-')
                    {
                        result -= cont / 2;
                        cont = 0;
                    }
                    else
                    {
                        result += 1;
                        cont = 0;
                    }
                    prev = '+';
                    j++;

                    py++;
                }
                else if ((epc[i].y - epc[i].x) < (py - px))
                {
                    if (prev == '-')
                    {
                        result += 1;
                        cont++;
                    }
                    else if (prev == '+')
                    {
                        result -= cont / 2;
                        cont = 0;
                    }
                    else
                    {
                        result += 1;
                        cont = 0;
                    }
                    prev = '-';
                    j++;
                    px++;
                }
                else
                {
                    prev = ' ';
                    cont = 0;
                    j++;

                    px++;
                    py++;
                }
                if (j >= xindex)
                    break;
            }
        }
    }

    return result;
}

// 初期化
void NewDiff(Diff *diff, char *str1, char *str2, int len2)
{
    if (N <= len2)
    {
        diff->str1 = str1, diff->str2 = str2;
        diff->len1 = N, diff->len2 = len2;
        diff->flag = 1;
    }
    else
    {
        diff->str1 = str2, diff->str2 = str1;
        diff->len1 = len2, diff->len2 = N;
        diff->flag = 0;
    }
    diff->delta = diff->len2 - diff->len1;
    diff->offset = diff->len1 + 1;
    diff->max = diff->len1 + diff->len2 + 3;
    diff->worst = (diff->len1 + diff->len2) * diff->len1 / 5;
    diff->pwrlen = 0;

    diff->fp = (int *)malloc(diff->max * sizeof(int));
    diff->path = (int *)malloc(diff->max * sizeof(int));
    diff->pwr = (PointWithRoute *)malloc(diff->worst * sizeof(PointWithRoute));

    for (int i = 0; i < diff->max; i++)
    {
        diff->fp[i] = -1;
        diff->path[i] = -1;
    }
    diff->rep = 0;
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

// pループの中身 返り値はpループの終了時のl、終了しなければ0
int pLoop(Diff *diff, int p)
{
    int l;
    for (int k = -p; k <= diff->delta - 1; k++)
    {
        // printf("k:%d\t", k);
        // このループのpの値ではfp[k+1]はまだ求められていないので、一つ前のpループでの値を参照できる
        // 削除したときにp値が増え、挿入は削除した分のdeltaへの復帰なのでpが増えない
        l = k + diff->offset;

        diff->fp[l] = snake(k, diff->fp[l - 1] + 1, diff->fp[l + 1], diff);
    }

    // ループの方向を逆にしている。挿入と削除で増減するpの値が逆のため
    for (int k = diff->delta + p; k >= diff->delta + 1; k--)
    {
        // printf("k:%d\t", k);
        // 上に同じ
        l = k + diff->offset;
        diff->fp[l] = snake(k, diff->fp[l - 1] + 1, diff->fp[l + 1], diff);
    }

    l = diff->delta + diff->offset;
    diff->fp[l] = snake(diff->delta, diff->fp[l - 1] + 1, diff->fp[l + 1], diff);
    if (diff->pwrlen < diff->worst + 100000000)
    {
        diff->worst += 100000000;
        diff->pwr = (PointWithRoute *)realloc(diff->pwr, sizeof(PointWithRoute) * diff->worst);
    }
    if (diff->fp[l] == diff->len2) // k=delta上でy=len2までたどり着く、つまり点(len1,len2)に到着していた場合
    {
        // printf("D=%d+2*%d\n", delta, p);
        diff->d = diff->delta + 2 * p;
        return 1;
    }
    return 0;
}

// 終了時
void freeDiff(Diff *diff)
{
    free(diff->fp);
    free(diff->path);
    free(diff->pwr);
}

// 一気にやる
DiffTypes calcDiff(char *x, char *y, char *s, char *l, int len_s, int len_l)
{
    Diff *diff[4];
    for (int i = 0; i < 4; i++)
    {
        diff[i] = (Diff *)malloc(sizeof(Diff));
    }

    // 逆順
    char xr[N], yr[N];
    char *sr, *lr;
    sr = (char *)malloc(len_s * sizeof(Diff));
    lr = (char *)malloc(len_l * sizeof(Diff));
    for (int i = 0; i < N; i++)
    {
        xr[i] = x[N - i - 1];
        yr[i] = y[N - i - 1];
    }
    for (int i = 0; i < len_s; i++)
    {
        sr[i] = s[len_s - i - 1];
    }
    for (int i = 0; i < len_s; i++)
    {
        lr[i] = l[len_l - i - 1];
    }

    // 初期化
    NewDiff(diff[0], x, s, len_s);
    NewDiff(diff[1], y, l, len_l);
    NewDiff(diff[2], xr, lr, len_l);
    NewDiff(diff[3], yr, sr, len_s);

    // l格納
    int l_[4];
    for (int i = 0; i < 4; i++)
    {
        l_[i] = diff[i]->delta + diff[i]->offset;
    }
    // 進度と前位置
    int prog[4][P_ARR] = {0}, prev[4] = {0};
    // 推定位置格納
    int est = -1;
    // 推定値格納
    int est_Lx[4][P_ARR];
    // 残差の2乗格納
    int res2_Lx[4][P_ARR];
    // 最大残差*3とindex
    int res3tmp[2] = {0}, res_index = 0;
    int res3[4] = {0}, res_cont[4] = {0}, tmp_x[4][10] = {0};
    int res_max[4] = {0}, res_max_index[4] = {P_ARR / 2, P_ARR / 2, P_ARR / 2, P_ARR / 2};
    int f_break = 0;

    // 0の時はdeltaの影響で値がでかいのでパス
    for (int i = 0; i < 4; i++)
    {
        pLoop(diff[i], 0);
        prev[i] = diff[i]->pwr[diff[i]->pwrlen - 1].x;
    }

    // p[1,2*TMP-1]まで(prog[0~1])
    int x_, p;
    for (p = 1; p <= 3 * TMP; p++)
    {
        if (pLoop(diff[0], p) || pLoop(diff[1], p) || pLoop(diff[2], p) || pLoop(diff[3], p))
        {
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            x_ = diff[i]->pwr[diff[i]->pwrlen - 1].x;
            prog[i][p / TMP] += x_ - prev[i];
            prev[i] = x_;
        }
    }

    // est_L[3]を使うので作っておく
    for (int i = 0; i < 4; i++)
    {
        est_Lx[i][2] = (int)(0.50 * (prog[i][0] + prog[i][1]));
        res2_Lx[i][0] = 0;
        res2_Lx[i][1] = 0;
        res2_Lx[i][2] = 0;
    }

    for (p = 3 * TMP + 1;; p++)
    {
        if (pLoop(diff[0], p) || pLoop(diff[1], p) || pLoop(diff[2], p) || pLoop(diff[3], p))
        {
            res_index = -1;
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            x_ = diff[i]->pwr[diff[i]->pwrlen - 1].x;
            prog[i][p / TMP] += x_ - prev[i];
            prev[i] = x_;
        }

        // 位置とP、l
        // fp[l]が最高到達点のyだから、yがidataだった場合posになる
        // PがTMP進むごとに進んだ長さを比較
        if ((p + 1) % TMP == 0)
        {
            int tmp = p / TMP;

            // printf("\n[%d]res3:", tmp);
            for (int i = 0; i < 4; i++)
            {
                // 予測値と残差
                est_Lx[i][tmp] = (int)(0.05 * prog[i][tmp - 1] + 0.30 * prog[i][tmp - 2] + 0.30 * prog[i][tmp - 3] + 0.35 * est_Lx[i][tmp - 1]);
                res2_Lx[i][tmp] = (prog[i][tmp] - est_Lx[i][tmp]) * (prog[i][tmp] - est_Lx[i][tmp]);
                res3[i] = res2_Lx[i][tmp] + res2_Lx[i][tmp - 1] + res2_Lx[i][tmp - 2];
                // printf("\t\t%d:%d,x=%d", i, res3[i], diff[i]->pwr[diff[i]->pwrlen - 1].x);

                // 最大を記録しておく
                if (res_max[i] < res3[i])
                {
                    res_max[i] = res3[i];
                    res_max_index[i] = diff[i]->pwr[diff[i]->pwrlen - 1].x;
                }

                // 残差から変化点推定
                if (tmp == 3)
                {
                    continue;
                }
                else if (tmp < 7)
                {
                    switch (i)
                    {
                    case 0:
                    case 1:
                        res3tmp[0] += res3[i];
                        break;

                    case 2:
                    case 3:
                        res3tmp[1] += res3[i];
                        break;
                    }
                }
                else
                {
                    switch (i)
                    {
                    case 0:
                    case 1:
                        if (res3[i] > (res3tmp[0] / 6) * 4)
                        {
                            res_cont[i] += 1;
                        }

                        else
                            res_cont[i] = 0;

                        if (res_cont[i] >= 2)
                        {
                            tmp_x[i][res_cont[i] - 2] = diff[i]->pwr[diff[i]->pwrlen - 1].x;
                            if (i == 0)
                            {
                                for (int j = 0; j < res_cont[1] - 2; j++)
                                {
                                    if (abs(tmp_x[i][res_cont[i] - 2] - tmp_x[1][j]) < 1000)
                                    {
                                        res_index = (tmp_x[i][res_cont[i] - 2] + tmp_x[1][j]) / 2 - 1000;
                                        // printf("est_x:%d\n", res_index);
                                        f_break = 1;
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                for (int j = 0; j < res_cont[0] - 2; j++)
                                {
                                    if (abs(tmp_x[i][res_cont[i] - 2] - tmp_x[0][j]) < 1000)
                                    {
                                        res_index = (tmp_x[i][res_cont[i] - 2] + tmp_x[0][j]) / 2 - 1000;
                                        // printf("est_x:%d\n", res_index);
                                        f_break = 1;
                                        break;
                                    }
                                }
                            }
                        }

                        break;

                    case 2:
                    case 3:
                        if (res3[i] > (res3tmp[1] / 6) * 4)
                        {
                            res_cont[i] += 1;
                        }
                        else
                            res_cont[i] = 0;

                        if (res_cont[i] >= 1)
                        {
                            tmp_x[i][res_cont[i] - 2] = diff[i]->pwr[diff[i]->pwrlen - 1].x;
                            if (i == 2)
                            {
                                for (int j = 0; j < res_cont[3] - 2; j++)
                                {
                                    if (abs(tmp_x[i][res_cont[i] - 2] - tmp_x[3][j]) < 1000)
                                    {
                                        res_index = 50000 - (tmp_x[i][res_cont[i] - 2] + tmp_x[3][j]) / 2 + 1000;
                                        // printf("est_x:%d\n", res_index);
                                        f_break = 1;
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                for (int j = 0; j < res_cont[2] - 2; j++)
                                {
                                    if (abs(tmp_x[i][res_cont[i] - 2] - tmp_x[2][j]) < 1000)
                                    {
                                        res_index = 50000 - (tmp_x[i][res_cont[i] - 2] + tmp_x[2][j]) / 2 + 1000;
                                        // printf("est_x:%d\n", res_index);
                                        f_break = 1;
                                        break;
                                    }
                                }
                            }

                            break;
                        }
                    }
                }
                if (f_break)
                {
                    break;
                }
            }
        }
        // break条件
        if (f_break)
        {
            break;
        }
    }

    if (res_index == -1)
    {
        int tmp = 0;
        for (int i = 0; i <= 1; i++)
        {
            tmp += res_max_index[i];
        }
        for (int i = 2; i <= 3; i++)
        {
            tmp += 50000 - res_max_index[i];
        }
        res_index = tmp / 4;
    }

    // breakしたときのpから再開
    int p_ = p, fl[4] = {0};
    for (int i = 0; i <= 1; i++)
    {
        while (res_index > diff[i]->pwr[diff[i]->pwrlen - 1].x && !(fl[i]))
        {
            // printf("%d > %d\n", res_index, diff[i]->pwr[diff[i]->pwrlen - 1].x);
            fl[i] = pLoop(diff[i], p_);
            p++;
        }
        p_ = p;
    }
    for (int i = 2; i <= 3; i++)
    {
        while (50000 - res_index > diff[i]->pwr[diff[i]->pwrlen - 1].x && !(fl[i]))
        {
            fl[i] = pLoop(diff[i], p_);
            p++;
        }
        p_ = p;
    }

    int err = 0;
    for (int i = 0; i <= 1; i++)
    {
        int r = diff[i]->path[diff[i]->delta + diff[i]->offset]; // pLoopの返り値がlになる
        Point epc[N];
        int j = 0;
        while (r != -1)
        {
            epc[j].x = diff[i]->pwr[r].x;
            epc[j].y = diff[i]->pwr[r].y;
            r = diff[i]->pwr[r].r;
            j++;
        }
        // printf("j:%d,x:%d\n", j, res_index);
        err += recordSeq(diff[i], epc, j, res_index);
    }

    for (int i = 2; i <= 3; i++)
    {
        int r = diff[i]->path[diff[i]->delta + diff[i]->offset];
        Point epc[N];
        int j = 0;
        while (r != -1)
        {
            epc[j].x = diff[i]->pwr[r].x;
            epc[j].y = diff[i]->pwr[r].y;
            r = diff[i]->pwr[r].r;
            j++;
        }
        // printf("j:%d,x:%d\n", j, 50000 - res_index);
        err += recordSeq(diff[i], epc, j, 50000 - res_index);
    }

    DiffTypes difftypes = {res_index, err};
    for (int i = 0; i < 4; i++)
    {
        freeDiff(diff[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        free(diff[i]);
    }
    free(sr);
    free(lr);
    return difftypes;
}

int main(int argc, char const *argv[])
{
    {
        // ファイル入力
        FILE *ifile = fopen(argv[1], "r");
        FILE *ofile = fopen(argv[2], "r");

        if (ifile == NULL || ofile == NULL)
        {
            fprintf(stderr, "cannot open file\n");
            exit(1);
        }

        // idata, odata読込
        char x[N], y[N];
        if (fgets(x, N + 100000, ifile) == NULL || fscanf(ifile, "%c", y) == 0 || fgets(y, N + 100000, ifile) == NULL)
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

        DiffTypes dt = calcDiff(x, y, s, l, len_s, len_l);

        // printf("%d,%d\n", dt.pos, dt.err); //検証用出力

        // ファイル出力
        // 元のanswerと同階層で出力する際は上書きに注意
        FILE *afile = fopen("answer", "w");
        if (afile == NULL)
        {
            fprintf(stderr, "cannot open file\n");
            exit(1);
        }
        fprintf(afile, "%d,%d\n", dt.pos, dt.err);

        // 各ファイルをクローズする
        fclose(ifile);
        fclose(ofile);
        fclose(afile);
    }
    return 0;
}
