#include "RakiUtility.h"

int rutility::GetDigits(float value, int left, int right)
{
    double mod_value;
    int result;

    /* –‘O‚Éâ‘Î’l‚ğ‹‚ß‚Ä‚¨‚­ */
    value = fabs(value);

    /* nŒ…–ÚˆÈ‰º‚ÌŒ…‚ğæ“¾ */
    mod_value = fmod(value, pow(10, right + 1));

    /* mŒ…–ÚˆÈã‚ÌŒ…‚ğæ“¾ */
    result = int(mod_value / pow(10, left));

    return result;
}
