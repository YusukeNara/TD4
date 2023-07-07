#include "RakiUtility.h"
#include "NY_random.h"

int rutility::GetDigits(float value, int left, int right)
{
    double mod_value;
    int result;

    /* Ž–‘O‚Éâ‘Î’l‚ð‹‚ß‚Ä‚¨‚­ */
    value = fabs(value);

    /* nŒ…–ÚˆÈ‰º‚ÌŒ…‚ðŽæ“¾ */
    mod_value = fmod(value, pow(10, right + 1));

    /* mŒ…–ÚˆÈã‚ÌŒ…‚ðŽæ“¾ */
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
