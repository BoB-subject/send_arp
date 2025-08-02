#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#pragma once

struct IP{
    uint8_t ip[4];
    void set_ip(char *ip_){
        
        int i=0;
        char *ip_token;
        
        for(i=0;i<4;i++){//strtok(".",ip_)에서 세그 폴트가 남 strtok(ip_,".")이거였음....
            this->ip[i]=atoi(strtok(ip_,"."));//앞에서 하나씩만 잘라짐
            
            i++;
        }
    }    
};