#include "flight_controller/flight_control_unit.h"

#include <mavros_msgs/OverrideRCIn.h>

namespace flight_controller {

FlightControlUnit::FlightControlUnit() : goal_(nullptr), execution_(false) {
  ros::NodeHandle nh;
  pose_sub_ = nh.subscribe("/erlecopter/ground_truth/position", 1, &FlightControlUnit::poseCallback, this);
  cmd_pub_ = nh.advertise<mavros_msgs::OverrideRCIn>("/cmd", 1);
}

bool FlightControlUnit::ok() const {
  const double x = point_.point.x;
  const double y = point_.point.y;
  const double z = point_.point.z;
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
  const double timeout_sec = 10.;
  const double poll_rate = 20.;
  for (int i = 0; i < poll_rate * timeout_sec; ++i) {
    //ROS_INFO_STREAM(__PRETTY_FUNCTION__ << ": spin!");
    if (isGoalReached()) {
      execution_ = false;
      ROS_INFO("goal reached");
      return true;
    }
    ros::Rate(poll_rate).sleep();
  }

  if (!isGoalReached()){
     ROS_INFO("goal not reached: shutting down...");
     shutdown();
     return true;
  }
  execution_ = false;
  return false;
}

bool FlightControlUnit::isGoalReached() const {
    const double dx = goal_->position.x - point_.point.x;
    const double dy = goal_->position.y - point_.point.y;
    ROS_INFO("error_dx_dy = (%f, %f)", dx, dy);
    return std::hypot(dx, dy) < 0.1;
}

void FlightControlUnit::poseCallback(
  const geometry_msgs::PointStamped::ConstPtr& point) {
  point_ = *point;
  //ROS_INFO("I heard: [%f],[%f],[%f]", point_.point.x, point_.point.y, point_.point.z);
  if (execution_) {
    mavros_msgs::OverrideRCIn cmd;
    controller_.computeCommand(*goal_, point, &cmd);
    cmd_pub_.publish(cmd);
  } else {
    goal_.reset(nullptr);
  }
}

}  // namespace flight_controller
