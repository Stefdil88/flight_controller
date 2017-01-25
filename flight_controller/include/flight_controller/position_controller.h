//#ifndef POSITIONCONTROLLER_H_
//#define POSITIONCONTROLLER_H_

#include <ros/ros.h>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PointStamped.h>
#include <mavros_msgs/OverrideRCIn.h>

#include <memory>

namespace flight_controller {

class PositionController {
  public:
    PositionController();
    virtual ~PositionController() {}

    void activate(const ros::Time& stamp) { lastTime.reset(new ros::Time(stamp));}
    void deactivate() { lastTime.reset(nullptr); }
    bool isActive() const { return lastTime != nullptr; }

    void computeCommand(const geometry_msgs::Pose& reference,
                        const geometry_msgs::PointStamped::ConstPtr& point,
                        mavros_msgs::OverrideRCIn* cmd);

  private:
    std::unique_ptr<ros::Time> lastTime;
    double pre_err_x, pre_err_y, integral_x, integral_y;
    double kP_x=50, kD_x=60, kI_x=10, kP_y=50, kD_y=60, kI_y=10;
    double Roll, Pitch;
};

}
//#endif
