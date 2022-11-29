#include <stdio.h>
#include <string.h>
#include <uv.h>

static uv_interface_address_t *info;
static int count;

char *get_ip(char *dev) {
    if (info == NULL){
       uv_interface_addresses(&info, &count);
    }
    if (dev != NULL){
         static char buf[256];

         for (int i = 0; i< count; i++) {
             uv_interface_address_t interface = info[i];
             if (strcmp(interface.name, dev) == 0) {
               if (interface.address.address4.sin_family == AF_INET) {
                 uv_ip4_name(&interface.address.address4, buf, sizeof(buf));
               }
               else if (interface.address.address4.sin_family == AF_INET6) {
                 uv_ip6_name(&interface.address.address6, buf, sizeof(buf));
               }
               break;
             }
         }
         return buf;
    }
    return NULL;
}
/*
int main() {
    printf("%s\n", get_ip("eth0"));
    printf("%s\n", get_ip("lo"));
}
*/
