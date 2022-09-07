const axios = require("axios");
const { performance } = require('perf_hooks');

const apiURL = "https://api.pi.delivery/v1/pi";

async function Test() {
  const numberOfDigits = 1000;
  let startDigit = 0
  let stringResult = "";
  const startTime = performance.now();
  console.log("Iniciando");
  for (let i = 0; i < 1000; i++) {
    console.log("Loop ", i, " ", performance.now() - startTime);
    const result = await axios.get(`${apiURL}?start=${startDigit}&numberOfDigits=${numberOfDigits}&radix=10`);
    stringResult += result.data.content;
    startDigit += numberOfDigits;
  }
  console.log("Finalizou em ", performance.now() - startTime, " ms");

}
Test();