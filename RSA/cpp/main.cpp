#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include "RSA.hpp"

int main()
{
    RSA rsa;

    std::cout << "plain text : " << std::endl;
    std::string plain;
    getline(std::cin, plain); //平文を入力

    unsigned int n, e, d;
    std::tie(n, e, d) = rsa.keyGenerate(); //鍵を生成
    std::cout << "generated keys : " << std::endl;
    std::cout << "n = " << n << ", e = " << e << ", d = " << d << std::endl;

    std::cout << "encrypted asciicode :" << std::endl;
    std::vector<unsigned int> encrypted_ascii = rsa.encrypt(plain, n, e); //平文を暗号化
    for (int i = 0; i < encrypted_ascii.size(); i++)                      //暗号化した平文を出力
    {
        if (i >= (encrypted_ascii.size() - 1))
        {
            std::cout << encrypted_ascii[i] << std::endl;
        }
        else
        {
            std::cout << encrypted_ascii[i] << "," << std::flush;
        }
    }

    std::cout << "decrypted text :" << std::endl;
    std::cout << rsa.decrypt(encrypted_ascii, n, d) << std::endl; //復号化して出力

    return 0;
}