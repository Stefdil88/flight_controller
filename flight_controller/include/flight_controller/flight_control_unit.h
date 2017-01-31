//#ifndef FLIGHTCONTROLUNIT_H_
//#define FLIGHTCONTROLUNIT_H_

#include "flight_controller/position_controller.h"
#include <ros/ros.h>
#include <memory>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PointStamped.h>

namespace flight_controller {

class FlightControlUnit {
  public:
    FlightControlUnit();
    virtual ~FlightControlUnit() {}

    bool ok() const;
    bool inExecution() const;
    double shutdown();
    void setGoal(const geometry_msgs::Pose& goal);
    bool execute();
    
  public:
    double min_x, min_y, min_z;
    double max_x, max_y, max_z;
    //ros::Time lastTime;

  private:
    void poseCallback(const geometry_msgs::PointStamped::ConstPtr& point);
    bool isGoalReached() const;

  private:
    ros::Subscriber pose_sub_;
    ros::Publisher cmd_pub_;

    bool execution_;
    std::unique_ptr<geometry_msgs::Pose> goal_;
    geometry_msgs::PointStamped point_;

    PositionController controller_;
};

}
//#endif
