#include <iostream>

bool IsPrime(__int128_t);

__int128_t atoint128_t(const char *s)
{
  const char *p = s;
  __int128_t val = 0;

  if (*p == '-' || *p == '+')
  {
    p++;
  }
  while (*p >= '0' && *p <= '9')
  {
    val = (10 * val) + (*p - '0');
    p++;
  }
  if (*s == '-')
    val = val * -1;
  return val;
}

std::string GetNumberAsString(__uint128_t number)
{
  uint64_t lower = (uint64_t)number;
  uint64_t high = (number >> 64);
  return std::to_string(high) + std::to_string(lower);
}

int main(int argc, char **argv)
{
  __int128_t number;
  if (argc > 1)
    number = atoint128_t(argv[1]);
  else
  {
    std::string input;
    std::cout << "Enter a 128bit number to check if is prime: ";
    std::cin >> input;
    number = atoint128_t(input.c_str());
  }
  auto isPrime = IsPrime(number);
  std::cout << "Is prime: " << (isPrime ? "YES" : "NO") << std::endl;
}

bool IsPrime(__int128_t number)
{
  if (number % 2 == 0)
    return false;
  for (__int128_t i = 3; i * i <= number; i += 2)
  {
    if (number % i == 0)
    {
      std::cout << "Not prime, divisible by: " << GetNumberAsString(i) << std::endl;
      return false;
    }

    if (i % 1000001 == 0)
    {
      printf("%s\n", GetNumberAsString(i).c_str());
      __int128_t remaining = (number - (i * i));
      printf("remaining: %s\n", GetNumberAsString(remaining).c_str());
    }
  }
  return true;
}