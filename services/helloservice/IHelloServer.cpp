/******************************************************************
 * @file   IHelloServer.cpp
 * @author Richard Luo, cdominant7@gmail.com
 * @date   2011/07/15 08:22:38
 * 
 * @brief  Did he have passion?
 * 
 ****************************************************************** 
 */

#include "IHelloServer.h"

#include <stdint.h>
#include <sys/types.h>

#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <utils/Vector.h>
#include <utils/Timers.h>

#include <binder/Parcel.h>
#include <binder/IInterface.h>
#include "NameMacros.h"

namespace android {

enum {
    SAY_HELLO = IBinder::FIRST_CALL_TRANSACTION
};

class BpHelloServer : public BpInterface<IHelloServer>
{
public:
    BpHelloServer(const sp<IBinder>& impl)
        : BpInterface<IHelloServer>(impl)
    {
    }

    virtual void sayHello()
    {
        Parcel data, reply;
        data.writeInterfaceToken(IHelloServer::getInterfaceDescriptor());
        remote()->transact(SAY_HELLO, data, &reply);
    }

};

IMPLEMENT_META_INTERFACE(HelloServer, "android.gui.HelloServer");

status_t BnHelloServer::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code) {
    case SAY_HELLO:
        CHECK_INTERFACE(IHelloServer, data, reply);
        LOGD("before call sayHello!");
        this->sayHello();
        // Vector<Hello> v(getHelloList());
        // size_t n = v.size();
        // reply->writeInt32(n);
        // for (size_t i=0 ; i<n ; i++) {
        //     reply->write(static_cast<const Flattenable&>(v[i]));
        // }
        return NO_ERROR;
        break;
    }
    
    return BBinder::onTransact(code, data, reply, flags);
}

} // end of namespace android
