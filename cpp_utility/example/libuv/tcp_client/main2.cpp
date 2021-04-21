#include <iostream>
#include<stdio.h>  
#include<string.h>  
#include<errno.h>  
#include <uv.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment (lib,"Advapi32.lib")
#pragma comment (lib,"Iphlpapi.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "userenv.lib")

void echo_read(uv_stream_t* server, ssize_t nread, const uv_buf_t* buf) {
    if (nread == -1) {
        fprintf(stderr, "error echo_read");
        return;
    }

    printf("result: %s\n", buf->base);
}

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

void on_write_end(uv_write_t* req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }
    uv_read_start(req->handle, alloc_buffer, echo_read);
}

void on_connect(uv_connect_t* req, int status) {
    if (status == -1) {
        fprintf(stderr, "error on_write_end");
        return;
    }
    char buffer[100];
    uv_buf_t buf = uv_buf_init(buffer, sizeof(buffer));
    char* message = (char*)"hello";
    buf.len = strlen(message);
    buf.base = message;
    uv_stream_t* tcp = req->handle;
    uv_write_t write_req;
    int buf_count = 1;
    uv_write(&write_req, tcp, &buf, buf_count, on_write_end);
}

int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_tcp_t* socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, socket);

    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    struct sockaddr_in dest;
    uv_ip4_addr("0.0.0.0", 7000, &dest);

    int r = uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);


    if (r)
    {
        fprintf(stderr, "connect error %s\n", uv_strerror(r));
        return 1;
    }

    return uv_run(loop, UV_RUN_DEFAULT);

}