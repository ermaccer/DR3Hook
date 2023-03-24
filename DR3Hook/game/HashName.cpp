#include "HashName.h"
#include "..\utils\addr.h"

tHashID* cHashName::HashString(tHashID* result, const char* text, int lenght)
{
    return  ((tHashID*(__fastcall*)(tHashID*, const char*, int))_addr(0x140DA0AE0))(result, text, lenght);
}
