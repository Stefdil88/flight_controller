#include "flight_controller/position_controller.h"

#define MINRC 1100
#define BASERC 1500
#define MAXRC 1900

<<<<<<< HEAD
//#define FACTOR  30

namespace flight_controller {

PositionController::PositionController(): pre_err_x(0), pre_err_y(0)
{

}
=======
#define FACTOR  60

namespace flight_controller {

>>>>>>> 8aa5560095dfc3c8308c1a2e0c4aee27e4906efb
void PositionController::computeCommand(const geometry_msgs::Pose& reference,
                        const geometry_msgs::PointStamped::ConstPtr& point,
                        mavros_msgs::OverrideRCIn* cmd) {
  const double err_x = reference.position.x - point->point.x;
  const double err_y = reference.position.y - point->point.y;
  const double err_z = reference.position.z - point->point.z;
<<<<<<< HEAD
  //ROS_INFO("errore x e y (%f, %f):", err_x, err_y);

  //Time since last call
  double dt = (ros::Time::now() - lastTime).toSec();
  lastTime = ros::Time::now();

  //Calculating derivative
  double derivative_x = (err_x - pre_err_x)*dt;
  double derivative_y = (err_y - pre_err_y)*dt;

  //Total output
  Roll = BASERC + err_x * kP + kD*derivative_x;
  Pitch = BASERC + err_y * kP + kD*derivative_y;

  // Limiting Pitch and Roll
  if (Roll > MAXRC)
  {
      Roll = MAXRC;
  } else if (Roll < MINRC)
  {
      Roll = MINRC;
  }

=======

  //ROS_INFO("ciao [%f], [%f]:", err_x, err_y);
  Roll = BASERC - err_x * FACTOR;
  Pitch = BASERC - err_y * FACTOR;

  // Limit the Pitch
>>>>>>> 8aa5560095dfc3c8308c1a2e0c4aee27e4906efb
  if (Pitch > MAXRC)
  {
      Pitch = MAXRC;
  } else if (Pitch < MINRC)
  {
      Pitch = MINRC;
  }
<<<<<<< HEAD

  pre_err_x = err_x;
  pre_err_y = err_y;

  //ROS_INFO("roll and pitch (%f, %f):", Roll, Pitch);
  ROS_INFO("errore x e y (%f, %f):", err_x, err_y);
=======
  //ROS_INFO("ciao [%f], [%f}]: ", Roll, Pitch);
>>>>>>> 8aa5560095dfc3c8308c1a2e0c4aee27e4906efb
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
