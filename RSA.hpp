#include <iostream>
#include <tuple>
#include <string>
#include <cmath>
#include <vector>
#include "standardCalculation.hpp"

class RSA
{
public:
    //公開鍵と秘密鍵の生成を行う関数
    std::tuple<unsigned int, unsigned int, unsigned int> keyGenerate();
    //公開鍵によって暗号化を行う関数 平文をASCIIコードに変換し、コードを暗号化した数字を配列にして返す
    std::vector<unsigned int> encrypt(std::string plain, unsigned int n, unsigned int e);
    //秘密鍵によって複合化を行う関数 暗号化された数字の配列から復号処理を行い、最後にASCIIコードを参照して平文を返す
    std::string decrypt(std::vector<unsigned int> encrypted_ascii, unsigned int n, unsigned int d);

private:
    standardCalculation stdc;
};