#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include <cstring>
#pragma once

struct IP{
    uint8_t ip[4];
    void set_ip(char *ip_){
        char *ptr = strtok(ip_, ".");
        int i=0;
        // std::cout<<ptr<<std::endl;
        while(ptr!=NULL){
            ip[i]=atoi(ptr);
            // std::cout<<ptr<<std::endl;
            ptr=strtok(NULL, ".");
            i++;
        }

    }    
};