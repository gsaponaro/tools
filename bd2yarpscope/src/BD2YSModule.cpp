/*
 * Copyright: (C) 2012-2015 POETICON++, European Commission FP7 project ICT-288382
 * Copyright: (C) 2014 VisLab, Institute for Systems and Robotics,
 *                Istituto Superior Técnico, Universidade de Lisboa, Lisbon, Portugal
 * Author: Giovanni Saponaro <gsaponaro@isr.ist.utl.pt>
 * CopyPolicy: Released under the terms of the GNU GPL v2.0
 *
 */

#include "BD2YSModule.h"

BD2YSModule::BD2YSModule()
{
}

BD2YSModule::~BD2YSModule()
{
}

bool BD2YSModule::configure(ResourceFinder &rf)
{
    moduleName = rf.check("name", Value("bd2yarpscope")).asString();
    setName(moduleName.c_str());

    inAffPortName = "/" + moduleName + "/affDescriptor:i";
    inAffPort.open(inAffPortName.c_str());

    outAffPortName = "/" + moduleName + "/affDescriptor:o";
    outAffPort.open(outAffPortName.c_str());

    inToolAffPortName = "/" + moduleName + "/toolAffDescriptor:i";
    inToolAffPort.open(inToolAffPortName.c_str());

    outToolAffPortName = "/" + moduleName + "/partDescriptor:o";
    outToolAffPort.open(outToolAffPortName.c_str());

    mode = rf.check("mode", Value("whole")).asString();
    if (mode != "whole" && mode != "top" && mode != "bottom")
    {
        mode = "whole";
        fprintf(stdout, "warning: invalid mode, using default %s\n", mode.c_str());
    }
    fprintf(stdout, "using mode %s\n", mode.c_str());

    blobIndex = rf.check("index", Value(0)).asInt(); // idx of blob to process
    fprintf(stdout, "started %s on blob %d\n", moduleName.c_str(), blobIndex);

    inAff = NULL;
    inToolAff = NULL;

    return true;
}

bool BD2YSModule::interruptModule()
{
    inAffPort.interrupt();
    outAffPort.interrupt();

    inToolAffPort.interrupt();
    outToolAffPort.interrupt();

    return true;
}

bool BD2YSModule::close()
{
    inAffPort.close();
    outAffPort.close();

    inToolAffPort.close();
    outToolAffPort.close();

    return true;
}

bool BD2YSModule::updateModule()
{
    // read blobs data
    inAff = inAffPort.read(mode=="whole");
    inToolAff = inToolAffPort.read(mode=="top"||mode=="bottom");

    if (inAff != NULL)
        sizeAff = static_cast<int>( inAff->get(0).asDouble() );

    if (inToolAff != NULL)
        sizeToolAff = static_cast<int>( inToolAff->get(0).asDouble() );

    if (sizeAff>0 || sizeToolAff>0)
        writeAff();

    return true;
}

double BD2YSModule::getPeriod()
{
    return 0.0;
}

void BD2YSModule::writeAff()
{
    if (mode == "whole")
    {
        Bottle &b = outAffPort.prepare();
        b.clear();

        const int numDesc = 37; // expected number of descriptors
        const int firstDescIndex = 23;
        
        if (inAff->tail().get(blobIndex).asList()->size() != numDesc)
            fprintf(stdout, "warning: blob %d has %d descriptors, was expecting %d\n", blobIndex, inAff->tail().get(blobIndex).asList()->size(), numDesc);

        for (int desc=firstDescIndex; desc<numDesc; desc++)
        {
            b.addDouble(inAff->tail().get(blobIndex).asList()->get(desc).asDouble());
        }

        outAffPort.write();
    }
    else
    {
        Bottle &b = outToolAffPort.prepare();
        b.clear();

        const int numDesc = 9; // expected number of descriptors
        const int firstDescIndex = 2;
        const int lastDescIndex = numDesc-2; // skip length

        if (inToolAff->tail().get(blobIndex).asList()->size() != 2)
            fprintf(stdout, "warning: parts has %d blobs, was expecting 2\n", inToolAff->tail().get(blobIndex).asList()->size());

        if (inToolAff->tail().get(blobIndex).asList()->get(mode=="top"?0:1).asList()->size() != numDesc)
            fprintf(stdout, "warning: blob %d has %d part descriptors, was expecting %d\n", blobIndex, inToolAff->tail().get(blobIndex).asList()->get(mode=="top"?0:1).asList()->size(), numDesc);

        for (int desc=firstDescIndex; desc<=lastDescIndex; desc++)
        {
            b.addDouble(inToolAff->tail().get(blobIndex).asList()->get(mode=="top"?0:1).asList()->get(desc).asDouble());
        }

        outToolAffPort.write();
    }
}
