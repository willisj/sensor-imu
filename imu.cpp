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
		if(imu.get_imu_data(&newdata))
			printf("%lf: acc: %lf,%lf,%lf\tgyro: %lf,%lf,%lf\tmagnet: %lf,%lf,%lf\n",
					newdata.timestamp, newdata.accel.x, newdata.accel.y, newdata.accel.z,
					newdata.gyro.x, newdata.gyro.y, newdata.gyro.z,
					newdata.mag.x, newdata.mag.y, newdata.mag.z );
		usleep(10000);
    	}

	std::cout << "Exiting." << std::endl;
    return 0;
}
