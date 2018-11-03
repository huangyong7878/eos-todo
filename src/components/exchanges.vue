<template>

    <div >
        
     <section>
        <!-- <div class="block">
            <b-switch v-model="allowNew">
                Allow new tags
            </b-switch>
        </div> -->
        <b-field label="Deposit EOS">
            <b-input
                v-model="exhcnageInput"
                placeholder="EOS"
                >
            </b-input>
        </b-field>
        <button class="button" v-on:click="exchange">Deposit</button>
        <pre style="max-height: 400px"><b>NST:</b>  {{equalNST}}</pre>
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
  name: "exchangeComp",
  data() {
    return {
      exhcnageInput:'0',
    };
  },
  computed:{
      equalNST:function(){
          var num =  this.exhcnageInput * 100;
          return num.toFixed(4);
      }
  },

  
  methods: {
    exchange(){
        const account = this.$store.getAccount();
        const transactionOptions = { authorization:[`${account.name}@${account.authority}`] };
        const eos = this.$store.getEosClient();
        eos.transfer(account.name,config.eos.contractSender,(this.exhcnageInput*1).toFixed(4)+" EOS",'exchange').then(trx => {
            // That's it!
            console.log(`Transaction ID: ${trx.transaction_id}`);
        }).catch(error => {
            console.error(error);
        });
    }
  }
};
</script>

