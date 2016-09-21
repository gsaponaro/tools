/*
 * Copyright: (C) 2016 VisLab, Institute for Systems and Robotics,
 *                Istituto Superior Técnico, Universidade de Lisboa, Lisbon, Portugal
 * Author: Giovanni Saponaro <gsaponaro@isr.ist.utl.pt>
 * CopyPolicy: Released under the terms of the GNU GPL v2.0
 *
 */

#include <yarp/os/Log.h>
#include <yarp/os/Stamp.h>
#include <yarp/os/Time.h>

#include "CheckCameraSynchModule.h"

using namespace yarp::os;
using namespace yarp::sig;

CheckCameraSynchModule::CheckCameraSynchModule()
{
}

CheckCameraSynchModule::~CheckCameraSynchModule()
{
}

bool CheckCameraSynchModule::configure(ResourceFinder &rf)
{
    moduleName = rf.check("name",Value("checkCameraSynch")).asString().c_str();
    setName(moduleName.c_str());

    leftImgPortName = "/" + moduleName + "/left:i";
    rightImgPortName = "/" + moduleName + "/right:i";

    leftImgPort.open(leftImgPortName.c_str());
    rightImgPort.open(rightImgPortName.c_str());

    return true;
}

bool CheckCameraSynchModule::interruptModule()
{
    leftImgPort.interrupt();
    rightImgPort.interrupt();

    return true;
}

bool CheckCameraSynchModule::close()
{
    leftImgPort.close();
    rightImgPort.close();

    return true;
}

bool CheckCameraSynchModule::updateModule()
{
    ImageOf<PixelRgb> *yarpLeftImg = leftImgPort.read(true);
    ImageOf<PixelRgb> *yarpRightImg = rightImgPort.read(true);

    if (yarpLeftImg==NULL || yarpRightImg==NULL)
        return true;

    Stamp stampLeft;
    Stamp stampRight;
    leftImgPort.getEnvelope(stampLeft);
    rightImgPort.getEnvelope(stampRight);

    if (!stampLeft.isValid() ||
        !stampRight.isValid())
    {
        yWarning("timestamp(s) not valid");
    }

    yInfo("left %f, right %f => deviation between cameras %f",
          stampLeft.getTime(),
          stampRight.getTime(),
          stampLeft.getTime()-stampRight.getTime());

    return true;
}

double CheckCameraSynchModule::getPeriod()
{
    return 0.0;
}
