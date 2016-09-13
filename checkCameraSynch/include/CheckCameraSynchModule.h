/*
 * Copyright: (C) 2016 VisLab, Institute for Systems and Robotics,
 *                Istituto Superior Técnico, Universidade de Lisboa, Lisbon, Portugal
 * Author: Giovanni Saponaro <gsaponaro@isr.ist.utl.pt>
 * CopyPolicy: Released under the terms of the GNU GPL v2.0
 *
 */

#ifndef CHECK_CAMERA_SYNCH_MODULE_H
#define CHECK_CAMERA_SYNCH_MODULE_H

#include <string>

#include <yarp/os/BufferedPort.h>
#include <yarp/os/RFModule.h>
#include <yarp/sig/Image.h>

class CheckCameraSynchModule : public yarp::os::RFModule
{
    private:
        std::string moduleName;

        std::string leftImgPortName;
        std::string rightImgPortName;

        yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > leftImgPort;
        yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > rightImgPort;

    public:
        CheckCameraSynchModule();
        virtual ~CheckCameraSynchModule();

        virtual bool configure(yarp::os::ResourceFinder &rf);
        virtual bool interruptModule();
        virtual bool close();
        virtual bool updateModule();
        virtual double getPeriod();
};

#endif
