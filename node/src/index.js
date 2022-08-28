const PiSearcher = require("../build/Release/PiSearcher");

console.time('c++');
console.log(PiSearcher.FindPalindrome("123456789", 9, 100));
console.timeEnd('c++');