<template>

    <div >
        
     <section>
      
        <b-field label="tranfer NST">

            <b-input
                v-model="nst"
                placeholder="NST"
                >
            </b-input>
        </b-field>
        <b-field label="to">
                            <b-input
                                type="text"
                                v-model="to"
                                placeholder="todo title"
                                icon="newspaper"
                                required>
                            </b-input>
        </b-field>  
        <button class="button" v-on:click="transfer">transfer</button>
        <!-- <pre style="max-height: 400px"><b>NST:</b>  {{equalNST}}</pre> -->
    </section>
        <!-- <p> your current NST balance is {{nstbalance}}</p>
        <p> your current EOS balance is {{eosbalance}}</p>
        <div>
            <label>deposit </label><input v-model="exhcnageInput"><label> EOS</label>
            <p>you can get {{equalNST}} NST</p>
            <button  v-on:click="exchange">exchange NST</button>
        </div> -->
    </div>    
</template>
    


<script>
const config = require("../config.js");
var _ = require("lodash");
export default {
  name: "transfer",
  data() {
    return {
      nst:'',
      to:''
    };
  },
  
  methods: {
   async transfer(){
        const account = this.$store.getAccount();
        const transactionOptions = { authorization:[`${account.name}@${account.authority}`] };
        const eos = this.$store.getEosClient();
        var contract  = await eos.contract(config.eos.tokenCode)
        contract.transfer(account.name,this.to,(this.nst*1).toFixed(4)+" "+config.eos.tokenName,'nothing').then(trx => {
            // That's it!
            console.log(`Transaction ID: ${trx.transaction_id}`);
        }).catch(error => {
            console.error(error);
        });
    }
  }
};
</script>

