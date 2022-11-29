ALL:
	gcc -g -o rtpxy multicast.c iportsplit.c ipaddr.c conf.c -luv -lcyaml
	gcc  -o rtpxy-wrap main.c conf.c -lcyaml
