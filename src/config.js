module.exports = {
    eos: {
        contractName: "todo", // Contract name
        contractSender: "todo", // User executing the contract (should be paired with private key),
        contractScope:"todo",
        costs:{
          creatorg: "200.0000 ",
          joinorg: "10.0000 ",
          addtodo: "10.0000 ",
          exchangeback: "5.0000 "

        },
        network: {
          protocol: "http",
          blockchain: "eos",
          host: "127.0.0.1",
          port: 8888,
          chainId: "cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f"  // get this using http://dev.cryptolions.io:38888/v1/chain/get_info
        },
        tokenName:"NST",
        tokenCode:"nsealstokens",
        eosOptions: {}
    },
  };