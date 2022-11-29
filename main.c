/*
 * uv_thread_t is pthread_t, and is type unsigned long int.
 * File: main.c
 * Created on: 2022-11-28
 * Author : rainfly 18910158363
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "conf.h"
 
void start(struct channels *chan) {
    char cmd[256];
    sprintf(cmd, "rtpxy %s %s %s %s", chan->sdev, chan->url+strlen("rtp://"), chan->dev, chan->dst);
    system(cmd);
}

int main(int argc, char **argv)
{
        struct conf * conf = parse_conf();
        if (conf == NULL) {
            printf("config error\n");
            return -1;
        }

        /*
	uv_thread_t threads[MAX_CHAN];
        for (unsigned i = 0; i < conf->chans_count; i++) {
	    uv_thread_create(threads + i, relay_thread, &conf->chans[i]);	
        }       

        for (unsigned i = 0; i < conf->chans_count; i++) {
	    uv_thread_join(threads + i);	//thread join
        }
        */
         for (unsigned i = 0; i < conf->chans_count; i++) {
             if (strcmp(conf->chans[i].type, "rtp") != 0) {
                 continue;
             }
             printf("%u. type: %s\n", i + 1, conf->chans[i].type);
             printf("name: %s\n", conf->chans[i].name);
             printf("%s\n", conf->chans[i].sdev);
             printf("%s\n ", conf->chans[i].url);
             printf("%s\n ", conf->chans[i].dev);
             printf("%s\n", conf->chans[i].dst);
             start(conf->chans + i);
         }
    while (1) 
        sleep(60); 
}

