const { performance } = require('perf_hooks');
const StartSearching = require("./piDigits.js");
const { FindPrimePalindrome } = require("./piDigits");
const fs = require("fs");
const readlineSync = require('readline-sync');


let primePalindromes = [];
let smallerDigitFound = -1;

async function GetMultipleDigits() {
  var access = fs.createWriteStream('./errors.log');
  process.stderr.write = access.write.bind(access);
  ///TODO: Fazer em multithread
  const silentMode = readlineSync.question('Silent mode (true|false):') || "false";
  const batchSize = parseInt(readlineSync.question('batchSize (default 100):')) || 100;
  const offset = parseInt(readlineSync.question('offset (default 0):')) || 0;
  // const silentMode = false;
  // const batchSize = 100;
  // const offset = 19000000000;
  process.env.SILENT = silentMode;
  const apiDigitSize = 1000;
  const palindromeSize = 21;
  const batchesIndex = Array.from(Array(batchSize).keys());
  const promises = batchesIndex.map(batchIndex => GetDigits(batchIndex, batchSize, apiDigitSize, palindromeSize, offset));
  await Promise.all(promises);
  console.log("Finalizou promises");
  if (primePalindromes.length == 0) {
    console.log("Nenhum palíndromo encontrado");
  }
  for (const prime of primePalindromes) {
    console.log('----------------------------');
    console.log("Digito: " + prime.digit);
    console.log("Numero: " + prime.number);
  }
  console.log('----------------------------');
}

async function GetDigits(batchIndex, batchSize, apiDigitSize, palindromeSize, offset) {
  return new Promise(async (resolve, reject) => {
    let startDigit = 0;
    let startTime = performance.now();
    const loopQuantity = 1000000 * 20;
    for (let i = 0; i < loopQuantity && (smallerDigitFound < 0 || smallerDigitFound > startDigit); i++) {
      if (batchIndex == 0 && i % 30 == 0) {
        const endTime = performance.now();
        console.log(`startDigit: ${startDigit}. ${startDigit.toExponential(4)}`);
        console.log(`loop took ${(endTime - startTime) / 1000} seconds`);
        startTime = performance.now();
      }
      const startBatchDigit = i * batchSize * apiDigitSize;
      const startInteractionDigit = batchIndex * apiDigitSize;
      const overlap = (i * batchSize + batchIndex) * (palindromeSize - 1);
      startDigit = Math.max(0, (startBatchDigit + (apiDigitSize * batchIndex) - overlap + offset));
      if (process.env.SILENT != "true") {
        console.log(`[${batchIndex}] Busca iniciando em ${startDigit} e terminando em ${startDigit + apiDigitSize}`);
      }
      const result = await FindPrimePalindrome(startDigit, apiDigitSize, palindromeSize, batchIndex);
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
        resolve();
        return;
      }
    }
    resolve();
  });

}

function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}
GetMultipleDigits();