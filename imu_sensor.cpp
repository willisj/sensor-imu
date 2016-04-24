#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string> 
#include <string.h>
#include <vector>
#include <sstream>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include "imu_sensor.hpp"

#define BUFLEN 256
imu_res::imu_res(int port){
	// bind etc. 
	int i; 

	if ((this->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
		perror("sock");
		exit(1);
	}
	else 
		printf("Server : Socket() successful\n");

	bzero(&(this->my_addr), sizeof(this->my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(this->sockfd, (struct sockaddr* ) &(this->my_addr), sizeof(this->my_addr))==-1){
		perror("bind");
		exit(1);
	}
	else
		printf("Server : bind() successful\n");
}

std::vector<std::string> split(const std::string &text, char sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

void imu_res::parse_line(char * buf, size_t pkt_size){
	struct imu_data_t imu_data;
	double  time,  x, y, z;
	int id;
	const char * delim = ",";
	std::string line(buf);
	std::vector<std::string> elems = split(line,','); 
	imu_data.timestamp = stof( elems.at(0) );
	for(int i = 1; i < elems.size();){


		id = stoi( elems.at(i++) );
		
		x = stod( elems.at(i++) );
		y = stod( elems.at(i++) );
		z = stod( elems.at(i++) );

		switch( id ){
			case 3:
				imu_data.accel.x = x;
				imu_data.accel.y = y;
				imu_data.accel.z = z;
				break;
			case 4:
				imu_data.gyro.x = x;
				imu_data.gyro.y = y;
				imu_data.gyro.z = z;
				break;
			case 5:
				imu_data.mag.x = x;
				imu_data.mag.y = y;
				imu_data.mag.z = z;
				break;
		}
	}
	this->newdata = imu_data;
}

bool imu_res::poll_imu(){
	// get a single packet
	char buf[256];
	size_t pkt_size = 0;
	socklen_t slen=sizeof(this->cli_addr);

	pkt_size = recvfrom( this->sockfd, buf, BUFLEN-1, 0, (struct sockaddr*)&(this->cli_addr), &slen);
	if (pkt_size==-1){
		perror("error receiving packet");
		return false;
	}
	buf[pkt_size] = '\0';

	this->newdata_mtex.lock();
	this->parse_line(buf,pkt_size);
	this->newdata_mtex.unlock();
	
	return true;
}

// get the current values
void imu_res::get_imu_data(struct imu_data_t *dest){
	this->newdata_mtex.lock();
	memcpy(dest, &(this->newdata), sizeof(imu_data_t));	
	this->newdata_mtex.unlock();
}

// thread function
static void * imu_loop(void *data){
	imu_res *imu = (imu_res *)data;
	while(imu->poll_imu())	{}
}

void imu_res::thread_imu(){
	this->imu_thread = new pthread_t;
	pthread_create(this->imu_thread, NULL, imu_loop, (void *)this);

}



