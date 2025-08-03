#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <pcap.h>
#include "Arp.h"
#include "Ether.h"
#include <netinet/in.h>
#include <unistd.h>
const uint8_t fmac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
const uint8_t zmac[6]={0x00,0x00,0x00,0x00,0x00,0x00};
const struct Ether *ethernet;
const struct ARP *arp;
#pragma pack(push, 1)
struct ARP_PACKET{
    Ether ether;
    ARP arp;
};
#pragma pack(pop)
typedef struct {//그냥 구조체랑 다른게 별명을 붙여줄 수 있음
	char* dev_;
} Param;

Param param = {
	.dev_ = NULL
};
void usage() {
	printf("syntax: pcap-test <interface>\n");
	printf("sample: pcap-test wlan0\n");
}
bool parse(Param* param, int argc, char* argv[]) {
	if (argc != 4) {
		usage();
		return false;
	}
	param->dev_ = argv[1];
	return true;
}
int main(int argc, char* argv[]){
    if (argc != 4) {
		usage();
		return EXIT_FAILURE;
	}
	char* dev = argv[1];
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* pcap = pcap_open_live(dev, BUFSIZ, 1, 1, errbuf);
	if (pcap == nullptr) {
		fprintf(stderr, "couldn't open device %s(%s)\n", dev, errbuf);
		return EXIT_FAILURE;
	}
    ARP_PACKET arp_packet;//request
    arp_packet.ether.smac.set_mac(arp_packet.ether.smac.get_mac_address(argv[1]));
	arp_packet.ether.dmac.set_mac(fmac);//6크기에 8이들어가서 seg fault남--주소가 들어가고 있음.--> 지역 변수를 반환해서 문제였음.
	//왜 Mac의 변수를 공유할까 왜 싱글톤이 되었을까 난 그렇게 만든 적이 없는 데
	arp_packet.ether.type=arp_packet.ether.arp;

    arp_packet.arp.code=htons(0x0001);
    arp_packet.arp.smac.set_mac(arp_packet.ether.smac.get_mac_address(argv[1]));//이렇게 안하면 FFFFFFFF로 고정됨 왜지?
    arp_packet.arp.sip.set_ip(argv[2]);
    arp_packet.arp.tmac.set_mac(zmac);
    arp_packet.arp.tip.set_ip(argv[3]);
    // std::cout<<sizeof(ARP_PACKET)<<std::endl;
    for(int i=0;i<100;i++){
		int res = pcap_sendpacket(pcap, reinterpret_cast<const u_char*>(&arp_packet), sizeof(ARP_PACKET));
		sleep(0.1);
		if (res != 0) {
			fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(pcap));
		}
	}
	struct pcap_pkthdr* header;
	const __u_char* packet;
	
	// std::cout<<5<<std::endl;
	// arp_packet.arp.tip={172,20,10,3};
	while(true){
		int res = pcap_next_ex(pcap, &header, &packet);
		if (res == 0) continue;
		if (res == PCAP_ERROR || res == PCAP_ERROR_BREAK) {
			printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(pcap));
			break;
		}
		ethernet = (struct Ether*)(packet);
		arp = (struct ARP*)(packet+14);
		if(arp->code==2){
			arp_packet.ether.smac.set_mac(ethernet->smac.mac);
			// std::cout<<1<<std::endl;
			arp_packet.arp.smac.set_mac(ethernet->dmac.mac);
			arp_packet.arp.code=htons(2);
			std::cout<<1<<std::endl;
			// std::cout<<1<<std::endl;
			res = pcap_sendpacket(pcap, reinterpret_cast<const u_char*>(&arp_packet), sizeof(ARP_PACKET));
			if (res != 0) {
				fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(pcap));
			}
		
		}
		
		sleep(1);
	}
	
    
}



