const StartSearching = require("./piDigits.js");
const { FindPrimePalindrome } = require("./piDigits");

let primePalindromes = [];

async function GetMultipleDigits() {
  ///TODO: Fazer em multithread
  const batchSize = 10;
  const apiDigitSize = 1000;
  const palindromeSize = 9;
  const offset = 0;
  const batchesIndex = Array.from(Array(batchSize).keys());
  const promises = batchesIndex.map(batchIndex => GetDigits(batchIndex, batchSize, apiDigitSize, palindromeSize, offset));
  await Promise.all(promises);
  console.log("Finalizou");
}

async function GetDigits(batchIndex, batchSize, apiDigitSize, palindromeSize, offset) {
  for (let i = 0; i < 1000; i++) {
    const startBatchDigit = batchSize * apiDigitSize * i;
    const overlap = (batchSize * batchIndex + i) * (palindromeSize - 1);
    const startDigit = Math.max(0, (startBatchDigit + (apiDigitSize * batchIndex) - overlap + offset));
    console.log(`Busca iniciando em ${startDigit} e terminando em ${startDigit + apiDigitSize}`);
    const result = await FindPrimePalindrome(startDigit, apiDigitSize, palindromeSize);
    console.log(result);
    if (result != "Not found") {
      console.log("RESULTADO! " + result);

      console.log("Digito com palíndromo primo: " + result.digit);
      console.log("palíndromo primo: " + result.number);
      await sleep(30000);
      break;
    }
  }
}

GetMultipleDigits();

function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}