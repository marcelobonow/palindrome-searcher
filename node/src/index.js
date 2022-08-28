const StartSearching = require("./piDigits.js");
const { GetPiDigits } = require("./piDigits");

async function GetMultipleDigits() {
  ///TODO: Fazer em multithread
  const batchDigitSize = 1000;
  const palindromeSize = 9;
  for (let i = 0; i < 1000; i++) {
    const startDigit = Math.max(0, (i * batchDigitSize - (palindromeSize - 1)));
    const result = await GetPiDigits(startDigit, batchDigitSize, palindromeSize);
    console.log(result);
    if (result != "Not found") {
      console.log("RESULTADO!");
      await sleep(30000);
    }
  }
  StartSearching.GetPiDigits();
}
GetMultipleDigits();

function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}