#include "types.h"
#include "revision.h"
#include "define.h"



#ifdef TSTAT7_ARM
uint8 const hexinfo[20] __attribute__((at(0x8008200)))  = {'T', 'e', 'm', 'c', 'o', 'T', 'S', 'T', 'A', 'T', '7', 'A', 'R', 'M', 0, FW_VER_LOW, FW_VER_HIGH,  0, 0, 103};
#else
uint8 const hexinfo[20] __attribute__((at(0x8008200)))  = {'T', 'e', 'm', 'c', 'o', 'T', 'S', 'T', 'A', 'T', '8', 0, 0, 0, 0, FW_VER_LOW, FW_VER_HIGH,  0, 0, 103};
#endif


