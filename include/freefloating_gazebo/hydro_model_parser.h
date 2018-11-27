#ifndef HYDRO_MODEL_PARSER_H
#define HYDRO_MODEL_PARSER_H


#include <tinyxml2.h>
#include <ros/ros.h>
#include <Eigen/Core>
#include <freefloating_gazebo/hydro_link.h>
#include <urdf/model.h>

namespace ffg
{

class HydroModelParser
{
  struct Thruster
  {
    std::string name;
    bool fixed;
    double max_thrust;
    std::vector<double> map;
  };

public:
  HydroModelParser() {}

  void parseAll(ros::NodeHandle &nh, bool display = true);
  void parseThusters(std::string sdf_str, std::string robot_name = "");
  void parseLinks(ros::NodeHandle &nh, bool display = true)
  {
    std::string robot_description;
    nh.getParam("robot_description", robot_description);
    parseLinks(robot_description, display);
  }
  void parseLinks(const std::string &robot_description, bool display = true)
  {
    tinyxml2::XMLDocument doc;
    doc.Parse(robot_description.c_str());
    parseLinks(doc.RootElement(), display);
  }
  void parseLinks(tinyxml2::XMLElement *root, bool display = true);

  std::map<std::string, HydroLink> getLinks() const {return links;}
  std::vector<Thruster> getThrusters() const {return thrusters;}
  Eigen::MatrixXd thrusterMap() const;

  // utility functions for base_link
  uint thrusterInfo(std::vector<uint> &thruster_fixed,
                    std::vector<uint> &thruster_steering,
                    std::vector<std::string> &thruster_names,
                    std::vector<double> &max_thrust) const;
  std::vector<double> maxVelocity() const;
  std::vector<double> maxWrench() const;

protected:
  std::map<std::string, HydroLink> links;
  std::vector<Thruster> thrusters;
  std::string base_link = "base_link";

  bool isNamed(tinyxml2::XMLElement* elem, std::string name) const
  {
    return strcmp(elem->Attribute("name"), name.c_str()) == 0;
  }

  void addBuoyancy(const tinyxml2::XMLElement* elem, const std::string &name, const urdf::InertialSharedPtr &inertial);
  void addHydrodynamics(const tinyxml2::XMLElement* elem, const std::string &name);

};

}

#endif // HYDRO_MODEL_PARSER_H
