/*
 * System server main initialization.
 *
 * The system server is responsible for becoming the Binder
 * context manager, supplying the root ServiceManager object
 * through which other services can be found.
 */

#define LOG_TAG "sysproc"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/TextOutput.h>
#include <utils/Log.h>

#include <HelloService.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <cutils/properties.h>

using namespace android;

namespace android {
/**
 * This class is used to kill this process when the runtime dies.
 */
class GrimReaper : public IBinder::DeathRecipient {
public: 
    GrimReaper() { }

    virtual void binderDied(const wp<IBinder>& who)
    {
        LOGI("Grim Reaper killing system_server...");
        kill(getpid(), SIGKILL);
    }
};

} // namespace android



extern "C" status_t system_init()
{
    LOGI("Entered system_init()");
    
    sp<ProcessState> proc(ProcessState::self());
    
    sp<IServiceManager> sm = defaultServiceManager();
    LOGI("ServiceManager: %p\n", sm.get());
    
    sp<GrimReaper> grim = new GrimReaper();
    sm->asBinder()->linkToDeath(grim, grim.get(), 0);
    
    char propBuf[PROPERTY_VALUE_MAX];
    property_get("system_init.startsurfaceflinger", propBuf, "1");

    LOGI("before HelloService::instantiate()");
    // Start the sensor service
    HelloService::instantiate();
    LOGI("after HelloService::instantiate()");
        
    // If running in our own process, just go into the thread
    // pool.  Otherwise, call the initialization finished
    // func to let this process continue its initilization.
    if (proc->supportsProcesses()) {
        LOGI("System server: entering thread pool.");
        ProcessState::self()->startThreadPool();
        IPCThreadState::self()->joinThreadPool();
        LOGI("System server: exiting thread pool.");
    } else {
        LOGI("System server: does not support proc->supportsProcesses.");
    }
    
    return NO_ERROR;
}

