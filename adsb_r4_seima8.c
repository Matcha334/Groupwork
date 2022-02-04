#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50000
#define K 250 //���ݒl
#define min2(a, b) (a < b ? a : b)
#define max2(a, b) (a > b ? a : b)
#define min3(a, b, c) (a > b ? (b > c ? c : b) : (a > c ? c : a))
#define max3(a, b, c) (a < b ? (b < c ? c : b) : (a < c ? c : a))


//��ʓI�ȕ��@�ŕҏW���������߂�֐�
//idata:x��y, odata1:��ɓ��삳����s��l, odata2:��ɓ��삳����s��l,
//ist:idata�ɂ��ĉ��Ԗڂ̉�, ost:odata�ɂ��ĊJ�n�C���f�b�N�X, pos:odata1��odata2�̓���ւ��ʒu
static int* Distance (char * idata,char * odata1,char * odata2, unsigned short ist, unsigned short ost, unsigned short pos){
    unsigned short matrix[K+1][2*K+1];//DP�s��
    unsigned short i,j;//i������idata,j������odata

    //�z��̒[�𖄂߂�
    for (i = 0; i <= K; i++) {
        matrix[i][0] = i;
    }
    for (i = 1; i <= 2*K; i++) {
        matrix[0][i] = i;
    }

    //DP�s��𖄂߂�
    for (i = 1; i <= K; i++) {
        char c1,c2;//c1,c2:��r����
        c1 = idata[i-1+K*ist];

        for (j = 1; j <= 2*K; j++) {
            //�ǂ����odata���̗p���邩
            if (j-1+ost < pos) {
                c2 = odata1[j-1+ost];//����ւ��Ȃ��Ƃ�
            } else {
                c2 = odata2[j-1+ost];//����ւ���Ƃ�
            }

            unsigned short cost=(c1==c2?0:1);//�������������ǂ����̃R�X�g
            matrix[i][j]=min3(matrix[i-1][j]+1,matrix[i][j-1]+1,matrix[i-1][j-1]+cost);
        }
    }

    //idata�̒���K�ɑΉ�����odata�̒��������߂�->j
    for (i = 0; i < K;) {
        if (matrix[i][j] == matrix[i+1][j+1]) {//��v
            i++;
            j++;
        } else if (matrix[i][j+1] <= matrix[i+1][j]) {//�}��
            j++;
        } else {//�폜
            i++;
        }
    }

    unsigned short ans[2];//�Ԃ�l�p
    ans[0] = j;//�Ԃ�l1
    ans[1] = matrix[K][j];//�Ԃ�l2
    return ans;
}

//���������߂�֐�
//sw:�X�C�b�`; sw=1:pos�p, sw=2:err�p
//idata:x��y, odata1:��ɓ��삳����s��l, odata2:��ɓ��삳����s��l, pos:odata1��odata2�̓���ւ��ʒu
unsigned short run (unsigned short sw,char * idata,char * odata1,char * odata2, unsigned short pos) {
    unsigned short a[2];
    unsigned short d = 0, q =0, ave =0 ,i;
    for (i = 0; i <N/K; i++) {
        a = Distance (idata, odata1, odata2, i, q, pos);

        q = q + a[0];
        d = d + a[1];
        ave = d/(i+1);

        //pos�̂Ƃ��̕Ԃ�l
        if (sw == 1){
            if (a[1] > ave+(K/10)) {
                return i*K;
            }
        }

    }
    if (sw == 2) return d;//err�̂Ƃ��̕Ԃ�l

    return -1;//�G���[�̂Ƃ��̕Ԃ�l
}

unsigned short main (int argc, char const *argv[])
{
    // �t�@�C������
    // �t�@�C���K�w�͊e���ō��킹�Ă�������
    FILE *ifile = fopen("Model2/dat7/idata","r");
    FILE *ofile = fopen("Model2/dat7/odata","r");
    if (ifile == NULL || ofile == NULL)
    {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }
    

    // idata, odata�Ǎ�
    char x[N], y[N];
    fgets(x, N + 1, ifile);
    fscanf(ifile, "%c", y); // ���s�폜
    fgets(y, N + 1, ifile);
    char s[N * 2], l[N * 2];
    fgets(s, N * 2, ofile);
    fgets(l, N * 2, ofile);

    // ���s������
    s[strlen(s) - 1] = '\0';
    l[strlen(l) - 1] = '\0';

    //pos:����ւ�����ʒu, err:�ԈႢ�̐�
    unsigned short pos = 0, err = 0;

    unsigned short a,b;//pos��err�v�Z�̂��߂̋L�^�p

    //pos��T��
    a = run(1,x, s, l, N);//x��s���r����pos����Ԃ�
    b = run(1,y, l, s, N);//y��l���r����pos����T��
    pos = (a+b)/2;//pos�����̕��ςŋ��߂�


    //err��T��
    //����(a+b)/2��pos�ɂ���Ɠ�Ƀo�O��->��������
    a = run(2,x, s, l, (a+b)/2);//x�ɑ΂���err�����߂�
    b = run(2,y, l, s, (a+b)/2);//y�ɑ΂���err�����߂�
    err = a+b;

    printf("%d,%d\n",pos,err);//���ؗp�o��

    /*���؂̂��߈�U����

    // test
    printf("%s\n", x);
    printf("%s\n", y);
    printf("%s\n", s);
    printf("%s\n", l);

    // �t�@�C���o��
    // ����answer�Ɠ��K�w�ŏo�͂���ۂ͏㏑���ɒ���
    FILE *afile = fopen("my answer", "w");
    if (afile == NULL)
    {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }
    fprintf(afile, "%d,%d\n", pos, err);

    // �e�t�@�C�����N���[�Y����
    fclose(ifile);
    fclose(ofile);
    fclose(afile);
    */

    return 0;
}

