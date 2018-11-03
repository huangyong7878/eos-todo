var items = [1,3,4,5,6]
const immediatePromise = () => new Promise((resolve) => setImmediate(resolve));
const timeoutPromise = (timeout) => new Promise((resolve) => setTimeout(resolve, timeout));
const randForTen = async () => {
  let results = [];
  
  for (let i = 0; i < 10; i++) {
    await immediatePromise(1000);
    results.push(Math.random());
  }
  
  return results;
}
const a1 = async (items) => {
  for (let i = 0; i < items.length; i++) {
    const result = await randForTen;
    console.log(result().then(a=>{
        console.log(a)
    }));
  }
}


const randAsyncTimer = (i) => {
    // Timeout within 1 second
    const timeout = Math.floor(Math.random() * 1000);
    return new Promise((resolve) => setTimeout(() => resolve(i), timeout));
  };
  
  const race = async () => {
    let calls = [randAsyncTimer(1), randAsyncTimer(2), randAsyncTimer(3)];
    // Start the race
    const result = await Promise.race(calls);
    console.log(result);
    // Possible outputs are between 1 to 3
  }

race();

var testa = [];
testa.push(1);
testa.push(2);
testa.unshift(3);
console.log(testa);

