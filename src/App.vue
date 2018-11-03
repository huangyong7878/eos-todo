<template>
   <section  class="hero is-light is-fullheight">
    <div class="hero-head" >
          <nav class="navbar has-text-black has-background-info">
                <div class="container">
                    <div class="navbar-brand">
                        <a class="navbar-item" href="../">
                            <img src="./assets/logo.png" alt="Logo">
                            <p> </p>
                            <p class="column subtitle has-text-white">
                                NineSeals
                            </p>
                            
                        </a>
                        
                        <span 
                        class="navbar-burger burger" data-target="navbarMenu">
                            <span></span>
                            <span></span>
                            <span></span>
                        </span>
                    </div>
                    <div id="navbarMenu" class="navbar-menu">
                        <div class="navbar-end">
                            <span class="navbar-item">
                                <a class="button is-white is-outlined" href="#">
                                    <span class="icon">
                                        <i class="fa fa-home"></i>
                                    </span>
                                    <span>Home</span>
                                </a>
                            </span>
                            <span class="navbar-item">
                                <a class="button is-white is-outlined" href="#/exchange">
                                    <span class="icon">
                                        <i class="fa fa-superpowers"></i>
                                    </span>
                                    <span>{{nstbalance}}</span>
                                </a>
                            </span>
                            <span class="navbar-item">
                                <a class="button is-white is-outlined" href="#/exchange">
                                    <span class="icon">
                                        <i class="fa fa-book"></i>
                                    </span>
                                    <span>{{eosbalance}}</span>
                                </a>
                            </span>                    
                        </div>
                    </div>
                </div>
         </nav>
    </div> 
     <div class="hero-body">
      <aside class="menu">
  <p class="menu-label">
    General
  </p>
  <ul class="menu-list">
     <li><router-link to="/board">Dashboard</router-link></li>
  </ul>
  <p class="menu-label">
    Teams
  </p>
  <ul class="menu-list">
  
    <li>
      <!-- <a class="is-active">Teams</a> -->
      <ul id="v-for-teams">

        <li v-for="team in teams" :key="team.orgid"><router-link :to="{ name: 'teams', params: { id: team.orgid,orgname:team.teamname }}">{{team.teamname}}</router-link></li>
        <!-- <li><router-link to="/addteam">Add a team</router-link></li>
        <li><router-link to="/jointeam">Join a team</router-link></li> -->
      </ul>
    </li>
    <li><router-link to="/addteam">Add a team</router-link></li>
    <li><router-link to="/jointeam">Join a team</router-link></li>
  </ul>
  <p class="menu-label">
    Transactions
  </p>
  <ul class="menu-list">
     <li><router-link to="/rates">Rates</router-link></li>
     <li><router-link to="/exchange">Exchange</router-link></li>
     <li><router-link to="/transfer">Transfer</router-link></li>
    <li><a>Balance</a></li>
  </ul>
</aside>   
    
  <div class="container column">
          <router-view></router-view>
  </div>
</div>  

    
</section>
  
  
</template>

<script>


// import ScatterJS from "scatter-js/dist/scatter.esm";
const config = require("./config.js");
import { timer } from 'vue-timers'
export default {
  name: "App",
  timers: {
    updateBalance: { time: 5000, autostart: true,repeat:true }
  },
  components: {
    // sidebar
  },
  data() {
    return {
      identified: false,
      teams:[],
      nstbalance:'',
      eosbalance:'',
      msg: "Welcome to NineSeals's TODO"
    };
  },
  methods:{
    updateBalance(){
      // console.log('execute updateBalance');
       this.$root.$emit("updateNST","reloadNST");
         this.$root.$emit("updateEOS","reloaEOS");
        
    },
     
    async reloadTeam(){
      //  var account = this.$store.getAccount();
        var that = this;
       this.$th.getaccountorgs().then(teams=>{       
         if(teams!=null){
           that.teams = teams.filter(item=>{
             return true
           })
         }
       }).catch(err=>{
         console.log(err);    
         this.$root.$emit('catcherror',"you see this view because your <b>scatter wallet</b> has not been configured correctly.")   ;
       });
    }
  },
  async mounted() {
    await this.reloadTeam();
    var that = this;
    this.$root.$on('needreloadteams', function (msg) { // 
      console.log('need reload teams')
      that.reloadTeam();
    });
    this.$root.$on('updateNST',msg=>{
      this.$th.getBalance().then(balance=>{
        this.nstbalance=balance;
      })
    });
     this.$root.$on('updateEOS',msg=>{
      this.$th.getEosBalance().then(balance=>{
        this.eosbalance=balance;
      })
    });
    // this.$root.$on('updateNST',msg=>{
    //   this.nstbalance = msg;
    // })
    this.$root.$on('catcherror',msg=>{
          this.$dialog.alert({
                    title: 'Error',
                    message:  msg,   
                    type: 'is-danger',
                    hasIcon: true,
                    icon: 'times-circle',
                    iconPack: 'fa'
                })
    })
  }
};
</script>

<style>
</style>

