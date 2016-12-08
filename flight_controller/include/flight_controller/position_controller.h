//#ifndef POSITIONCONTROLLER_H_
//#define POSITIONCONTROLLER_H_

#include <ros/ros.h>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PointStamped.h>
#include <mavros_msgs/OverrideRCIn.h>

namespace flight_controller {

class PositionController {
  public:
    PositionController() {}
    virtual ~PositionController() {}

    void computeCommand(const geometry_msgs::Pose& reference,
                        const geometry_msgs::PointStamped::ConstPtr& point,
                        mavros_msgs::OverrideRCIn* cmd);

  private:
    double kP, kD;
};

}
//#endif
