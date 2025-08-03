#include <stdint.h>
#include <string>
#include <regex>
#include <fstream>
#include <streambuf>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <unistd.h>
#pragma once

#define MAC_ADDR_LEN 6
struct MAC{

    uint8_t mac[6];
    // uint8_t result_mac[6];//6크기에 8이들어가서 seg fault남--주소가 들어가고 있음.--> 지역 변수를 반환해서 문제였음. 전역변수를 선언하는 것으로 해결함
    void set_mac(const uint8_t bytes[]){
        for(int i=0;i<6;i++){
            this->mac[i]=bytes[i];
        }
    };
    void set_mac(uint8_t bytes[]){
        for(int i=0;i<6;i++){
            this->mac[i]=bytes[i];
        }
    };

    uint8_t* get_mac_address(char *argv){//주소값을 반환해서 segfault가 남
        int sock;
        struct ifreq ifr;
        char *mac_ = NULL;
        memset(&ifr, 0x00, 6);
        strcpy(ifr.ifr_name, argv);

        // Unix 도메인 소켓 하나를 생성한다.
        int fd = socket(AF_UNIX, SOCK_DGRAM, 0);

        if (fd < 0) {
            perror("socket creation error");
            return 0;
        }

        // 이 소켓을 이용해 ifr 을 보낸다.
        // 두번째 파라미터는 request code, 이름은 SIOCGIFHWADDR (이것은 이 code는 device-dependent임)  32     // 세번째 파라미터는, 전달할 매개변수. 이 매개변수 안에 응답한 정보가 들어감.
        if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
            perror("ioctl error");
            return 0;
        }
        mac_ = ifr.ifr_hwaddr.sa_data;
        for(int i=0;i<6;i++){
            mac[i]=mac_[i];
            // std::cout<<result_mac[i]<<std::endl;
        }
        printf("%s: %02x:%02x:%02x:%02x:%02x:%02x\n", ifr.ifr_name, mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        close(fd);
        return mac;
    }
};