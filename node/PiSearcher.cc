#include <node.h>
#include <string>
#include <iostream>
#include <cinttypes>

namespace PiSearcher
{
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Number;
  using v8::Object;
  using v8::String;
  using v8::Value;

  struct PrimePalindromes
  {
  public:
    uintmax_t number;
    uintmax_t index;
  };

  struct FindPrimePalindromesResult
  {
  public:
    intmax_t palindromeIndex;
    bool isPrime;
  };

  FindPrimePalindromesResult FindPrimePalindromes(const char *, unsigned int, uintmax_t);
  std::vector<unsigned int> *FindPalindromes(const char *, unsigned int, unsigned int);
  bool IsPrime(uintmax_t);
  uintmax_t GetPalindromeNumber(const char *, unsigned int, unsigned int);

  void FindPalindromeNodeInterface(const FunctionCallbackInfo<Value> &args)
  {
    Isolate *isolate = args.GetIsolate();

    if (!args[0]->IsString())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid digits").ToLocalChecked()));
      return;
    }
    if (!args[1]->IsNumber())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid palindrome size").ToLocalChecked()));
      return;
    }
    if (!args[2]->IsNumber())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid start digit").ToLocalChecked()));
      return;
    }

    v8::String::Utf8Value str(isolate, args[0]);
    std::string digitsString(*str);

    unsigned int palindromeSize = args[1].As<Number>()->Value();
    unsigned int startDigit = args[2].As<Number>()->Value();
    unsigned int digitsSize = digitsString.length();
    // Local<Number> num = Number::New(isolate, palindromeSize);

    FindPrimePalindromesResult piSearchResult = FindPrimePalindromes(digitsString.c_str(), palindromeSize, digitsSize);
    std::string result;
    /// a: não encontrou
    /// b: encontrou mas não é primo
    /// c: encontrou e é primo
    if (piSearchResult.palindromeIndex < 0)
      result = "a";
    else if (piSearchResult.isPrime)
    {
      std::cout << "Encontrou resultado " << piSearchResult.palindromeIndex << std::endl;
      auto piDigitString = std::to_string(piSearchResult.palindromeIndex);
      result = "c:" + piDigitString;
    }
    else
    {
      auto piDigitString = std::to_string(piSearchResult.palindromeIndex);
      result = "b:" + piDigitString;
    }

    args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
  }

  void Initialize(Local<Object> exports)
  {
    NODE_SET_METHOD(exports, "FindPalindrome", FindPalindromeNodeInterface);
  }

  FindPrimePalindromesResult FindPrimePalindromes(const char *digits, unsigned int palindromeSize, uintmax_t digitsLength)
  {
    // std::vector<PrimePalindromes> *primePalindromes = new std::vector<PrimePalindromes>();
    std::vector<unsigned int> *palindromesIndex = FindPalindromes(digits, digitsLength, palindromeSize);
    FindPrimePalindromesResult result;
    if (palindromesIndex->size() >= 0)
    {
      for (std::vector<unsigned int>::iterator palindromeIterator = palindromesIndex->begin(); palindromeIterator != palindromesIndex->end();
           ++palindromeIterator)
      {
        ///É necessário tirar o buffer padding porque na primeira rodada não há buffer padding
        auto palindromeNumber = GetPalindromeNumber(digits, *palindromeIterator, palindromeSize);
        auto palindromeStartingIndex = *palindromeIterator + 1;
        result.palindromeIndex = palindromeStartingIndex;
        if (IsPrime(palindromeNumber))
        {
          std::cout << "Prime palindrome number: " << palindromeNumber << "\n";
          std::cout << "Prime palindrome start index: " << palindromeStartingIndex << "\n";
          // PrimePalindromes primePalindrome;
          // primePalindrome.number = palindromeNumber;
          // primePalindrome.index = palindromeStartingIndex;
          // primePalindromes->push_back(primePalindrome);
          result.isPrime = true;
          return result;
        }
        else
        {
          result.isPrime = false;
          std::cout << "Palindrome " << palindromeNumber << " is not prime \n";
        }
      }
    }
    result.palindromeIndex = -1;
    return result;
  }

  std::vector<unsigned int> *FindPalindromes(const char *buffer, unsigned int bufferSize, unsigned int palindromeSize)
  {
    auto radarLength = palindromeSize;
    auto radar = new char[radarLength];

    std::vector<unsigned int> *results = new std::vector<unsigned int>();

    for (unsigned int bufferIndex = 0; bufferIndex <= (bufferSize - radarLength); bufferIndex++)
    {
      /// TODO: tirar o radar, tirando o memcpy extra
      memcpy(radar, buffer + bufferIndex, radarLength);
      auto found = true;
      /// A verificação é com o espelhamento do valor em cima do palindromo
      /// se tamanho do palindromo = 3
      /// 0 compara com 5, 1 compara com 4, 2 compara com 3

      for (int radarIndex = 0; radarIndex < palindromeSize; radarIndex++)
      {
        auto leftDigit = radar[radarIndex];
        auto rightDigit = radar[radarLength - 1 - radarIndex];

        /// Lida com o caso de pelo tamanho do buffer, ter ultrapassado o tamanho do arquivo
        if (leftDigit == '\0' || rightDigit == '\0')
        {
          found = false;
          break;
        }
        if (leftDigit != rightDigit)
        {
          found = false;
          break;
        }
      }
      if (found)
        results->push_back(bufferIndex);
    }

    return results;
  }

  uintmax_t GetPalindromeNumber(const char *buffer, unsigned int index, unsigned int palindromeSize)
  {
    char *substring = new char[palindromeSize];
    memcpy(substring, buffer + index, palindromeSize);
    uintmax_t number = std::strtoimax(substring, nullptr, 10);
    return number;
  }

  bool IsPrime(uintmax_t number)
  {
    for (uintmax_t i = 2; i * i <= number; i++)
    {
      if (number % i == 0)
        return false;
    }
    return true;
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);
}