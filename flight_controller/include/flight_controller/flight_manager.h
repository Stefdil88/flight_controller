//#ifndef FLIGHTMANAGER_H_
//#define FLIGHTMANAGER_H_

#include "flight_controller/FlyToGoal.h"

#include <ros/ros.h>

#include "flight_controller/flight_control_unit.h"

namespace flight_controller {

class FlightManager {
  public:
    enum flight_mode_t { IDLE, GUIDED, STABILIZE, LOITER };

  public:
    FlightManager();
    virtual ~FlightManager() {}

    //bool initialize();
    void initialize();
    void spin(double hz = 20.);
    void emergencyLand(double current_height);

  private:

    //void emergencyLand(double current_height);

    bool flyServiceCallback(
      FlyToGoal::Request& req, FlyToGoal::Response& res);

  private:
    // Fly service
    ros::ServiceServer fly_srv_;
    //ros::ServiceClient fly_cl_;
    //flight_controller::FlyToGoal ftg;

    // The flight control unit.
    // Flight modality
    flight_mode_t flight_mode_;

    FlightControlUnit fcu_;

    // Flight parameters.
    double height_;
};

}  // namespace flight_controller
//#endif
