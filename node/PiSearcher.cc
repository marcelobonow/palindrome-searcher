#include <node.h>
#include <string>

namespace pi_searcher
{
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Number;
  using v8::Object;
  using v8::String;
  using v8::Value;

  void FindPalindrome(const FunctionCallbackInfo<Value> &args)
  {
    Isolate *isolate = args.GetIsolate();

    if (!args[0]->IsString())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments").ToLocalChecked()));
      return;
    }
    if (!args[1]->IsNumber())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments").ToLocalChecked()));
      return;
    }
    if (!args[2]->IsNumber())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments").ToLocalChecked()));
      return;
    }

    v8::String::Utf8Value str(isolate, args[0]);
    std::string cppStr(*str);

    unsigned int palindromeSize = args[1].As<Number>()->Value();
    unsigned int bufferSize = args[2].As<Number>()->Value();
    // Local<Number> num = Number::New(isolate, palindromeSize);

    args.GetReturnValue().Set(String::NewFromUtf8(isolate, cppStr.c_str()).ToLocalChecked());
  }

  void Initialize(Local<Object> exports)
  {
    NODE_SET_METHOD(exports, "FindPalindrome", FindPalindrome);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);
}