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

#include "ARequest.h"

#include <ctype.h>
#include "RLooperRoster.h"

namespace android {

ARequest::ARequest(uint32_t what, RLooper::handler_id target)
        : mWhat(what),
          mTarget(target)
{
}

ARequest::~ARequest() {
    // clear();
}

uint32_t ARequest::what() const {
    return mWhat;
}

void ARequest::setWhat(uint32_t what) {
    mWhat = what;
}

void ARequest::setTarget(RLooper::handler_id handlerID) {
    mTarget = handlerID;
}

RLooper::handler_id ARequest::target() const {
    return mTarget;
}

void ARequest::post(int64_t delayUs) {
    extern RLooperRoster gRLooperRoster;

    gRLooperRoster.postRequest(this, delayUs);
}

void ARequest::cancel() {
    extern RLooperRoster gRLooperRoster;
    gRLooperRoster.cancelRequest(this);
}

}  // namespace android
