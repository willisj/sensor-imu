
CXX = g++ -std=gnu++11
required_libs = -lpthread -I . -pthread

module: 
	$(CXX) $(required_libs) $(debug_flags) -c imu_sensor.cpp imu_sensor.hpp

example: module
	$(CXX) $(required_libs) $(debug_flags) -o imu_sensor_test imu.cpp imu_sensor.o

debug: debug_flags = -ggdb -O0
debug: module

all: module example 

clean:
	rm imu_sensor.o&
	rm imu_sensor_test&
	rm imu_sensor.hpp.gch
