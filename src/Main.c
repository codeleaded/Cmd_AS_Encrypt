#if defined __linux__
#include "/home/codeleaded/System/Static/Library/Kryptography.h"
#elif defined _WINE
#include "/home/codeleaded/System/Static/Library/Kryptography.h"
#elif defined _WIN32
#include "F:/home/codeleaded/System/Static/Library/Kryptography.h"
#endif

int main() {

    BigNumber bn1 = BigNumber_Parse_Hex("00000000000000000000000000000000");
    BigNumber bn2 = BigNumber_Parse_Hex("00000000000000000000000000000000");
    
    BigNumber bn3 = BigNumber_Add(&bn1,&bn2);
    BigNumber_Free(&bn3);

    BigNumber_Free(&bn2);
    BigNumber_Free(&bn1);

    return 0;
}