const { performance } = require('perf_hooks');
const StartSearching = require("./piDigits.js");
const { FindPrimePalindrome } = require("./piDigits");
const fs = require("fs");
const readlineSync = require('readline-sync');
const util = require('util');

const appendFile = util.promisify(fs.appendFile);


///TODO: Colocar resultado em arquivos
let primePalindromes = [];
let startDigitBatches = [];
let smallerDigitFound = -1;
let errorFile;
let outputFile;
let searchSize = 2;
let lastUpdateTime;

async function GetMultipleDigits() {
  errorFile = fs.createWriteStream('./errors.log');
  process.stderr.write = errorFile.write.bind(errorFile);
  outputFile = fs.createWriteStream('./output.log');
  ///TODO: Fazer em multithread
  const silentMode = readlineSync.question('Silent mode (true|false):') || "false";
  const batchSize = parseInt(readlineSync.question('batchSize (default 100):')) || 100;
  const offset = parseInt(readlineSync.question('offset (default 0):')) || 0;
  searchSize = parseFloat(readlineSync.question('how many billions to search (float, default 2):')) || 2;
  // const silentMode = false;
  // const batchSize = 100;
  // const offset = 19000000000;
  process.env.SILENT = silentMode;
  const apiDigitSize = 1000;
  const palindromeSize = 21;
  const batchesIndex = Array.from(Array(batchSize).keys());
  const promises = batchesIndex.map(batchIndex => GetDigits(batchIndex, batchSize, apiDigitSize, palindromeSize, offset));
  lastUpdateTime = performance.now();
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
    ///It has 1000 times less because the API retrieves 1000 digits each time 
    const oneBillionLoop = 1000000;
    const loopQuantity = oneBillionLoop * searchSize;

    for (let i = 0; i < loopQuantity && (smallerDigitFound < 0 || smallerDigitFound > startDigit); i++) {
      let nowTime = performance.now();
      if (nowTime - lastUpdateTime >= 5000) {
        console.log("\n\n");
        console.log(`loop took ${(nowTime - lastUpdateTime) / 1000} seconds`);

        const indexSmallestDigit = GetSmallestBatchDigitIndex();
        const smallestStartDigit = startDigitBatches[indexSmallestDigit];
        console.log(`startDigit: ${smallestStartDigit}. ${smallestStartDigit.toExponential(4)}`);
        lastUpdateTime = nowTime;
        await appendFile("./output.log", `[${indexSmallestDigit}] Menor busca: ${smallestStartDigit}\r\n`);
        lastUpdateTime = nowTime;
      }
      const startBatchDigit = i * batchSize * apiDigitSize;
      const startInteractionDigit = batchIndex * apiDigitSize;
      const overlap = (i * batchSize + batchIndex) * (palindromeSize - 1);
      startDigit = Math.max(0, (startBatchDigit + (apiDigitSize * batchIndex) - overlap + offset));
      startDigitBatches[batchIndex] = startDigit;
      if (process.env.SILENT != "true") {
        console.log(`[${batchIndex}] Busca iniciando em ${startDigit} e terminando em ${startDigit + apiDigitSize}`);
      }
      const result = await FindPrimePalindrome(startDigit, apiDigitSize, palindromeSize, batchIndex);
      if (result.isPrime && result.digit >= 0 && result.number >= 0) {
        console.log("RESULTADO! ", result);

        const absoluteDigit = (result.digit + startDigit);
        console.log("Digito com palíndromo primo: " + absoluteDigit);
        console.log("palíndromo primo: " + result.number);
        if (smallerDigitFound > absoluteDigit || smallerDigitFound < 0)
          smallerDigitFound = absoluteDigit;

        primePalindromes.push({
          digit: absoluteDigit,
          number: result.number,
        });
        await appendFile("./output.log", `!!!!!!!!!!!!!!!!!![${batchIndex}][P] Encontrou palíndromo no digito: ${absoluteDigit}, número: ${result.number}\r\n`);
        await appendFile("./palindromes.log", `[${batchIndex}] Encontrou palíndromo no digito: ${absoluteDigit}, número: ${result.number}\r\n`);
        resolve();
        return;
      }
      else if (!result.isPrime && result.digit >= 0 && result.number >= 0) {
        console.log("Encontrou palíndromo não primo no digito " + result.digit);
        console.log("palíndromo não primo: " + result.number);
        await appendFile("./output.log", `[${batchIndex}][NP] Encontrou palíndromo não primo no digito: ${absoluteDigit}, número: ${result.number}\r\n`);
      }
    }
    resolve();
  });
}

function GetSmallestBatchDigitIndex() {
  let indexSmallestFound = 0;
  for (const batchIndex in startDigitBatches) {
    if (startDigitBatches[batchIndex] > startDigitBatches[indexSmallestFound])
      indexSmallestFound = batchIndex;
  }
  return indexSmallestFound;
}

function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}



GetMultipleDigits();