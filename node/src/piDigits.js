const axios = require("axios");
const PiSearcher = require("../build/Release/PiSearcher");

const apiURL = "https://api.pi.delivery/v1/pi";

async function FindPrimePalindrome(startDigit, numberOfDigits, palindromeSize, batchIndex) {
  if (isNaN(startDigit) || isNaN(numberOfDigits)) {
    console.log(`[${batchIndex}] Invalid start digit or number of digits`);
    return;
  }

  return new Promise(async (resolve, reject) => {
    try {
      const result = await axios.get(`${apiURL}?start=${startDigit}&numberOfDigits=${numberOfDigits}&radix=10`);
      const digits = result.data.content;
      const piSearchResult = PiSearcher.FindPalindrome(digits, palindromeSize, startDigit);
      if (piSearchResult[0] != "a") {
        const piDigit = Number.parseInt(piSearchResult.split(':')[1]);
        const piNumber = digits.substr(piDigit - 1, palindromeSize);
        resolve({
          digit: piDigit,
          number: piNumber,
          isPrime: piSearchResult[0] == 'c',
        });
        return;
      }
      else {
        resolve({
          digit: -1,
          number: -1,
          isPrime: false,
        });
        return;
      }
    }
    catch (error) {
      console.log("startDigit: " + startDigit + ", batchIndex: " + batchIndex);
      console.error(`[${batchIndex}] Digit: ${startDigit} API error: `, error);
      console.log('\x1b[31m%s\x1b[0m', `[${batchIndex}] Digit: ${startDigit} API status error: ${error.code}, ${error.errno}`);
      await sleep(500);
      resolve(await FindPrimePalindrome(startDigit, numberOfDigits, palindromeSize, batchIndex));
    }
  });
}

function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}

module.exports.FindPrimePalindrome = FindPrimePalindrome;