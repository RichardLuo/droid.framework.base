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

#ifndef ANDROID_HELLO_SERVICE_H
#define ANDROID_HELLO_SERVICE_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/Vector.h>
#include <utils/SortedVector.h>
#include <utils/KeyedVector.h>
#include <utils/threads.h>
#include <utils/RefBase.h>

#include <binder/BinderService.h>
#include <binder/Permission.h>
#include "IHelloServer.h"

namespace android {
// ---------------------------------------------------------------------------

class HelloService :
        public BinderService<HelloService>,
        public BnHelloServer
{
   friend class BinderService<HelloService>;

public:
    
    HelloService();
    
    virtual ~HelloService();

    virtual void onFirstRef();

    static char const* getServiceName() { return "HelloService"; }

    void sayHello(int32_t n);

};

// ---------------------------------------------------------------------------
}; // namespace android

#endif // ANDROID_HELLO_SERVICE_H
