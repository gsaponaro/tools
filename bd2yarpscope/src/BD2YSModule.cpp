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

    inAffPortName = "/" + moduleName + "/desc:i";
    inAffPort.open(inAffPortName.c_str());

    outAffPortName = "/" + moduleName + "/desc:o";
    outAffPort.open(outAffPortName.c_str());
    
    blobIndex = rf.check("index", Value(0)).asInt(); // idx of blob to process
    fprintf(stdout, "started %s on blob %d\n", moduleName.c_str(), blobIndex);

    inAff = NULL;
    firstRun = true;

    return true;
}

bool BD2YSModule::interruptModule()
{
    inAffPort.interrupt();
    outAffPort.interrupt();

    return true;
}

bool BD2YSModule::close()
{
    inAffPort.close();
    outAffPort.close();

    return true;
}

bool BD2YSModule::updateModule()
{
    // read blobs data
    inAff = inAffPort.read(false);
    if (inAff == NULL)
        return true;

    sizeAff = static_cast<int>( inAff->get(0).asDouble() );

    if (sizeAff>0) 
        writeAff();

    return true;
}

double BD2YSModule::getPeriod()
{
    return 0.0;
}

void BD2YSModule::writeAff()
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
