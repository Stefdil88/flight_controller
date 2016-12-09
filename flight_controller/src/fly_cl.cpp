#include "flight_controller/FlyToGoal.h"


#include "ros/ros.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "fly_to_goal");
  ros::NodeHandle nh;

ros::ServiceClient fly_cl_=nh.serviceClient<flight_controller::FlyToGoal>("/fly_to_goal");

flight_controller::FlyToGoal ftg;

ftg.request.goal.position.x=1;
ftg.request.goal.position.y=0;
ftg.request.goal.position.z=3;
ftg.request.goal.orientation.x=0;
ftg.request.goal.orientation.y=0;
ftg.request.goal.orientation.z=0;
ftg.request.goal.orientation.w=1;

if (fly_cl_.call(ftg))
  {
    ROS_INFO("Call executed ...[%f]", ftg.request.goal.position.x);
  }
  else
  {
    ROS_ERROR("Failed to call service fly_to_goal");
    return 1;
  }

  return EXIT_SUCCESS;
}
