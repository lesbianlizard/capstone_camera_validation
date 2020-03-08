






//  client2.cpp
//  img_proc_feb6
//
//  Created by ATW2 on 2/6/20.
//  Copyright © 2020 ATW2. All rights reserved.
//


/*M///////////////////////////////////////////////////////////////////////////////////////
 //
 //  基于OpenCV和Socket的图像传输（发送）
 //
 //    By 彭曾 , at CUST, 2016.08.07
 //
 //    website: www.pengz0807.com  email: pengz0807@163.com
 //
 //M*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <iostream>

using namespace std;

#define PORT1 8079
#define PORT2 8080

// setup socket for second transmission of original frames 
int socket_setup(uint16_t port){
	int sock = 0;
    sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		cout << "error in socket()" << endl;
        return -1; 
    }

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
		printf("inet_pton error: %s\n", strerror(errno));
        return -1; 
    }

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		cout << "error in connect: " << strerror(errno) << endl;
        return -1;
    }
	return sock;
}

int main()
{
    socket_setup(PORT1);
    printf("socket1 done\n");
    
    socket_setup(PORT2);
    printf("socket2 done\n");
}
