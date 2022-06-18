#include "standardCalculation.hpp"

using namespace std;
//最大公約数の計算を行う関数
unsigned int standardCalculation::gcd(unsigned int a, unsigned int b)
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

//最小公倍数の計算を行う関数
unsigned int standardCalculation::lcm(unsigned int a, unsigned int b)
{
    return a + b / gcd(a, b);
}

//バイナリ法を使った冪剰余の計算関数
unsigned int standardCalculation::modpow(unsigned int a, unsigned int e, unsigned int n)
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