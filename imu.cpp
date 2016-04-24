#include <iostream>
#include <cstdlib>
#include <string.h>

#include <unistd.h>

#include "imu_sensor.hpp"

int main(void)
{
	
	imu_res imu(5555); 
	struct imu_data_t newdata;
	
	std::cout << "Starting IMU thread" << std::endl;
	imu.thread_imu();


    	for (;;) {
		imu.get_imu_data(&newdata);
		printf("%lf: %lf,%lf,%lf\n",newdata.timestamp, newdata.gyro.x, newdata.gyro.y, newdata.gyro.z );
		usleep(10000);
    	}

	std::cout << "Exiting." << std::endl;
    return 0;
}
