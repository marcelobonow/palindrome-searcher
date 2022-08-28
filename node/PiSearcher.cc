#include <node.h>
#include <string>

namespace pi_searcher
{
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;

  void FindPalindrome(const FunctionCallbackInfo<Value> &args)
  {
    Isolate *isolate = args.GetIsolate();

    if (!args[0]->IsNumber())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments").ToLocalChecked()));
      return;
    }

    std::string teste = "Teste OK";
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, teste.c_str()).ToLocalChecked());
  }

  void Initialize(Local<Object> exports)
  {
    NODE_SET_METHOD(exports, "FindPalindrome", FindPalindrome);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);
}