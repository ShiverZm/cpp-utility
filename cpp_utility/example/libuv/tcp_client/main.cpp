//#include <iostream>
//#include<stdio.h>  
//#include<string.h>  
//#include<errno.h>  
//#include <uv.h>
//
//#pragma comment(lib,"ws2_32.lib")
//#pragma comment (lib,"Advapi32.lib")
//#pragma comment (lib,"Iphlpapi.lib")
//#pragma comment(lib, "Psapi.lib")
//#pragma comment(lib, "user32.lib")
//#pragma comment(lib, "userenv.lib")
//
//uv_loop_t* loop;
//#define DEFAULT_PORT 7000
//
//uv_tcp_t mysocket;
//
//
//uv_buf_t iov;
//char buffer[128];
//
//uv_fs_t read_req;
//uv_fs_t open_req;
//void on_read(uv_fs_t* req);
//
//void on_write(uv_write_t* req, int status)
//{
//    if (status < 0)
//    {
//        fprintf(stderr, "Write error: %s\n", uv_strerror(status));
//        uv_fs_t close_req;
//        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
//        uv_close((uv_handle_t*)&mysocket, NULL);
//        exit(-1);
//    }
//    else
//    {
//        uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
//    }
//}
//
//void on_read(uv_fs_t* req)
//{
//    if (req->result < 0)
//    {
//        fprintf(stderr, "Read error: %s\n", uv_strerror(req->result));
//    }
//    else if (req->result == 0)
//    {
//        uv_fs_t close_req;
//        // synchronous
//        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
//        uv_close((uv_handle_t*)&mysocket, NULL);
//    }
//    else
//    {
//        iov.len = req->result;
//        uv_write((uv_write_t*)req, (uv_stream_t*)&mysocket, &iov, 1, on_write);
//    }
//}
//
//void on_open(uv_fs_t* req)
//{
//    if (req->result >= 0)
//    {
//        iov = uv_buf_init(buffer, sizeof(buffer));
//        uv_fs_read(uv_default_loop(), &read_req, req->result, &iov, 1, -1, on_read);
//    }
//    else
//    {
//        fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
//        uv_close((uv_handle_t*)&mysocket, NULL);
//        exit(-1);
//    }
//}
//
//const char* data = "hello world";
//const uv_buf_t buff = {20, (char*)data };
//
//void on_connect(uv_connect_t* req, int status)
//{
//    if (status < 0)
//    {
//        fprintf(stderr, "Connection error %s\n", uv_strerror(status));
//
//        return;
//    }
//
//    fprintf(stdout, "Connect ok\n");
//
//
//    uv_write_t writereq;
//
//
//    uv_write(&writereq, req->handle, &buff, 1, on_write);
//}
//
//int main2()
//{
//    loop = uv_default_loop();
//
//    
//    uv_tcp_init(loop, &mysocket);
//
//    struct sockaddr_in addr;
//
//    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
//
//
//    uv_ip4_addr("127.0.0.1", DEFAULT_PORT, &addr);
//
//    int r = uv_tcp_connect(connect, &mysocket, (const struct sockaddr*)&addr, on_connect);
//
//    if (r)
//    {
//        fprintf(stderr, "connect error %s\n", uv_strerror(r));
//        return 1;
//    }
//
//    return uv_run(loop, UV_RUN_DEFAULT);
//}