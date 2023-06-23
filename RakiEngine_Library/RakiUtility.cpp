#include "RakiUtility.h"

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
