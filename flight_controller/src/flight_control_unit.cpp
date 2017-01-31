#include "flight_controller/flight_control_unit.h"

#include <mavros_msgs/OverrideRCIn.h>

namespace flight_controller {

FlightControlUnit::FlightControlUnit() : goal_(nullptr), execution_(false) {
  ros::NodeHandle nh;
  pose_sub_ = nh.subscribe("/erlecopter/ground_truth/position", 1, &FlightControlUnit::poseCallback, this);
  cmd_pub_ = nh.advertise<mavros_msgs::OverrideRCIn>("/mavros/rc/override", 1000);
}

bool FlightControlUnit::ok() const {
  const double x = point_.point.x;
  const double y = point_.point.y;
  const double z = point_.point.z;
  //ROS_INFO("Position (%f, %f, %f)", x, y, z);
  const bool good_x = x >= min_x && x <= max_x;
  const bool good_y = y >= min_y && y <= max_y;
  const bool good_z = z >= min_z && z <= max_z;
  return good_x && good_y && good_z;
}

bool FlightControlUnit::inExecution() const {
  return execution_;
}

double FlightControlUnit::shutdown() {
  cmd_pub_.shutdown();
  return point_.point.z;
}

void FlightControlUnit::setGoal(const geometry_msgs::Pose& goal) {
  goal_.reset(new geometry_msgs::Pose(goal));
}

bool FlightControlUnit::execute() {
  if (!goal_)
    return false;
  // Start flying...
  execution_ = true;
  const double timeout_sec = 30.;
  const double poll_rate = 20.;
  for (int i = 0; i < poll_rate * timeout_sec; ++i) {

    if (ros::ok())
      ros::spinOnce();
    //ROS_INFO("ciao [%i]; ",execution_);
    //std::cout<<execution_;
    //ROS_INFO("extrema: %d", ok());
      if (ok()){
        if (isGoalReached()) {
          execution_ = false;
          ROS_INFO("goal reached");
          return true;
        }
        ros::Rate(poll_rate).sleep();
      }
      else {//ROS_INFO("VAFFANCULO");
            shutdown();} //ATTENZIONE: atterraggio di emergenza da implementare
    }
  if (!isGoalReached()){
     ROS_INFO("goal not reached: shutting down...");
     //shutdown();
     execution_=false;
     return true;
  }
  //execution_ = false;
  //return false;
}

bool FlightControlUnit::isGoalReached() const {
    const double dx = goal_->position.x - point_.point.x;
    const double dy = goal_->position.y - point_.point.y;
    //ROS_INFO("error_dx_dy = (%f, %f)", dx, dy);
    return std::hypot(dx, dy) < 0.1;
}

void FlightControlUnit::poseCallback(
  const geometry_msgs::PointStamped::ConstPtr& point) {
  point_ = *point;
  //ROS_INFO("I heard: [%f],[%f],[%f]", point_.point.x, point_.point.y, point_.point.z);
  //ROS_INFO_STREAM(__PRETTY_FUNCTION__ << ": spin!");
  if (execution_) {
    //ROS_INFO("I heard: [%f],[%f],[%f]", point_.point.x, point_.point.y, point_.point.z);
    mavros_msgs::OverrideRCIn cmd;
    //lastTime = ros::Time::now();
    if (!controller_.isActive())
      controller_.activate(ros::Time::now());
    controller_.computeCommand(*goal_, point, &cmd);
    //ROS_INFO("I heard: [%d],[%d]", cmd.channels[0], cmd.channels[1]);
    cmd_pub_.publish(cmd);
  } else {
    goal_.reset(nullptr);
    controller_.deactivate();
  }
}

}  // namespace flight_controller
