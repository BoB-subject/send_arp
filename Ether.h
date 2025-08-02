#include <stdint.h>
#include "Mac.h"


class Ether{
public:
    enum type{
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





