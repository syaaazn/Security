#include "RSA.hpp"

//公開鍵(nとe)と秘密鍵(d)を返す関数 今回はpとqが固定のため定数を返す関数となる
std::tuple<unsigned int, unsigned int, unsigned int> RSA::keyGenerate()
{
    unsigned const int p = 13;              //指定されている素数
    unsigned const int q = 19;              //指定されている素数
    unsigned int n = p * q;                 //公開鍵
    unsigned int euler = (p - 1) * (q - 1); // Euler(n) = (p - 1) * (q -1)の計算結果
    unsigned int e = 2;                     //公開鍵の生成(Euler(n)と互いに素になるeが見つかるまでループさせる)
    while (stdc.gcd(euler, e) != 1)
    {
        e++;
    }
    unsigned int d = 2; //秘密鍵の生成((e * d) mod Euler(n) = 1 を満たすdが見つかるまでループさせる)
    while (e * d % euler != 1)
    {
        d++;
    }
    return std::forward_as_tuple(n, e, d);
}

/*渡した平文と公開鍵二つによって平文を暗号化して一文字ずつ配列に格納して返す関数
平文をASCIIコードに直してから暗号文の式に代入して暗号化している*/
std::vector<unsigned int> RSA::encrypt(std::string plain, unsigned int n, unsigned int e)
{
    std::vector<unsigned int> encrypted_ascii;
    for (int i = 0; i < plain.size(); i++)
    {
        unsigned int ascii_trans = static_cast<unsigned int>(plain[i]); //平文をASCIIコードに変換
        encrypted_ascii.push_back(stdc.modpow(ascii_trans, e, n));      //暗号化して配列に格納(M^eの処理は愚直に行うと桁溢れする懸念があるので別途modpow関数を利用している)
    }

    return encrypted_ascii;
}
/*渡した暗号化された数字と公開鍵nと秘密鍵dによって復号処理を行って文字列で返す関数
復号化した数字をASCIIコードから元の文字に変換している*/
std::string RSA::decrypt(std::vector<unsigned int> encrypted_ascii, unsigned int n, unsigned int d)
{
    std::string decrypted_str;
    for (int i = 0; i < encrypted_ascii.size(); i++)
    {
        unsigned int decrypted_ascii = stdc.modpow(encrypted_ascii[i], d, n); //復号化(C^dの処理は愚直に行うと桁溢れする懸念があるので別途modpow関数を利用している)
        decrypted_str += static_cast<char>(decrypted_ascii);                  //文字に変換
    }
    return decrypted_str;
}