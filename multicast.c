/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "uv.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void split(char *src, const char *separator, char **dest, int *num);
char *get_ip(char *dev);


static uv_udp_t server;
static uv_udp_t client;
static uv_loop_t *loop;
static struct sockaddr_in out_addr;

static uint32_t received;
static uint32_t old_received;

static void close_cb(uv_handle_t* handle) {
}

typedef struct {
  uv_udp_send_t req;
  uv_buf_t buf;
} write_req_t;


static void sv_send_cb(uv_udp_send_t* req, int status) {
   //uv_close((uv_handle_t*) req->handle, close_cb);

    /* Free the read/write buffer and the request */
    write_req_t* wr;
    wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

static void cl_recv_cb(uv_udp_t* handle,
                       ssize_t nread,
                       const uv_buf_t* buf,
                       const struct sockaddr* addr,
                       unsigned flags) {
   if (nread < 0)
   {
        fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) handle, NULL);
        free(buf->base);
        return;
   }

   if (nread == 0) {
     /* Everything OK, but nothing read. */
     free(buf->base);
     return;
   }
   write_req_t *wr;
   wr = (write_req_t*) malloc(sizeof *wr);
   wr->buf = uv_buf_init(buf->base, nread);
   uv_udp_send(&wr->req,
                  &client,
                  &wr->buf,
                  1,
                  (const struct sockaddr*)&out_addr,
                  sv_send_cb);
   received += nread;
}
 

static void alloc_cb(uv_handle_t* handle,
                     size_t suggested_size,
                     uv_buf_t* buf) {
  char *slab = (char*)malloc(65535);
  buf->base = slab;
  buf->len = 65535;
}

static void timer_cb(uv_timer_t *handle) {
    //无限循环计时器
    //uv_timer_stop(handle);
    //用完一定要调用uv_close,不然会内存泄露
    //uv_close((uv_handle_t*)handle, NULL);
    if (received == old_received){
        uv_timer_stop(handle);
        uv_close((uv_handle_t*) &server, NULL);
        uv_close((uv_handle_t*) &client, NULL);
        uv_close((uv_handle_t*)handle, NULL);
        uv_stop(loop);
        uv_loop_close(loop);
    }else {
        old_received = received;
    }
}
 
int rtp_main(char *sdev, char *url, char *ddev, char *dst) {

  int r;
  uv_buf_t buf;
  struct sockaddr_in recv_addr;
  char *input_interface_ip;
  char* src_ip;
  char *src_port;
  int num;

  char *out_interface_ip;
  char *dst_ip;
  char *dst_port;

  loop = uv_default_loop();

  printf("%s %s %s %s\n", sdev, url, ddev, dst);
  input_interface_ip = strdup(get_ip(sdev));
  out_interface_ip = strdup(get_ip(ddev));
  
  char *parse_src[2] = {0};
  split(url, ":", parse_src, &num);
  src_ip = parse_src[0];
  src_port = parse_src[1];

  char *parse_out[2] = {0};
  split(dst, ":", parse_out, &num);
  dst_ip = parse_out[0];
  dst_port = parse_out[1];
  //recv
  printf("interface: %s %s \n", input_interface_ip, out_interface_ip);
  printf("input: %s %s \n", src_ip, src_port);
  printf("out: %s %s \n", dst_ip, dst_port);
  r = uv_udp_init(loop, &server);
  r = uv_ip4_addr(src_ip, atoi(src_port), &recv_addr);
  r = uv_udp_bind(&server, (const struct sockaddr*)&recv_addr, UV_UDP_REUSEADDR);

  r = uv_udp_set_multicast_interface(&server, input_interface_ip);
  r = uv_udp_set_membership(&server, src_ip, input_interface_ip, UV_JOIN_GROUP);
  r = uv_udp_recv_start(&server, alloc_cb, cl_recv_cb);

  //send 
  r = uv_udp_init(loop, &client);
  r = uv_ip4_addr(dst_ip, atoi(dst_port), &out_addr);
  struct sockaddr_in b_addr;
  r = uv_ip4_addr(out_interface_ip, 0, &b_addr);
  r = uv_udp_bind(&client, (const struct sockaddr*)&b_addr, UV_UDP_REUSEADDR);
  r = uv_udp_set_multicast_ttl(&client, 128);
  r = uv_udp_set_multicast_loop(&client, 0);

  r = uv_udp_set_multicast_interface(&client, out_interface_ip);
  r = uv_udp_set_membership(&client, dst_ip, out_interface_ip, UV_JOIN_GROUP);


 /*
  uv_udp_send_t req;
  uv_buf_t sndbuf;
  sndbuf = uv_buf_init("PING", 4);
  r = uv_udp_send(&req,
                  &client,
                  &sndbuf,
                  1,
                  (const struct sockaddr*)&out_addr,
                  sv_send_cb);
*/
  uv_timer_t timer_req;
  uv_timer_init(loop, &timer_req);
  uv_timer_start(&timer_req, timer_cb, 10000, 10000);
 
  free(input_interface_ip);
  free(out_interface_ip);
  uv_run(loop, UV_RUN_DEFAULT);
  return 0;
}

int main(int argc , char **argv){
    daemon(1, 0);
    received = old_received = 0;
    char *sdev, *url, *dev, *dst;    
    while (1) {
        sdev = strdup(argv[1]);
        url = strdup(argv[2]);
        dev = strdup(argv[3]);
        dst = strdup(argv[4]);
        rtp_main(sdev, url, dev, dst);
        free(sdev);
        free(url);
        free(dev);
        free(dst);
        sleep(3);
    }
}

