/*
 * Copyright: (C) 2016 VisLab, Institute for Systems and Robotics,
 *                Istituto Superior Técnico, Universidade de Lisboa, Lisbon, Portugal
 * Author: Giovanni Saponaro <gsaponaro@isr.ist.utl.pt>
 * CopyPolicy: Released under the terms of the GNU GPL v2.0
 *
 */

#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>

#include "CheckCameraSynchModule.h"

using namespace yarp::os;

int main(int argc, char *argv[])
{
    ResourceFinder rf;
    rf.configure(argc, argv);

    Network yarp;
    if(! yarp.checkNetwork() )
    {
        fprintf(stdout,"YARP server not available!\n");
        return -1; // EXIT_FAILURE
    }

    CheckCameraSynchModule mod;
    return mod.runModule(rf);
}

