#include "NY_random.h"

std::random_device NY_random::rnd;

int NY_random::intrand_nl()
{
    return rnd();
}

int NY_random::intrand_sl(int max, int min)
{
    int maxtmp, mintmp;
    //逆の場合
    if (max < min) {
        maxtmp = min;
        mintmp = max;
    }
    else {
        maxtmp = max;
        mintmp = min;
    }

    if (min == max) {
        return min;
    }

    //メルセンヌ・ツイスタ乱数生成器
    std::mt19937 mt(rnd());
    //引数指定範囲の一様乱数
    std::uniform_int_distribution<> rand(mintmp, maxtmp);

    return rand(mt);
}

double NY_random::doublerand_sl(double max, double min)
{
    //メルセンヌ・ツイスタ乱数生成器
    std::mt19937 mt(rnd());
    //引数指定範囲の一様乱数
    std::uniform_real_distribution<> rand(min, max);

    return rand(mt);
}

float NY_random::floatrand_sl(float max, float min)
{
    //メルセンヌ・ツイスタ乱数生成器
    std::mt19937 mt(rnd());
    //引数指定範囲の一様乱数
    std::uniform_real_distribution<> rand(min, max);

    return static_cast<float>(rand(mt));
}
