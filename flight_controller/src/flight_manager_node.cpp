#include "flight_controller/flight_manager.h"
#include <ros/ros.h>
#include<cstdlib>

int main(int argc, char **argv) {
  ros::init(argc, argv, "flight_manager");

  flight_controller::FlightManager flight_manager;
<<<<<<< HEAD
  for (ros::Rate lr(1.); !flight_manager.initialize(); lr.sleep())
    ROS_WARN_STREAM("Trying to initialize the flight manager.");
=======
  //for (ros::Rate lr(1.); !flight_manager.initialize(); lr.sleep())
    //ROS_WARN_STREAM("Trying to initialize the flight manager.");
>>>>>>> 8aa5560095dfc3c8308c1a2e0c4aee27e4906efb

  /*if (atoll(argv[1])==1){
    ROS_INFO("Initialization");
    flight_manager.initialize();
  }*/

  flight_manager.spin();
  //flight_manager.emergencyLand(3);
  return EXIT_SUCCESS;
}
