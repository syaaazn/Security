#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int gcd(unsigned int a, unsigned int b);
unsigned int lcm(unsigned int a, unsigned int b);
unsigned int modpow(unsigned int a, unsigned int e, unsigned int n);
void keyGenerate(unsigned int *n, unsigned int *e, unsigned int *d);
void encrypt(char *plain, int size, unsigned int *encrypted_ascii, unsigned int n, unsigned int e);
char *decrypt(unsigned int *encrypted_ascii, int size, unsigned int n, unsigned int d);

int main()
{
    printf("plain text : \n");
    char c;
    char *plain;
    int count = 0;
    while ((c = getchar()) != '\n')
    {
        plain = (char *)realloc(plain, sizeof(char) * (count + 1));
        *(plain + count++) = c;
    }

    unsigned int n, e, d;
    keyGenerate(&n, &e, &d);
    printf("generated keys : \n");
    printf("n : %u, e : %u, d : %u \n", n, e, d);

    printf("encrypted asciicode :\n");
    unsigned int encrypted_ascii[count];
    encrypt(plain, count, encrypted_ascii, n, e);
    for (int i = 0; i < sizeof(encrypted_ascii) / sizeof(int); i++) //暗号化した平文を出力
    {
        if (i >= (sizeof(encrypted_ascii)) / sizeof(int) - 1)
        {
            printf(" %d\n", encrypted_ascii[i]);
        }
        else
        {
            printf("%d, ", encrypted_ascii[i]);
        }
    }
    printf("decrypted text :\n");
    printf("%s", decrypt(encrypted_ascii, count, n, d));

    free(plain);

    return 0;
}

unsigned int gcd(unsigned int a, unsigned int b)
{
    if (b == 0)
    {
        return a;
    }
    else
    {
        return gcd(b, a % b);
    }
}

unsigned int lcm(unsigned int a, unsigned int b)
{
    return a + b / gcd(a, b);
}

unsigned int modpow(unsigned int a, unsigned int e, unsigned int n)
{
    unsigned int result = 1;
    while (e > 0)
    {
        if ((e & 1) == 1)
        {
            result = (result * a) % n;
        }
        e >>= 1;
        a = (a * a) % n;
    }
    return result;
}

void keyGenerate(unsigned int *n, unsigned int *e, unsigned int *d)
{
    unsigned const int p = 13;              //指定されている素数
    unsigned const int q = 19;              //指定されている素数
    *n = p * q;                             //公開鍵
    unsigned int euler = (p - 1) * (q - 1); // Euler(n) = (p - 1) * (q -1)の計算結果
    unsigned int key = 2;                   //公開鍵の生成(Euler(n)と互いに素になるeが見つかるまでループさせる)
    while (gcd(euler, key) != 1)
    {
        key++;
    }
    *e = key;
    key = 2; //秘密鍵の生成((e * d) mod Euler(n) = 1 を満たすdが見つかるまでループさせる)
    while (*e * key % euler != 1)
    {
        key++;
    }
    *d = key;
}

void encrypt(char *plain, int size, unsigned int *encrypted_ascii, unsigned int n, unsigned int e)
{
    for (int i = 0; i < size; i++)
    {
        unsigned int ascii_trans = (int)(plain[i]);       //平文をASCIIコードに変換
        encrypted_ascii[i] = (modpow(ascii_trans, e, n)); //暗号化して配列に格納(M^eの処理は愚直に行うと桁溢れする懸念があるので別途modpow関数を利用している)
    }
}

char *decrypt(unsigned int *encrypted_ascii, int size, unsigned int n, unsigned int d)
{
    char *decrypted_str = (char *)malloc(sizeof(char) * size);

    for (int i = 0; i < size; i++)
    {
        unsigned int decrypted_ascii = modpow(encrypted_ascii[i], d, n); //復号化(C^dの処理は愚直に行うと桁溢れする懸念があるので別途modpow関数を利用している)
        decrypted_str[i] = (char)decrypted_ascii;                        //文字に変換
    }
    return decrypted_str;
}
