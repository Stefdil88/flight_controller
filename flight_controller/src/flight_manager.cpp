#include "flight_controller/flight_manager.h"

#include <ros/ros.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/CommandTOL.h>

namespace flight_controller {

FlightManager::FlightManager() : flight_mode_(IDLE) {
  // Fly service
  ros::NodeHandle nh;
  fly_srv_ = nh.advertiseService(
    "/fly_to_goal", &FlightManager::flyServiceCallback, this);

  // Parameters.
  ros::NodeHandle pnh("~");
  pnh.param<double>("flight_height", height_, 3.);
  pnh.param<double>("safety_min_x", fcu_.min_x, -3.);
  pnh.param<double>("safety_min_y", fcu_.min_y, -3.);
  pnh.param<double>("safety_min_z", fcu_.min_z, 0.);
  pnh.param<double>("safety_max_x", fcu_.max_x, 3.);
  pnh.param<double>("safety_max_y", fcu_.max_y, 3.);
  pnh.param<double>("safety_max_z", fcu_.max_z, 5.);
}

///////////////////////////////////////////////
//Initialization as a normal function////
///////////////////////////////////////////////////

/*void FlightManager::initialize() {
  // Changing flight modality to GUIDED
  ROS_INFO_STREAM("Trying to call service [/mavros/set_mode]");
  mavros_msgs::SetMode srv_mode;
  srv_mode.request.base_mode = 0;
  srv_mode.request.custom_mode = "GUIDED";
  if(ros::service::call("/mavros/set_mode", srv_mode)) {
    flight_mode_ = GUIDED;
  } else {
    ROS_ERROR("Could not call service [/mavros/set_mode]. Shutting down.");
    flight_mode_ = IDLE;
  }
  // Preparing to flight.
  if (flight_mode_ == GUIDED) {
    // Arming.
    mavros_msgs::CommandBool srv_command;
    srv_command.request.value = true;
    if(ros::service::call("/mavros/cmd/arming", srv_command)){
      ROS_INFO_STREAM("Successfully armed the robot.");
    } else{
      ROS_ERROR("Arming failed.");
      flight_mode_ = IDLE;
    }
    // Take off.
    mavros_msgs::CommandTOL srv_takeoff;
    srv_takeoff.request.altitude = height_;
    srv_takeoff.request.latitude = 0.;
    srv_takeoff.request.longitude = 0.;
    srv_takeoff.request.min_pitch = 0.;
    srv_takeoff.request.yaw = 0.;
    if(ros::service::call("/mavros/cmd/takeoff", srv_takeoff)) {
      ROS_INFO("TakeOff successfully commanded.");
    } else {
      ROS_ERROR("TakeOff service Failed");
      flight_mode_ = IDLE;
    }
  }
}*/

///////////////////////////////////////////////////////
//Initialization as bool function////
//////////////////////////////////////////////////////
bool FlightManager::initialize() {
  // Changing flight modality to GUIDED
  //ROS_INFO_STREAM("Trying to call service [/mavros/set_mode]");
  mavros_msgs::SetMode srv_mode;
  srv_mode.request.base_mode = 0;
  srv_mode.request.custom_mode = "GUIDED";
  if(ros::service::call("/mavros/set_mode", srv_mode)) {
    ROS_INFO_STREAM("Mode changed in GUIDED");
    flight_mode_ = GUIDED;
  } else {
    ROS_ERROR("Could not call service [/mavros/set_mode]. Shutting down.");
    flight_mode_ = IDLE;
    return false;
  }
  // Preparing to flight.
  if (flight_mode_ == GUIDED) {
    // Arming.
    mavros_msgs::CommandBool srv_command;
    srv_command.request.value = true;
    if(ros::service::call("/mavros/cmd/arming", srv_command)){
      ROS_INFO_STREAM("Successfully armed the robot.");
    } else{
      ROS_ERROR("Arming failed.");
      flight_mode_ = IDLE;
      return false;
    }
    // Take off.
    mavros_msgs::CommandTOL srv_takeoff;
    srv_takeoff.request.altitude = height_;
    srv_takeoff.request.latitude = 0.;
    srv_takeoff.request.longitude = 0.;
    srv_takeoff.request.min_pitch = 0.;
    srv_takeoff.request.yaw = 0.;
    if(ros::service::call("/mavros/cmd/takeoff", srv_takeoff)) {
      ROS_INFO("TakeOff successfully commanded.");
    } else {
      ROS_ERROR("TakeOff service Failed");
      flight_mode_ = IDLE;
      return false;
    }
  }
  return true;
}

void FlightManager::spin(double hz) {
  for (ros::Rate lr(hz); ros::ok(); lr.sleep()) {
    try {
      ros::spinOnce();
      if (!fcu_.ok())
        emergencyLand(fcu_.shutdown());
    } catch (const std::runtime_error& err) {
      // Got a runtime error, force landing.
      ROS_ERROR_STREAM("Got an errror. Landing. " << err.what());
      emergencyLand(fcu_.shutdown());
      flight_mode_ = IDLE;
    }
    //ROS_INFO_STREAM(__PRETTY_FUNCTION__ << ": spin!");
  }
  //emergencyLand(fcu_.shutdown());
  //ROS_INFO_STREAM(__PRETTY_FUNCTION__ << ": spin!");
}

void FlightManager::emergencyLand(double current_height) {
  mavros_msgs::CommandTOL srv_land;
  srv_land.request.altitude = current_height;
  srv_land.request.latitude = 0;
  srv_land.request.longitude = 0;
  srv_land.request.min_pitch = 0;
  srv_land.request.yaw = 0;
  while (!ros::service::call("/mavros/cmd/land", srv_land))
    ROS_WARN_STREAM("Emergency landing! Retrying...");
}

bool FlightManager::flyServiceCallback(FlyToGoal::Request& req, FlyToGoal::Response& res) {
//  ROS_INFO("ciao ciao %f",req.goal.position.z);
  if (fcu_.ok() && !fcu_.inExecution()) {
    //ROS_INFO("ciao ciao %f",req.goal.position.z);
    fcu_.setGoal(req.goal);
    return fcu_.execute();
  }
  return false;
}

}  // namespace flight_controller
