#include <stdint.h>
#include "Mac.h"


class Ether{
public:
    enum type:uint16_t{//타입지정 안해주면 데이터 값으로 타입 지정함.난 int값으로 지정된 듯
        ipv4 = 0x0008,
        ipv6 = 0xdd86,
        arp  = 0x0608
    };
    MAC smac; 
    MAC dmac;    
    type type;

    bool is_type_arp(Ether &ether){
    
        if (ether.type==arp){
            return true;
        }
        else{
            return false;
        }
    }
};



