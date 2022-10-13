#include "functionexample.h"
int count;
std::string functionexample::hello(){
  return "Hello World";
}

int functionexample::add(int a, int b){
  return a + b;
}

int functionexample::loop(int i){
  for(int j = 0; j< i ; j++){
count = j;
  }
  return 0;
}

Napi::String functionexample::HelloWrapped(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();
  Napi::String returnValue = Napi::String::New(env, functionexample::hello());
  return returnValue;
}

Napi::Number functionexample::AddWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    }

   Napi::Number first = info[0].As<Napi::Number>();
   Napi::Number second = info[1].As<Napi::Number>();

    int returnValue = functionexample::add(first.Int32Value(), second.Int32Value());

    return Napi::Number::New(env, returnValue);
}

Napi::Number functionexample::LoopWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsNumber() ) {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    }
   Napi::Number first = info[0].As<Napi::Number>();
    int returnValue = functionexample::loop(first.Int32Value());
    return Napi::Number::New(env, returnValue);
}



// set the function here
Napi::Object functionexample::Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(
"hello", Napi::Function::New(env, functionexample::HelloWrapped)
  );
  exports.Set("add", Napi::Function::New(env, functionexample::AddWrapped));
   exports.Set("loop", Napi::Function::New(env, functionexample::LoopWrapped));
  return exports;
}

/*For every function in C++ we want to export we will basically create a NAPI wrapped function (HelloWrapped in this example) and add it to the exports 
object using Init.
Lets take some time to understand HelloWrapped function. Every wrapped function that needs to be exported to JS should have 
input params/return value from the Napi namespace.
Every wrapped function takes in CallbackInfo as the input parameter. 
This contains things like the context and the input parameters that needs to be passed to the function.
Init function is used to just set the export key as hello with corresponding wrapped function HelloWrapped . */