/*
 * Copyright: (C) 2016 VisLab, Institute for Systems and Robotics,
 *            Instituto Superior Técnico, Universidade de Lisboa, Lisbon, Portugal
 * Author: Giovanni Saponaro <gsaponaro@isr.ist.utl.pt>
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#include <string>

#include <yarp/os/Log.h>
#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>

#include "JointTrajectory2YarpModule.h"

int main(int argc, char* argv[])
{
    yarp::os::Network yarp;

    ResourceFinder rf;
    rf.setVerbose(false);
    rf.setDefaultContext(DEFAULT_NAME);
    rf.setDefaultConfigFile((std::string(DEFAULT_NAME)+".ini").c_str());
    rf.configure(argc, argv);

    if(rf.check("help"))
    {
        yInfo("Available options:");
        yInfo("--name prefix (default %s)", DEFAULT_NAME);
        yInfo("--in ros_topic_input (default %s)", DEFAULT_INPUT_TOPIC);
        yInfo("--out yarp_port_output (default %s)", DEFAULT_OUTPUT_PORT);
        return 0; // EXIT_SUCCESS
    }

    if (! yarp::os::Network::checkNetwork())
    {
        yError("yarpserver not available!");
        return 1; // EXIT_FAILURE
    }

    JointTrajectory2YarpModule mod;
    return mod.runModule(rf);
}
