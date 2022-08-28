const axios = require("axios");
const PiSearcher = require("../build/Release/PiSearcher");

const apiURL = "https://api.pi.delivery/v1/pi";

async function FindPrimePalindrome(startDigit, numberOfDigits, palindromeSize, batchIndex) {
  if (isNaN(startDigit) || isNaN(numberOfDigits)) {
    console.log(`[${batchIndex}] Invalid start digit or number of digits`);
    process.exit();
  }

  return new Promise(async (resolve, reject) => {
    try {
      const result = await axios.get(`${apiURL}?start=${startDigit}&numberOfDigits=${numberOfDigits}&radix=10`);
      const digits = result.data.content;
      const piSearchResult = PiSearcher.FindPalindrome(digits, palindromeSize, startDigit);
      if (piSearchResult != "Not found") {
        const piDigit = Number.parseInt(piSearchResult);
        const piNumber = digits.substr(piDigit - 1, palindromeSize);
        resolve({
          digit: piDigit,
          number: piNumber
        });
        return;
      }
      else {
        resolve(piSearchResult);
        return;
      }
    }
    catch (error) {
      console.log("startDigit: " + startDigit + ", batchIndex: " + batchIndex);
      console.error('./errors.log', `[${batchIndex} Digit: ${startDigit} API error: ] ${error}`);
      console.log('./errors.log', `[${batchIndex} Digit: ${startDigit} API error: ] ${error}`);
      await sleep(10000);
      FindPrimePalindrome(startDigit, numberOfDigits, palindromeSize, batchIndex);
    }
  });
}

function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}

module.exports.FindPrimePalindrome = FindPrimePalindrome;