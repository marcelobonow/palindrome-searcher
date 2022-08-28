const StartSearching = require("./piDigits.js");
const { FindPrimePalindrome } = require("./piDigits");

let primePalindromes = [];
let smallerDigitFound = -1;

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
  for (const prime of primePalindromes) {
    console.log('----------------------------');
    console.log("Digito: " + prime.digit);
    console.log("Numero: " + prime.number);
  }
  console.log('----------------------------');
}

async function GetDigits(batchIndex, batchSize, apiDigitSize, palindromeSize, offset) {
  let startDigit = 0;
  for (let i = 0; i < 1000 && (smallerDigitFound < 0 || smallerDigitFound > startDigit); i++) {
    const startBatchDigit = batchSize * apiDigitSize * i;
    const overlap = (batchSize * batchIndex + i) * (palindromeSize - 1);
    startDigit = Math.max(0, (startBatchDigit + (apiDigitSize * batchIndex) - overlap + offset));
    console.log(`Busca iniciando em ${startDigit} e terminando em ${startDigit + apiDigitSize}`);
    const result = await FindPrimePalindrome(startDigit, apiDigitSize, palindromeSize);
    console.log(result);
    if (result != "Not found") {
      console.log("RESULTADO! " + result);

      const absoluteDigit = (result.digit + startDigit);
      console.log("Digito com palíndromo primo: " + absoluteDigit);
      console.log("palíndromo primo: " + result.number);
      if (smallerDigitFound > absoluteDigit || smallerDigitFound < 0)
        smallerDigitFound = absoluteDigit;

      primePalindromes.push({
        digit: absoluteDigit,
        number: result.number,
      });
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