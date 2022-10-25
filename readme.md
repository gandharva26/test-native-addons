#### This project was developed to create a boiler plate for converting C++ library code to Javascript executable file (Node.Js)

# Setup

## Prerequisites

Before starting to use **Node-API** you need to assure you have the following
prerequisites:

* **Node.JS** see: [Installing Node.js](https://nodejs.org/)
* **Python** see: [InstallingPython](https://www.python.org/downloads/release/python-3100/)
* **C++** see: [Installing C++](https://gcc.gnu.org/install/)

For MacOS
```
brew install node@14
brew install python
brew install gcc
```

Steps to run this project-
```
npm install 
npm run build
node index.js
```

make sure you run npm run build again after changing c++ files.

Also when you add a new header file/cpp file :
```
Add it to binding.gyp
Add it to main.cpp
Do npm rebuild and access it via JS.
```





## Refer this step by step guide to wrap your C++ code into a Javascript executable file (Node.js)
  

1. Let's first Write some C++ functions that we need

- Create a folder cppsrc for storing C++ files and then create following files inside it:
- Create a folder 'Samples' inside cppsrc and setup the functional CPP file (C++ functions) and header(C++ headers) inside cppsrc/Samples

/* cppsrc/functionalexample.cpp */
a)functionalexample.cc

```
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
```

Now let's add some wrapping for the functions in the same file. For every function in C++ we want to export we will basically create a NAPI wrapped function (HelloWrapped in this example) and add it to the exports object using Init.
Lets take some time to understand HelloWrapped function. Every wrapped function that needs to be exported to JS should have 
input params/return value from the Napi namespace.
Every wrapped function takes in CallbackInfo as the input parameter. 
This contains things like the context and the input parameters that needs to be passed to the function.
Init function is used to just set the export key as hello with corresponding wrapped function HelloWrapped 

```
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

Napi::Object functionexample::Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(
"hello", Napi::Function::New(env, functionexample::HelloWrapped)
  );
  exports.Set("add", Napi::Function::New(env, functionexample::AddWrapped));
   exports.Set("loop", Napi::Function::New(env, functionexample::LoopWrapped));
  return exports;
}

```

Ultimately your  functionalexample.cpp file should look something like this:
```
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

Napi::Object functionexample::Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(
"hello", Napi::Function::New(env, functionexample::HelloWrapped)
  );
  exports.Set("add", Napi::Function::New(env, functionexample::AddWrapped));
   exports.Set("loop", Napi::Function::New(env, functionexample::LoopWrapped));
  return exports;
}
```


Now let's Setup header file inside /cppsrc/Samples
/* cppsrc/functionalexample.h */
b)functionalexample.h
```
#include <napi.h>
namespace functionexample {
  std::string hello();
  Napi::String HelloWrapped(const Napi::CallbackInfo& info);

int add(int a, int b);
Napi::Number AddWrapped(const Napi::CallbackInfo& info);

int loop(int i);
Napi::Number LoopWrapped(const Napi::CallbackInfo& info);

Napi::Object Init(Napi::Env env, Napi::Object exports);

}
```


Finally, let's set up the Main file - main.cpp


/* cppsrc/main.cpp */

```
#include <napi.h> // node - api module includes the napi header file so that we can access all the helper macros (#define ls = x.y.z), classes and functions.
#include "Samples/functionexample.h"
Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return functionexample::Init(env, exports);
}

NODE_API_MODULE(testaddon, InitAll)

```
Something about NAPI -
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
InitAll and it takes two parameters which are passed by N-API. 
First parameter env is the context that needs to be passed on to most N-API function and exports is the object used to set 
the exported functions and classes via N-API.

* This code defines the entry-point for the Node addon, it tells Node where to go
* once the library has been loaded into active memory. The first argument must
* match the "target" in our *binding.gyp*. Using NODE_GYP_MODULE_NAME ensures
* that the argument will be correct, as long as the module is built with
* node-gyp (which is the usual way of building modules). The second argument
* points to the function to invoke. The function must not be namespaced.

2. Now let's create a folder structure like this and initialize a package.json file:
- Folder structure
<img width="258" alt="Screen Shot 2022-10-13 at 7 30 59 PM" src="https://user-images.githubusercontent.com/55717003/195748640-10d131eb-d802-4335-b6c0-4cab2cfbe9a5.png">



Let's Set up node.Js Dependencies
```
npm install node-gyp --save-dev
npm install node-addon-api
```



3. Setup package.json which contains information about all node.js packages and dependencies
```
{
  "name": "test-addon",
  "version": "1.0.0",
  "description": "",
  "main": "index.js",
  "gypfile": true,
  "scripts": {
    "build": "node-gyp rebuild",
    "clean": "node-gyp clean"
  },
  "author": "",
  "license": "ISC",
  "devDependencies": {
    "node-gyp": "^3.7.0"
  },
  "dependencies": {
    "node-addon-api": "^1.3.0"
  }
}

```
4. Setup binding.gyp which acts as the binding file between C++ and Javascript and gives us pre-compiled C++ files

```
{
    "targets": [{
        "target_name": "testaddon",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
            "cppsrc/main.cpp",
             "cppsrc/Samples/functionexample.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'libraries': [],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }]
}
```


5. Setup index.js which serves as the Entry for our Library
```
const testAddon = require('./build/Release/testaddon.node')
console.log('addon', testAddon)


function add(a, b){
return a + b;
}

function loop(i){
    let count;
    let j;
    for(j=0; j < i;j ++){
count = i;
    }
    return 0;
}

console.log('hello ', testAddon.hello());

const cPlusPlusAddTime = "c++ Addition"
const jsTime = "JS Addition"
console.time(cPlusPlusAddTime);
console.log('add ', testAddon.add(5, 12));
console.timeEnd(cPlusPlusAddTime);


console.time(jsTime);
console.log('add ', add(5 ,12));
console.timeEnd(jsTime);

console.time(cPlusPlusAddTime);
console.log('add ', testAddon.add(5, 12));
console.timeEnd(cPlusPlusAddTime);

console.log("js loop")
console.time();
console.log('loop', loop(1000));
console.timeEnd();

console.log("C++ loop")
console.time();
console.log('loop', testAddon.loop(1000));
console.timeEnd();


module.exports = testAddon

/*
We added a simple add function.
We added the wrapper for the add function : AddWrapped which is used to interface the add function with N-API.
We added the key add to export the AddWrapped function to the JS.*/


```

Finally, Phew! we are done making our C++ functions executable through Javascript

Read more at: 

https://www.npmjs.com/package/node-gyp?activeTab=readme

https://github.com/nodejs/node-addon-api

https://blog.atulr.com/node-addon-guide/

https://medium.com/jspoint/a-simple-guide-to-load-c-c-code-into-node-js-javascript-applications-3fcccf54fd32
