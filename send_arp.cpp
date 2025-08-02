#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <pcap.h>
#include "Arp.h"
#include "Ether.h"
#include <netinet/in.h>

const uint8_t fmac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
const uint8_t zmac[6]={0x00,0x00,0x00,0x00,0x00,0x00};

struct ARP_PACKET{
    Ether ether;
    ARP arp;
};
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
    ARP_PACKET arp_packet;
	uint8_t *my_mac= arp_packet.ether.smac.get_mac_address(argv[1]);
    arp_packet.ether.smac.set_mac(my_mac);
	arp_packet.ether.dmac.set_mac(fmac);//6크기에 8이들어가서 seg fault남--주소가 들어가고 있음.--> 지역 변수를 반환해서 문제였음.
    arp_packet.ether.type=arp_packet.ether.arp;

    arp_packet.arp.code=0x0100;
    arp_packet.arp.smac.set_mac(my_mac);
    arp_packet.arp.sip.set_ip(argv[2]);
    arp_packet.arp.tmac.set_mac(zmac);
    arp_packet.arp.tip.set_ip(argv[3]);
    std::cout<<sizeof(ARP_PACKET)<<std::endl;
    int res = pcap_sendpacket(pcap, reinterpret_cast<const u_char*>(&arp_packet), sizeof(ARP_PACKET));
	if (res != 0) {
		fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(pcap));
	}
    
}



