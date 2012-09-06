/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_HELLO_INTERFACE_H
#define ANDROID_HELLO_INTERFACE_H


#include <stdint.h>
#include <math.h>
#include <sys/types.h>

#include <utils/SortedVector.h>
#include <utils/KeyedVector.h>
#include <utils/threads.h>
#include <utils/Atomic.h>
#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <utils/Singleton.h>
#include <utils/String16.h>

#include <binder/BinderService.h>
#include <binder/IServiceManager.h>

#include <IHelloServer.h>
#include "HelloService.h"
#include "NameMacros.h"


namespace android {

// ---------------------------------------------------------------------------

HelloService::HelloService()
//    :
//    Thread(false),
//      mDump("android.permission.DUMP"),
//      mInitCheck(NO_INIT)
{
}

void HelloService::onFirstRef()
{
    LOGE("nuHelloService starting...");

#if 0
    HelloDevice& dev(HelloDevice::getInstance());

    if (dev.initCheck() == NO_ERROR) {
        uint32_t virtualHellosNeeds =
                (1<<SENSOR_TYPE_GRAVITY) |
                (1<<SENSOR_TYPE_LINEAR_ACCELERATION) |
                (1<<SENSOR_TYPE_ROTATION_VECTOR);
        sensor_t const* list;
        int count = dev.getHelloList(&list);
        mLastEventSeen.setCapacity(count);
        for (int i=0 ; i<count ; i++) {
            registerHello( new HardwareHello(list[i]) );
            switch (list[i].type) {
                case SENSOR_TYPE_GRAVITY:
                case SENSOR_TYPE_LINEAR_ACCELERATION:
                case SENSOR_TYPE_ROTATION_VECTOR:
                    virtualHellosNeeds &= ~(1<<list[i].type);
                    break;
            }
        }

        if (virtualHellosNeeds & (1<<SENSOR_TYPE_GRAVITY)) {
            registerVirtualHello( new GravityHello(list, count) );
        }
        if (virtualHellosNeeds & (1<<SENSOR_TYPE_LINEAR_ACCELERATION)) {
            registerVirtualHello( new LinearAccelerationHello(list, count) );
        }
        if (virtualHellosNeeds & (1<<SENSOR_TYPE_ROTATION_VECTOR)) {
            registerVirtualHello( new RotationVectorHello(list, count) );
        }

        run("HelloService", PRIORITY_URGENT_DISPLAY);
        mInitCheck = NO_ERROR;
    }

#endif
}    
    

HelloService::~HelloService()
{
}

void HelloService::sayHello()
{
    LOGE("in HelloService::sayHello!!");
}


} // end of namespace android

#endif // ANDROID_HELLO_INTERFACE_H
