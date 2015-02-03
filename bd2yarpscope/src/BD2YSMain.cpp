/*
 * Copyright: (C) 2012-2015 POETICON++, European Commission FP7 project ICT-288382
 * Copyright: (C) 2014 VisLab, Institute for Systems and Robotics,
 *                Istituto Superior Técnico, Universidade de Lisboa, Lisbon, Portugal
 * Author: Giovanni Saponaro <gsaponaro@isr.ist.utl.pt>
 * CopyPolicy: Released under the terms of the GNU GPL v2.0
 *
 */

#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>

#include "BD2YSModule.h"

int main(int argc, char *argv[])
{
    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("bd2yarpscope");    // overridden by --context parameter
    rf.setDefaultConfigFile("bd2yarpscope.ini");  // overridden by --from parameter
    rf.configure(argc, argv);

    if(rf.check("help"))
    {
        fprintf(stdout,"\n");
        fprintf(stdout,"Available options:\n");
        fprintf(stdout,"--name prefix (default bd2yarpscope)\n");
        fprintf(stdout,"--index # (default 0)\n");

        return 0; // EXIT_SUCCESS
    }

    Network yarp;
    if(! yarp.checkNetwork() )
    {
        fprintf(stdout,"YARP server not available!\n");
        return -1; // EXIT_FAILURE
    }

    BD2YSModule mod;
    return mod.runModule(rf);
}

