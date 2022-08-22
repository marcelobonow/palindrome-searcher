#include <iostream>
#include <fstream>

int FindPalindrome(char*, unsigned int, unsigned int);
void PrintPalindrome(char*, unsigned int, unsigned int);

int main() {
	setlocale(LC_ALL, "");
	std::ifstream file("data/test.txt");

	const auto bufferSize = 2000000;
	std::cout << "Digite o tamanho do palindromo: ";
	std::string palindromeSizeInput;
	std::cin >> palindromeSizeInput;
	unsigned int palindromeSize = std::atoi(palindromeSizeInput.c_str());
	while (palindromeSize == 0 && strcmp(palindromeSizeInput.c_str(), "0") != 0) {
		std::cout << "Valor digitado inválido, tente novamente: ";
		std::cin >> palindromeSizeInput;
		palindromeSize = std::atoi(palindromeSizeInput.c_str());
	}
	char* buffer = (char*)malloc(sizeof(char) * bufferSize);
	while (file.get(buffer, bufferSize)) {
		std::cout << "Buffer" << "\n";
		///TODO: Append com os ultimos dígitos do buffer anterior + os primeiros dígitos do próximo buffer e envia para buscar o palindromo
		auto palindromeIndex = FindPalindrome(buffer, bufferSize, palindromeSize);
		if (palindromeIndex >= 0)
		{
			std::cout << "É palindromo no index " << palindromeIndex << "\n";
			PrintPalindrome(buffer, palindromeIndex, palindromeSize);
		}
		else
			std::cout << "Não é palindromo\n";
	}
}

int FindPalindrome(char* buffer, unsigned int bufferSize, unsigned int palindromeSize) {
	auto radarLength = palindromeSize * 2;
	auto radar = new char[radarLength];
	int index = 0;

	std::ofstream debugFile;
	debugFile.open("debug.txt");

	if (!debugFile)
	{
		std::cout << "Erro abrindo arquivo de debug!";
		exit(1);
	}


	for (int bufferIndex = 0; bufferIndex <= (bufferSize - radarLength); bufferIndex++)
	{
		memcpy(radar, buffer + bufferIndex, radarLength);
		auto found = true;
		///A verificação é com o espelhamento do valor em cima do palindromo
		///se tamanho do palindromo = 3
		///0 compara com 5, 1 compara com 4, 2 compara com 3

		for (int radarIndex = 0; radarIndex < palindromeSize; radarIndex++) {
			debugFile << "Verificando se " << radar[radarIndex] << " = " << radar[radarLength - 1 - radarIndex] << ". Buffer Index " << bufferIndex << "\n";
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

void PrintPalindrome(char* buffer, unsigned int index, unsigned int palindromeSize) {
	char* substring = new char[palindromeSize * 2 + 1];
	memcpy(substring, buffer + index, palindromeSize * 2);
	substring[palindromeSize * 2] = '\0';
	std::cout << substring << "\n";
}