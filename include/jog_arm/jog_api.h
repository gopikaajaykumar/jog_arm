///////////////////////////////////////////////////////////////////////////////
//      Title     : jog_api.h
//      Project   : jog_arm
//      Created   : 3/27/2018
//      Author    : Andy Zelenak
//      Platforms : Ubuntu 64-bit
//      Copyright : Copyright© The University of Texas at Austin, 2014-2017. All rights reserved.
//
//          All files within this directory are subject to the following, unless an alternative
//          license is explicitly included within the text of each file.
//
//          This software and documentation constitute an unpublished work
//          and contain valuable trade secrets and proprietary information
//          belonging to the University. None of the foregoing material may be
//          copied or duplicated or disclosed without the express, written
//          permission of the University. THE UNIVERSITY EXPRESSLY DISCLAIMS ANY
//          AND ALL WARRANTIES CONCERNING THIS SOFTWARE AND DOCUMENTATION,
//          INCLUDING ANY WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//          PARTICULAR PURPOSE, AND WARRANTIES OF PERFORMANCE, AND ANY WARRANTY
//          THAT MIGHT OTHERWISE ARISE FROM COURSE OF DEALING OR USAGE OF TRADE.
//          NO WARRANTY IS EITHER EXPRESS OR IMPLIED WITH RESPECT TO THE USE OF
//          THE SOFTWARE OR DOCUMENTATION. Under no circumstances shall the
//          University be liable for incidental, special, indirect, direct or
//          consequential damages or loss of profits, interruption of business,
//          or related expenses which may arise from use of software or documentation,
//          including but not limited to those resulting from defects in software
//          and/or documentation, or loss or inaccuracy of data of any kind.
//
///////////////////////////////////////////////////////////////////////////////

// Provide a C++ interface for sending motion commands to the jog_arm server.

#ifndef MOTION_API_H
#define MOTION_API_H

#include <geometry_msgs/PoseStamped.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <tf/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/transform_listener.h>

class jog_api {
public:
	// Constructor
  jog_api(const std::string& move_group_name) :
    move_group_(move_group_name),
    tf2_listener_(tf_buffer_)
  {
  	jog_vel_pub_ = nh_.advertise<geometry_msgs::TwistStamped>("/jog_arm_server/delta_jog_cmds", 1);
  }

  // Publish cmds for a Cartesian motion to bring the robot to the target pose.
  bool jacobian_move(geometry_msgs::PoseStamped& target_pose,
    const double trans_tolerance,
    const double rot_tolerance,
    const double linear_vel_scale,
    const double rot_vel_scale,
    const ros::Duration& timeout);

private:
	ros::NodeHandle nh_;

	// Used to retrieve the current robot pose, etc.
  moveit::planning_interface::MoveGroupInterface move_group_;

	ros::Publisher jog_vel_pub_;

  tf2_ros::Buffer tf_buffer_;
  tf2_ros::TransformListener tf2_listener_;

  bool transform_a_pose(geometry_msgs::PoseStamped &pose, std::string &desired_frame);

  // Calculate Euclidean distance between 2 Poses
  struct distance_and_twist {
    double translational_distance, rotational_distance;
    geometry_msgs::TwistStamped twist;
  };
  distance_and_twist calc_distance_and_twist(const geometry_msgs::PoseStamped &current_pose, const geometry_msgs::PoseStamped &target_pose, const double &linear_vel_scale, const double &rot_vel_scale);
};

#endif
