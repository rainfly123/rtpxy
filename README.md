# rtpxy

a tool used for relay UDP/RTP multicast stream to a new multicast addres, via another net interface 


## Install
1.[Install libcyaml](https://github.com/tlsa/libcyaml)   
2.sudo apt install libuv1-dev  


## Run
./rtpxy [sdev] [url] [dev] [dst]   
example: ./rtpxy eth1 239.1.1.33:5156 eth2 239.2.2.2:5146   
relay the stream from 239.1.1.33:5156 via eth1  to 239.2.2.2:5146 via eth2   

## use config.yaml

you can also config your conf.yaml , run the service through ./rtpxy-wrap. it will start rtpxy automatically,(one process per channel) 
