#include <iostream>

class standardCalculation
{
public:
    unsigned int gcd(unsigned int a, unsigned int b);                    //最大公約数の計算を行う関数
    unsigned int lcm(unsigned int a, unsigned int b);                    //最小公倍数の計算を行う関数
    unsigned int modpow(unsigned int a, unsigned int e, unsigned int n); //バイナリ法を使った冪剰余の計算関数
};