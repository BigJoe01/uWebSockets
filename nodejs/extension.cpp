#include "uWS.h"
#include "addon.h"
#include "../env.h"
#include "../env-inl.h"

uv_prepare_t prepare;

namespace node {

void Main(Local<Object> exports, Local<Value> unused, Local<Context> context) {
    Environment* env = Environment::GetCurrent(context);
    Isolate *isolate = exports->GetIsolate();

    exports->Set(String::NewFromUtf8(isolate, "server"), Namespace<uWS::SERVER>(isolate).object);
    exports->Set(String::NewFromUtf8(isolate, "client"), Namespace<uWS::CLIENT>(isolate).object);

    env->SetMethod(exports, "setUserData", setUserData<uWS::SERVER>);
    env->SetMethod(exports, "getUserData", getUserData<uWS::SERVER>);
    env->SetMethod(exports, "clearUserData", clearUserData<uWS::SERVER>);
    env->SetMethod(exports, "getAddress", getAddress<uWS::SERVER>);

    env->SetMethod(exports, "transfer", transfer);
    env->SetMethod(exports, "upgrade", upgrade);
    env->SetMethod(exports, "connect", connect);

    uv_prepare_init(hub.getLoop(), &prepare);
    uv_prepare_start(&prepare, [](uv_prepare_t *prepare) {
        makeCallbackSkipCount = 0;
    });
}

}

NODE_MODULE_CONTEXT_AWARE_BUILTIN(uws_builtin, node::Main)
