send_arp:
	g++ send_arp.cpp -o send_arp -lpcap

clean:
	rm send_arp