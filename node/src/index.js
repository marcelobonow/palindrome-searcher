const PiSearcher = require("../build/Release/PiSearcher");

console.time('c++');
console.log(PiSearcher.FindPalindrome(32));
console.timeEnd('c++');