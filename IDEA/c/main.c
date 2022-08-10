#include <stdio.h>
#define lo16(x) ((unsigned short)((x)&0xFFFF))
#define hi16(x) ((unsigned short)((x) >> 16))
/********************************************************/
/* x*ymod(2^(16+1)) を計算する関数 */
/********************************************************/
unsigned short mul(unsigned short x, unsigned short y)
{
    unsigned int t;
    unsigned short u, v;
    if (!x)
        return 1 - y;
    if (!y)
        return 1 - x;
    t = x * y;
    u = lo16(t);
    v = hi16(t);
    return (u - v) + (u < v);
}
/********************************************************/
/* a*xmod2^16+1=1 を計算する関数 */
/********************************************************/
unsigned short mulInv(unsigned short x)
{
    int u, v, w;
    int t1, t2, t3;
    int u1, u2, u3;
    int v1, v2, v3;
    u = (int)x, v = 0x10001;
    u1 = 1, u2 = u, u3 = 0;
    v1 = 0, v2 = v, v3 = 1;
    while (v2)
    {
        w = u2 / v2;
        t1 = u1 - v1 * w;
        t2 = u2 - v2 * w;
        t3 = u3 - v3 * w;
        u1 = v1, u2 = v2, u3 = v3;
        v1 = t1, v2 = t2, v3 = t3;
    }
    if (u1 < 0)
        u1 = u1 + 0x10001;
    return lo16(u1);
}
/********************************************************/
/* IDEA で使用する副鍵を生成する関数
キーボードから入力された文字列を元に暗号化と
復号化で使われる副鍵を生成する
キーボードからの入力は、main関数で行っている*/
/********************************************************/
void IdeaKeyGen(unsigned short key[8], unsigned short ekey[52], unsigned short dkey[52])
{
    int i, j;
    // テスト暗号化鍵
    printf("--暗号副鍵--\n");
    for (i = 0; i < 52; i++)
    {
        if (i < 8)
        {
            ekey[i] = key[i];
        }
        else
        {
            ekey[i] = ekey[((i + 1) & 0x7) ? i - 7 : i - 15] << 9 | ekey[(((i + 2) & 0x7) < 2) ? i - 14 : i - 6] >> 7;
        }
        printf("%x", ekey[i]);
    }
    printf("\n");
    // テスト復号化鍵
    printf("--復号副鍵--\n");
    dkey[48] = mulInv(ekey[0]);
    dkey[49] = -ekey[1];
    dkey[50] = -ekey[2];
    dkey[51] = mulInv(ekey[3]);
    for (i = 42, j = 4; i >= 0; i = i - 6, j = j + 6)
    {
        dkey[i + 4] = ekey[j + 0];
        dkey[i + 5] = ekey[j + 1];
        dkey[i + 0] = mulInv(ekey[j + 2]);
        if (i)
        {
            dkey[i + 2] = -ekey[j + 3];
            dkey[i + 1] = -ekey[j + 4];
        }
        else
        {
            dkey[1] = -ekey[49];
            dkey[2] = -ekey[50];
        }
        dkey[i + 3] = mulInv(ekey[j + 5]);
    }
    for (i = 0; i < 52; i++)
    {
        printf("%x", dkey[i]);
    }
    printf("\n");
}
/********************************************************/
/* IDEA 暗号化及び復号化を行う関数 */
/********************************************************/
void IdeaChipher(unsigned short plain[4], unsigned short chipher[4], unsigned short key[52])
{
    unsigned int r, x1, x2, x3, x4, kk, t1, t2, a;
    x1 = plain[0];
    x2 = plain[1];
    x3 = plain[2];
    x4 = plain[3];
    for (r = 0; r < 48; r += 6)
    {
        x1 = mul(x1, key[r]);
        x2 = lo16(x2 + key[r + 1]);
        x3 = lo16(x3 + key[r + 2]);
        x4 = mul(x4, key[r + 3]);
        kk = mul(key[r + 4], (x1 ^ x3));
        t1 = mul(key[r + 5], (lo16(kk + (x2 ^ x4))));
        t2 = lo16(kk + t1);
        x1 = x1 ^ t1;
        a = x2 ^ t2;
        x2 = x3 ^ t1;
        x3 = a;
        x4 = x4 ^ t2;
    }

    chipher[0] = mul(x1, key[48]);
    chipher[1] = lo16(x3 + key[49]);
    chipher[2] = lo16(x2 + key[50]);
    chipher[3] = mul(x4, key[51]);
}
int main(void)
{
    int i, j, k;
    unsigned short input[4], output[4], tete[4];
    unsigned short ekey[52], dkey[52], key[8], getChipher[100];
    char getkey[18], getText[100];
    for (i = 0; i < 18; i++)
        getkey[i] = '\0';
    for (i = 0; i < 100; i++)
        getText[i] = '\0';
    /* 副鍵のもととなる文字列の入力 */
    printf("Please Input Key -> ");
    scanf("%s", getkey);
    /* 1文字を連結して16ビットにしている */
    for (i = 0, j = 0; i < 8; i++, j += 2)
    {
        key[i] = (getkey[j] << 8) | getkey[j + 1];
    }
    printf("\n");
    /* 副鍵の生成 */
    IdeaKeyGen(key, ekey, dkey);
    printf("\n");
    /* 平文の入力 */
    printf("Please Input Plain Text -> ");
    scanf("%s", getText);
    printf("\n");

    printf("平文:");
    for (i = 0; i < 18; i++)
    {
        printf("%d ", getText[i]);
    }
    printf("\n");
    /* 暗号化処理を行う。 */
    /* 16*4 のブロック単位で暗号処理する */
    k = 0;
    printf("暗号:");
    for (i = 0; i < 18; i += 4)
    {
        input[0] = getText[i];
        input[1] = getText[i + 1];
        input[2] = getText[i + 2];
        input[3] = getText[i + 3];
        IdeaChipher(input, output, ekey);
        for (j = 0; j < 4; j++)
        {
            getChipher[k] = output[j];
            k++;
            printf("%x", output[j]);
        }
    }
    printf("\n");
    /* 復号化処理を行う。 */
    /* 16*4 のブロック単位で暗号処理する */
    printf("復号:");
    for (i = 0; i < 18; i += 4)
    {
        output[0] = getChipher[i];
        output[1] = getChipher[i + 1];
        output[2] = getChipher[i + 2];
        output[3] = getChipher[i + 3];
        IdeaChipher(output, tete, dkey);
        for (j = 0; j < 4; j++)
        {
            printf("%c", tete[j]);
        }
    }
    printf("\n");
    return (0);
}