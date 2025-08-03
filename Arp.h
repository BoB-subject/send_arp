#include<stdint.h>
#include<string.h>
#include <stdlib.h>
#include "Mac.h"
#include "Ip.h"   

#pragma pack(push, 1) // 구조체 패딩 제거
struct ARP{
    uint16_t HW_type=htons(0x0001);
    uint16_t PT_type=htons(0x0800);
    uint8_t HW_len=06;
    uint8_t PT_len=04;
    uint16_t code;
    MAC smac;
    IP sip;
    MAC tmac;
    IP tip;
};
#pragma pack(pop)