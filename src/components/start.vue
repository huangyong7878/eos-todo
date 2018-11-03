<template>
 
  <div id='main' class="hello">
    <h1>{{ msg }}</h1>
    <section>
      <section class="box" v-if="!identified" >
                <figure class="header">
                    <h1>
                        Oh you want to play NST TODO?
                    </h1>
                </figure>
                <p>
                    In order to save your data we're going to need your Identity.
                </p>

                <button v-on:click="requestIdentity">Provide Identity</button>
            </section>
    </section>

    <exchangeComp v-if="identified"></exchangeComp>

    <div v-if="identified">
    <h2>Links</h2>
    <ul>
      <li>
        <a
          href="https://vuejs.org"
          target="_blank"
        >
          create your team
        </a>
      </li>
      <li>
        <a
          href="https://forum.vuejs.org"
          target="_blank"
        >
          check team todos
        </a>
      </li>
      <li>
        <a
          href="https://chat.vuejs.org"
          target="_blank"
        >
          join team 
        </a>
      </li>
      <li>
        <a
          href="https://twitter.com/vuejs"
          target="_blank"
        >
          check assets
        </a>
      </li>
      <br>
      <li>
        <router-link to="/exchange">EOS/NST exchange</router-link>
          
        
      </li> 
    </ul>
      <router-view/>
  </div>
    <h2>Ecosystem</h2>
    <ul>
      <li>
        <a
          href="http://www.nineseals.com"
          target="_blank"
        >
          九个小海豹
        </a>
      </li>
      
    </ul>
  </div>
</template>

<script>
// const Eos = require("eosjs");
const config = require("../config.js");
import exchangeComp from '@/components/exchanges'


// const ScatterJS = require("scatter-js");

export default {
  name: "start",
  components:{
    exchangeComp
  },
  data() {
    return {
      identified:false,
      msg: "Welcome to NineSeals's TODO"
  
    };
  },
  methods: {
    requestIdentity() {
      this.$store.getScatter()
        .getIdentity({ accounts: [config.eos.network] })
        .then(id => {
          console.log(this.store);
          if (!id) return false;
          this.identified = true;
          console.log("id:" + id);
          var account = id.accounts[0];
          this.$store.setAccount(account);
          // this.$.exchangeComp.updateNST = true;


        //  this.store.
          // this[Actions.SET_IDENTITY](id);
          // SpaceInvaders.load(this.gameOver)
        })
        .catch(e => console.log(e));
    }
  }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
#main {
  /* color:#fbfbfb; */
}
h1,
h2 {
  font-weight: normal;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}

a {
  color: #42b983;
}
</style>
