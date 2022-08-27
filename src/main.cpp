#include <iostream>
#include <fstream>
#include <string>

//void Input();
//void WaitToExit();
int FindPalindrome(char*, unsigned int, unsigned int);
void PrintPalindrome(char*, unsigned int, unsigned int);
void FindFirstPrime(const char*);


unsigned int palindromeSize = 0;
unsigned int bufferMultiplier = 1;
const char* outputFileLocation = NULL;

int main(int argc, char** argv) {
	setlocale(LC_ALL, "");

	auto fileLocation = argc > 1 ? argv[1] : "data/pi.txt";
	outputFileLocation = argc > 2 ? argv[2] : "output.txt";
	palindromeSize = argc > 3 ? std::atoi(argv[3]) : 9;
	bufferMultiplier = argc > 4 ? std::atoi(argv[4]) : 1000;
	std::ifstream file(fileLocation);

	std::cout << "Buscando palindromo de tamanho " << palindromeSize << " com multiplicador " << bufferMultiplier << " no arquivo " << fileLocation << "\n";

	FindFirstPrime(outputFileLocation);
	return 0;

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
		std::cout << "Pagina " << bufferPage << "\n";
		///TODO: Append com os ultimos dígitos do buffer anterior + os primeiros dígitos do próximo buffer e envia para buscar o palindromo
		auto palindromeIndex = FindPalindrome(buffer, totalBufferSize, palindromeSize);
		if (palindromeIndex >= 0)
		{
			///É necessário tirar o buffer padding porque na primeira rodada não há buffer padding
			std::cout << "É palindromo iniciando no digito: " << (bufferPage * bufferSize) + palindromeIndex - bufferPadding + 1 << " (contagem iniciando em 1)\n";
			PrintPalindrome(buffer, palindromeIndex, palindromeSize);
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

void PrintPalindrome(char* buffer, unsigned int index, unsigned int palindromeSize) {
	char* substring = new char[palindromeSize + 1];
	memcpy(substring, buffer + index, palindromeSize);
	substring[palindromeSize] = '\0';

	std::ofstream outputFile;
	outputFile.open("output.txt", std::ios_base::app);

	if (!outputFile)
	{
		std::cout << "Erro abrindo arquivo de saída!";
		exit(1);
	}

	outputFile << substring << "\n";
}

void FindFirstPrime(const char* fileLocation)
{
	uintmax_t number;
	std::ifstream file(fileLocation);
	while (file >> number) {
		std::cout << number << "\n";

		bool isPrime = true;
		for (uintmax_t i = 2; i * i <= number; i++)
		{
			if (number % i == 0)
			{
				std::cout << "Não é primo \n";
				isPrime = false;
				break;
			}
		}
		if (isPrime)
		{
			std::cout << "O primeiro número primo: " << number << "\n";
			break;
		}
	}

}