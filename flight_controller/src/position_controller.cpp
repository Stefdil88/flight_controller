#include "flight_controller/position_controller.h"

namespace flight_controller {

void PositionController::computeCommand(const geometry_msgs::Pose& reference,
                        const geometry_msgs::PointStamped::ConstPtr& point,
                        mavros_msgs::OverrideRCIn* cmd) {
  const double err_x = reference.position.x - point->point.x;
  const double err_y = reference.position.y - point->point.y;
  const double err_z = reference.position.z - point->point.z;
  cmd->channels[0] = kP * err_x;
  cmd->channels[1] = kP * err_y;
  cmd->channels[2] = 0;
  cmd->channels[3] = 0;
  cmd->channels[4] = 0;
  cmd->channels[5] = 0;
  cmd->channels[6] = 0;
  cmd->channels[7] = 0;
}

}
