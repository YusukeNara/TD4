#include "RakiUtility.h"
#include "NY_random.h"

int rutility::GetDigits(float value, int left, int right)
{
    double mod_value;
    int result;

    /* 事前に絶対値を求めておく */
    value = fabs(value);

    /* n桁目以下の桁を取得 */
    mod_value = fmod(value, pow(10, right + 1));

    /* m桁目以上の桁を取得 */
    result = int(mod_value / pow(10, left));

    return result;
}

RVector3 rutility::randomRV3(RVector3 pos1, RVector3 pos2)
{
    RVector3 vmax;
    RVector3 vmin;
    
    if (pos1.x > pos2.x) {
        vmax.x = pos1.x;
        vmin.x = pos2.x;
    }
    else {
        vmax.x = pos2.x;
        vmin.x = pos1.x;
    }
    if (pos1.y > pos2.y) {
        vmax.y = pos1.y;
        vmin.y = pos2.y;
    }
    else {
        vmax.y = pos2.y;
        vmin.y = pos1.y;
    }
    if (pos1.z > pos2.z) {
        vmax.z = pos1.z;
        vmin.z = pos2.z;
    }
    else {
        vmax.z = pos2.z;
        vmin.z = pos1.z;
    }

    RVector3 result;

    result.x = NY_random::floatrand_sl(vmax.x, vmin.x);
    result.y = NY_random::floatrand_sl(vmax.y, vmin.y);
    result.z = NY_random::floatrand_sl(vmax.z, vmin.z);

    return result;
}

std::wstring rutility::charTowstring(const char* orig)
{
    // 確保するワイド文字用バッファのサイズは、バイト数ではなく文字数を指定する。
    size_t newsize = strlen(orig) + 1;
    wchar_t* wc = new wchar_t[newsize];

    // 変換.
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wc, newsize, orig, _TRUNCATE);

    std::wstring ret(wc);
    return ret;
}
