const { performance } = require('perf_hooks');
const StartSearching = require("./piDigits.js");
const { FindPrimePalindrome } = require("./piDigits");
const fs = require("fs");
const readlineSync = require('readline-sync');
const util = require('util');

const appendFile = util.promisify(fs.appendFile);


///TODO: colocar no arquivo digito inicial e quantos vai fazer
let primePalindromes = [];
let startDigitBatches = [];
let smallerDigitFound = -1;
let errorFile;
let outputFile;
let searchSize = 2;
let index;
let lastUpdateTime;
let nextStartDigit;
let endDigit;
let startTime;

async function GetMultipleDigits() {
  ///TODO: Fazer em multithread
  index = readlineSync.question('index (for files, default 0):') || 0;
  const silentMode = readlineSync.question('Silent mode (true|false):') || "false";
  const batchSize = parseInt(readlineSync.question('batchSize (default 100):')) || 100;
  const offset = parseInt(readlineSync.question('offset (default 0):')) || 0;
  searchSize = parseFloat(readlineSync.question('how many billions to search (float, default 2):')) || 2;
  index = 0;
  // const silentMode = false;
  // const batchSize = 100;
  // const offset = 19000000000;
  // searchSize = 2;

  errorFile = fs.createWriteStream(`./errors${index}.log`);
  process.stderr.write = errorFile.write.bind(errorFile);
  outputFile = fs.createWriteStream(`./output${index}.log`);
  process.env.SILENT = silentMode;
  nextStartDigit = offset;
  endDigit = offset + 1000000000 * searchSize;
  const apiDigitSize = 1000;
  const palindromeSize = 21;
  const batchesIndex = Array.from(Array(batchSize).keys());
  const promises = batchesIndex.map(batchIndex => GetDigits(batchIndex, batchSize, apiDigitSize, palindromeSize, offset));
  await appendFile(`./output${index}.log`, `Searching from ${offset} to ${endDigit} \r\n`);
  startTime = performance.now();
  lastUpdateTime = performance.now();
  await Promise.all(promises);
  console.log("Finalizou promises");
  if (primePalindromes.length == 0) {
    console.log("Nenhum palíndromo encontrado");
    await appendFile(`./output${index}.log`, `Nenhum palíndromo encontrado\r\n`);
  }
  for (const prime of primePalindromes) {
    console.log('----------------------------');
    console.log("Digito: " + prime.digit);
    console.log("Numero: " + prime.number);

    await appendFile(`./output${index}.log`, `---------------------------\r\n`);
    await appendFile(`./output${index}.log`, `Digit: ${prime.digit}\r\n`);
    await appendFile(`./output${index}.log`, `Number: ${prime.number}\r\n`);
  }
  console.log('----------------------------');
  await appendFile(`./output${index}.log`, `----------------------------\r\n`);
}

async function GetDigits(batchIndex, batchSize, apiDigitSize, palindromeSize, offset) {
  return new Promise(async (resolve, reject) => {
    let startDigit = 0;
    ///It has 1000 times less because the API retrieves 1000 digits each time 
    const loopTimesToBillion = 1000000000 / (apiDigitSize * batchSize);
    console.log("Total loops: ", loopTimesToBillion);
    const loopQuantity = loopTimesToBillion * searchSize;

    for (let i = 0; nextStartDigit <= endDigit && (smallerDigitFound < 0 || smallerDigitFound > startDigit); i++) {
      let nowTime = performance.now();
      if (nowTime - lastUpdateTime >= 2000) {
        const secondsRunning = (nowTime - startTime) / 1000;
        const digitsPerHour = ((nextStartDigit - offset) / (secondsRunning)) * 3600;
        console.log("\n\n");
        console.log(`loop took ${(nowTime - lastUpdateTime) / 1000} seconds`);
        console.log(`doing ${digitsPerHour.toFixed(2)} digits per hour (${digitsPerHour.toExponential(3)}), running for ${secondsRunning.toFixed(2)} seconds`);


        const indexSmallestDigit = GetSmallestBatchDigitIndex();
        const smallestStartDigit = startDigitBatches[indexSmallestDigit];
        console.log(`[${batchIndex}] startDigit: ${smallestStartDigit}. ${smallestStartDigit.toExponential(4)}, iteration ${i}`);
        lastUpdateTime = nowTime;
        await appendFile(`./output${index}.log`, `[${indexSmallestDigit}] smallest digit: ${smallestStartDigit}\r\n`);
        await appendFile(`./output${index}.log`, `[${indexSmallestDigit}] doing ${digitsPerHour.toFixed(2)} digits per hour (${digitsPerHour.toExponential(3)}), running for ${secondsRunning.toFixed(2)} seconds\r\n`);
        lastUpdateTime = nowTime;
      }
      let startBatchDigit = nextStartDigit - (palindromeSize - 1);
      nextStartDigit = startBatchDigit + apiDigitSize;
      const startInteractionDigit = batchIndex * apiDigitSize;
      if (startBatchDigit < 0)
        startBatchDigit = 0;
      startDigitBatches[batchIndex] = startBatchDigit;
      if (process.env.SILENT != "true") {
        console.log(`[${batchIndex}] Busca iniciando em ${startBatchDigit} e terminando em ${nextStartDigit}`);
      }
      const result = await FindPrimePalindrome(startBatchDigit, apiDigitSize, palindromeSize, batchIndex);
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
        await appendFile(`./output${index}.log`, `!!!!!!!!!!!!!!!!!![${batchIndex}][P] Encontrou palíndromo no digito: ${absoluteDigit}, número: ${result.number}\r\n`);
        await appendFile(`./palindromes${index}.log`, `[${batchIndex}] Encontrou palíndromo no digito: ${absoluteDigit}, número: ${result.number}\r\n`);
        resolve();
        return;
      }
      else if (!result.isPrime && result.digit >= 0 && result.number >= 0) {
        console.log("Encontrou palíndromo não primo no digito " + result.digit);
        console.log("palíndromo não primo: " + result.number);
        await appendFile(`./output${index}.log`, `[${batchIndex}][NP] Encontrou palíndromo não primo no digito: ${absoluteDigit}, número: ${result.number}\r\n`);
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