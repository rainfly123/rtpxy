#ifndef _C_CONF_H_
#define _C_CONF_H_

/* Structure for storing a channels item */
struct channels {
    char *type;
    char *name;
    //input 
    char *url;
    char *sdev;
    //output
    char *dst;
    char *dev;
};

/* Structure for storing a conf */
struct conf {
    char *key;
    struct channels *chans;
    unsigned chans_count;
};

struct conf * parse_conf(void);

#endif
