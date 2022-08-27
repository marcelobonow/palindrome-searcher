#include <iostream>
#include <fstream>
#include <string>
#include <cinttypes>

//void Input();
//void WaitToExit();
int FindPalindrome(char*, unsigned int, unsigned int);
uintmax_t GetPalindromeNumber(char*, unsigned int, unsigned int);
bool IsPrime(uintmax_t);

unsigned int palindromeSize = 0;
unsigned int bufferMultiplier = 1;

int main(int argc, char** argv) {
	setlocale(LC_ALL, "");

	auto fileLocation = argc > 1 ? argv[1] : "data/pi.txt";
	palindromeSize = argc > 2 ? std::atoi(argv[2]) : 9;
	bufferMultiplier = argc > 3 ? std::atoi(argv[3]) : 100000;
	std::ifstream file(fileLocation);

	std::cout << "Searching palindrome of length " << palindromeSize << " with buffer multiplier " << bufferMultiplier << " on file " << fileLocation << "\n";

	///Tratamento de input
	//Input();


	/// O palindromo pode ser cortado pela divisória do buffer, então precisamos alocar um offset
	///No pior caso, o palindromo cortado precisa de 2*palindromeSize - 1
	auto bufferSize = palindromeSize * bufferMultiplier;
	auto bufferPadding = (palindromeSize - 1);
	auto totalBufferSize = bufferSize + bufferPadding;
	char* buffer = (char*)malloc(sizeof(char) * totalBufferSize);
	memset(buffer, 0, totalBufferSize);
	auto bufferPage = 0;
	while (file.get(buffer + bufferPadding, bufferSize + 1)) {
		std::cout << "Page " << bufferPage << "\n";
		///TODO: Append com os ultimos dígitos do buffer anterior + os primeiros dígitos do próximo buffer e envia para buscar o palindromo
		auto palindromeIndex = FindPalindrome(buffer, totalBufferSize, palindromeSize);
		if (palindromeIndex >= 0)
		{
			///É necessário tirar o buffer padding porque na primeira rodada não há buffer padding
			auto palindromeNumber = GetPalindromeNumber(buffer, palindromeIndex, palindromeSize);
			auto palindromeStartingIndex = (bufferPage * bufferSize) + palindromeIndex - bufferPadding + 1;
			if (IsPrime(palindromeNumber))
			{
				std::cout << "Prime palindrome number: " << palindromeNumber << "\n";
				std::cout << "Prime palindrome start index: " << palindromeStartingIndex << "\n";
				break;
			}
			else
			{
				std::cout << "Palindrome " << palindromeNumber << " is not prime \n";
			}
		}

		///Passa os ultimos digitos para os primeiros, para usar na próxima busca
		memcpy(buffer, (buffer + bufferSize), bufferPadding);
		bufferPage++;
	}
	return 0;
}

//void Input() {
//	std::cout << "Digite o tamanho do palindromo: ";
//	std::string palindromeSizeInput;
//	std::cin >> palindromeSizeInput;
//	palindromeSize = std::atoi(palindromeSizeInput.c_str());
//	while (palindromeSize <= 0 && strcmp(palindromeSizeInput.c_str(), "0") != 0) {
//		std::cout << "Valor digitado inválido, tente novamente: ";
//		std::cin >> palindromeSizeInput;
//		palindromeSize = std::atoi(palindromeSizeInput.c_str());
//	}
//
//	std::cout << "Digite o multiplicador do buffer: ";
//	std::string bufferMultiplierInput;
//	std::cin >> bufferMultiplierInput;
//	bufferMultiplier = std::atoi(bufferMultiplierInput.c_str());
//	while (bufferMultiplier <= 0 && strcmp(bufferMultiplierInput.c_str(), "0") != 0) {
//		std::cout << "Valor digitado inválido, tente novamente: ";
//		std::cin >> bufferMultiplierInput;
//		bufferMultiplier = std::atoi(bufferMultiplierInput.c_str());
//	}
//}

//void WaitToExit() {
//	std::cout << "Enter para sair...\n";
//	std::cin.get();
//	std::cin.get();
//	exit(0);
//}

int FindPalindrome(char* buffer, unsigned int bufferSize, unsigned int palindromeSize) {
	auto radarLength = palindromeSize;
	auto radar = new char[radarLength];

	for (int bufferIndex = 0; bufferIndex <= (bufferSize - radarLength); bufferIndex++)
	{
		memcpy(radar, buffer + bufferIndex, radarLength);
		auto found = true;
		///A verificação é com o espelhamento do valor em cima do palindromo
		///se tamanho do palindromo = 3
		///0 compara com 5, 1 compara com 4, 2 compara com 3

		for (int radarIndex = 0; radarIndex < palindromeSize; radarIndex++) {
			/*debugFile << "Verificando se " << radar[radarIndex] << " = " << radar[radarLength - 1 - radarIndex] << ". Buffer Index " << bufferIndex << "\n";*/
			auto leftDigit = radar[radarIndex];
			auto rightDigit = radar[radarLength - 1 - radarIndex];

			///Lida com o caso de pelo tamanho do buffer, ter ultrapassado o tamanho do arquivo
			if (leftDigit == '\0' || rightDigit == '\0')
				return -1;
			if (leftDigit != rightDigit)
			{
				found = false;
				break;
			}
		}
		if (found)
			return bufferIndex;
	}

	return -1;
}

uintmax_t GetPalindromeNumber(char* buffer, unsigned int index, unsigned int palindromeSize) {
	char* substring = new char[palindromeSize];
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