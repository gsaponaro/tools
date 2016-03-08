/*
 * Copyright: (C) 2016 VisLab, Institute for Systems and Robotics,
 *            Instituto Superior Técnico, Universidade de Lisboa, Lisbon, Portugal
 * Author: Giovanni Saponaro <gsaponaro@isr.ist.utl.pt>
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#include "JointTrajectory2YarpModule.h"

bool JointTrajectory2YarpModule::configure(ResourceFinder &rf)
{
    moduleName = rf.check("name",Value(DEFAULT_NAME)).asString();
    yarp::os::RFModule::setName(moduleName.c_str());

    if (rf.check("in"))
    {
        inputPortName = rf.find("in").asString();
    }
    else
    {
        yWarning("no input topic specified, using default: %s", DEFAULT_INPUT_TOPIC);
        inputPortName = DEFAULT_INPUT_TOPIC;
    }

    if (rf.check("out"))
    {
        outputPortName = rf.find("out").asString();
    }
    else
    {
        yWarning("no output port specified, using default: %s", DEFAULT_OUTPUT_PORT);
        outputPortName = DEFAULT_OUTPUT_PORT;
    }

    rosNode = new yarp::os::Node("/"+std::string(DEFAULT_NAME)+"_node"); // can be any string

    inputPort.setReadOnly();
    bool validInput = inputPort.topic(inputPortName.c_str());
    if (!validInput)
    {
        yError("could not set ROS topic in input port");
        return false;
    }

    bool validOutput = outputPort.open(outputPortName.c_str());
    if (!validOutput)
    {
        yError("could not open output YARP port");
        return false;
    }

    // start clock
    t = yarp::os::Time::now();

    return true;
}

bool JointTrajectory2YarpModule::interruptModule()
{
    outputPort.interrupt();
    return true;
}

bool JointTrajectory2YarpModule::close()
{
    outputPort.close();
    return true;
}

double JointTrajectory2YarpModule::getPeriod()
{
    return 0.0; // sync on incoming data
}

bool JointTrajectory2YarpModule::updateModule()
{
    trajectory_msgs_JointTrajectory *mux;
    mux = inputPort.read(false); // TODO: check
    yarp::os::Time::delay(0.05); // TODO: check
    if (mux != NULL)
    {
        // TODO
        //trajectory_msgs_JointTrajectory &outYarp = outputPort.prepare();
        /*
        // http://stackoverflow.com/a/4254644/1638888
        char *dataYarp = reinterpret_cast<char*>(&(mux->data)[0]);
        outYarp.setExternal(dataYarp,
                            mux->width,
                            mux->height);
        */
        outputPort.write();

        // print a message periodically
        double t0 = yarp::os::Time::now();
        if ((t0 - t) > 10.0)
        {
            yInfo("bridge from ROS topic %s to YARP port %s running successfully",
                 inputPortName.c_str(),
                 outputPortName.c_str());
            t = t0;
        }
    }

    return true;
}
