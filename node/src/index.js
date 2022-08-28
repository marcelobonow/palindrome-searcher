const StartSearching = require("./piDigits.js");
const { GetPiDigits } = require("./piDigits");

async function GetMultipleDigits() {
  ///TODO: Fazer em multithread
  const batchDigitSize = 1000;
  const palindromeSize = 9;
  const offset = 100000;
  for (let i = 0; i < 1000; i++) {
    const startDigit = Math.max(0, (i * batchDigitSize - (palindromeSize - 1) + offset));
    console.log(`Busca iniciando em ${startDigit}`);
    const result = await GetPiDigits(startDigit, batchDigitSize, palindromeSize);
    console.log(result);
    if (result != "Not found") {
      console.log("RESULTADO! " + result);
      await sleep(30000);
      break;
    }
  }
  console.log("Finalizou");
}

GetMultipleDigits();

function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}