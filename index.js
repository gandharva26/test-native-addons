const testAddon = require('./build/Release/testaddon.node')
console.log('addon', testAddon)


function add(a, b){
return a + b;
}

function loop(i){
    let count;
    let j;
    for(j=0; j < i;j ++){
count = i;
    }
    return 0;
}

console.log('hello ', testAddon.hello());

const cPlusPlusAddTime = "c++ Addition"
const jsTime = "JS Addition"
console.time(cPlusPlusAddTime);
console.log('add ', testAddon.add(5, 12));
console.timeEnd(cPlusPlusAddTime);


console.time(jsTime);
console.log('add ', add(5 ,12));
console.timeEnd(jsTime);

console.time(cPlusPlusAddTime);
console.log('add ', testAddon.add(5, 12));
console.timeEnd(cPlusPlusAddTime);

console.log("js loop")
console.time();
console.log('loop', loop(1000));
console.timeEnd();

console.log("C++ loop")
console.time();
console.log('loop', testAddon.loop(1000));
console.timeEnd();


module.exports = testAddon

/*
We added a simple add function.
We added the wrapper for the add function : AddWrapped which is used to interface the add function with N-API.
We added the key add to export the AddWrapped function to the JS.*/