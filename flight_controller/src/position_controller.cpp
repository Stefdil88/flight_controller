#include "flight_controller/position_controller.h"

#define MINRC 1100
#define BASERC 1500
#define MAXRC 1900

#define FACTOR  60

namespace flight_controller {

void PositionController::computeCommand(const geometry_msgs::Pose& reference,
                        const geometry_msgs::PointStamped::ConstPtr& point,
                        mavros_msgs::OverrideRCIn* cmd) {
  const double err_x = reference.position.x - point->point.x;
  const double err_y = reference.position.y - point->point.y;
  const double err_z = reference.position.z - point->point.z;

  //ROS_INFO("ciao [%f], [%f]:", err_x, err_y);
  Roll = BASERC - err_x * FACTOR;
  Pitch = BASERC - err_y * FACTOR;

  // Limit the Pitch
  if (Pitch > MAXRC)
  {
      Pitch = MAXRC;
  } else if (Pitch < MINRC)
  {
      Pitch = MINRC;
  }
  //ROS_INFO("ciao [%f], [%f}]: ", Roll, Pitch);
  cmd->channels[0] = Roll;
  cmd->channels[1] = Pitch;
  cmd->channels[2] = BASERC;
  cmd->channels[3] = 0;
  cmd->channels[4] = 0;
  cmd->channels[5] = 0;
  cmd->channels[6] = 0;
  cmd->channels[7] = 0;
}

}
