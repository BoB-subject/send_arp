#include<stdint.h>
#include<string.h>
#include <stdlib.h>
#include "Mac.h"
#include "Ip.h"   


struct ARP{
    uint16_t HW_type=0x01;
    uint16_t PT_type=0x08;
    uint16_t code;
    uint8_t HW_len=0x06;
    uint8_t PT_len=0x04;
    
    MAC smac;
    IP sip;
    MAC tmac;
    IP tip;
};
