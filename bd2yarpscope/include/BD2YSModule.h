/*
 * Copyright: (C) 2012-2015 POETICON++, European Commission FP7 project ICT-288382
 * Copyright: (C) 2014 VisLab, Institute for Systems and Robotics,
 *                Istituto Superior Técnico, Universidade de Lisboa, Lisbon, Portugal
 * Author: Giovanni Saponaro <gsaponaro@isr.ist.utl.pt>
 * CopyPolicy: Released under the terms of the GNU GPL v2.0
 *
 */

#ifndef	__BD2YS_MODULE_H__
#define __BD2YS_MODULE_H__

#include <string>

#include <yarp/os/BufferedPort.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Stamp.h>
#include <yarp/os/Time.h>

using namespace std;
using namespace yarp::os;

class BD2YSModule : public RFModule
{
    private:
        string moduleName;
        string handlerPortName;

        string inAffPortName;
        string outAffPortName;
        string inToolAffPortName;
        string outToolAffPortName;

        BufferedPort<Bottle> inAffPort;
        BufferedPort<Bottle> outAffPort;
        BufferedPort<Bottle> inToolAffPort;
        BufferedPort<Bottle> outToolAffPort;

        Bottle *inAff;
        Bottle *inToolAff;

        bool gotSomething;
        string mode;
        int blobIndex;

    public:
        BD2YSModule();
        virtual ~BD2YSModule();

        virtual bool configure(ResourceFinder &rf);
        virtual bool interruptModule();
        virtual bool close();
        virtual bool updateModule();
        virtual double getPeriod();

        void writeAff();
};

#endif
