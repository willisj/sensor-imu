
#ifndef IMU_SENSOR_HPP
#define IMU_SENSOR_HPP

#include <iostream>
#include <cstdlib>
#include <string.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <mutex>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

struct imu_val_t{
	double x;
	double y;
	double z;
};

struct imu_data_t {
	float timestamp;
	struct imu_val_t accel; 
	struct imu_val_t gyro; 
	struct imu_val_t mag; 
};

class imu_res{
	private:
		struct sockaddr_in my_addr, cli_addr;
		int sockfd;// socket info
		pthread_t *imu_thread;
		std::mutex newdata_mtex;
		struct imu_data_t newdata;
	public:
		imu_res(int);
		void parse_line(char * , size_t );
		bool poll_imu();
		void thread_imu();
		void get_imu_data(struct imu_data_t *dest);
};


#endif
