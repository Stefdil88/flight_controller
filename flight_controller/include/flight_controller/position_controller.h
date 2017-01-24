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
<<<<<<< HEAD
    PositionController();
=======
    PositionController() {}
>>>>>>> 8aa5560095dfc3c8308c1a2e0c4aee27e4906efb
    virtual ~PositionController() {}

    void computeCommand(const geometry_msgs::Pose& reference,
                        const geometry_msgs::PointStamped::ConstPtr& point,
                        mavros_msgs::OverrideRCIn* cmd);

  private:
<<<<<<< HEAD

    ros::Time lastTime; //Time control
    double pre_err_x, pre_err_y;
    double kP=45, kD=40;
=======
    double kP, kD;
>>>>>>> 8aa5560095dfc3c8308c1a2e0c4aee27e4906efb
    double Roll, Pitch;
};

}
//#endif
