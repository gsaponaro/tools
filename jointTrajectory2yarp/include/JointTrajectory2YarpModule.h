/*
 * Copyright: (C) 2016 VisLab, Institute for Systems and Robotics,
 *            Instituto Superior Técnico, Universidade de Lisboa, Lisbon, Portugal
 * Author: Giovanni Saponaro <gsaponaro@isr.ist.utl.pt>
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#ifndef JOINTTRAJECTORY2YARP_MODULE_H
#define JOINTTRAJECTORY2YARP_MODULE_H

#define DEFAULT_NAME "jointTrajectory2yarp"
#define DEFAULT_INPUT_TOPIC "/vizzy/right_arm_trajectory_controller/command"
#define DEFAULT_OUTPUT_PORT "/yarpTrajectories/right_arm_shoulder/points:o"

#include <string>

#include <yarp/os/BufferedPort.h>
#include <yarp/os/Log.h>
#include <yarp/os/Network.h>
#include <yarp/os/Node.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/os/Subscriber.h>
#include <yarp/os/Time.h>
#include <yarp/sig/Image.h>

#include "trajectory_msgs_JointTrajectory.h"

using namespace yarp::os;
using namespace yarp::sig;

class JointTrajectory2YarpModule : public RFModule
{
private:
    std::string moduleName;
    std::string inputPortName;
    std::string outputPortName;
    Node *rosNode;
    Subscriber<trajectory_msgs_JointTrajectory> inputPort;
    BufferedPort<Bottle> outputPort;
    double t;

public:
    bool configure(ResourceFinder &rf);
    bool interruptModule();
    bool close();
    double getPeriod();
    bool updateModule();
};

#endif // JOINTTRAJECTORY2YARP_MODULE_H
