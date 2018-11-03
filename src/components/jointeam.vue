<template>
<div>
    <div class="field">
  <label class="label">Team Name</label>
  <div class="control">
    <input v-model="teamname" class="input" type="text" placeholder="your team  name">
  </div>
   <p class="help is-success">{{avateamname}}</p>
</div>

<div class="field">
  <label class="label">Your nicke name</label>
  <div class="control has-icons-left has-icons-right">
    <input class="input is-success" v-model="nickname" type="text" placeholder="your nickname" >
    <span class="icon is-small is-left">
      <i class="fas fa-user"></i>
    </span>
    <span class="icon is-small is-right">
      <i class="fas fa-check"></i>
    </span>
  </div>
 
</div>


<div class="field">
  <label class="label">Message</label>
  <div class="control">
    <textarea class="textarea" placeholder="Textarea"></textarea>
  </div>
</div>

<div class="field">
  <div class="control">
    <label class="checkbox">
      <input type="checkbox">
      I agree to the <a href="#">terms and conditions</a>
    </label>
  </div>
</div>

 <!-- <div class="isImageModalActive">
  <div class="modal-background"></div>
  <div class="modal-content">
   {{message}}
  </div>
  <button class="modal-close is-large" aria-label="close"></button>
</div> -->


<div class="field">
  <div class="control">
    <button class="button is-link" v-on:click="jointeam" :disabled="disablebtn">Submit</button>
  </div>
</div>
</div>
</template>
<script>
// import teamhandler from '../libs/teamhandler'
// var th = new teamhandler();
const config = require("../config.js");
let contrat = null;
export default {
    name:'jointeam',
   data() {
    return {
      teamname: "",
      nickname:"",
      disablebtn:true,
      // isImageModalActive: "modal is-active",
      // message:"your team has been created",
      allTeams:[]
    };
  },
    async mounted(){

         this.$th.getallorgs().then(loadTeams=>{
             this.allTeams = loadTeams;
             this.$root.$emit('needreloadteams', 'OK');
         });       
    },
    computed:{
        avateamname:function(){
           if(this.teamname == null || this.teamname=='')
            return '';    
           var dupteams =this.allTeams.filter(item=>{
                    return item.orgname==this.teamname
                })
                if(dupteams==null || dupteams.length==0){
                    this.disablebtn = true;
                    return "This team is not exist"
                }else{
                    this.disablebtn = false;
                     return "This team name is exist"
                }
        }
    },
    methods:{
        jointeam() {
            //first transfer 200 NST with createorg memo
            const account = this.$store.getAccount();
            var ts = Date.parse(new Date());
            const transactionOptions = { authorization:[`${account.name}@${account.authority}`] };
            const eos = this.$store.getEosClient();
            const nickname = this.nickname;
            const orgname = this.teamname;
            const secret = ts;
            const transferMsg = config.eos.costs.joinorg+config.eos.tokenName;
        
            eos.contract(config.eos.tokenCode).then((contract)=>{
            console.log(transferMsg)
            contract.transfer(account.name,config.eos.contractName,transferMsg,'jointeam-'+ts).then(()=>{
                console.log("transfer is done");
                eos.contract(config.eos.contractName).then((contract)=>{
                    console.log(account.name,nickname,orgname,secret);
                contract.jointeam(account.name,nickname,orgname,secret,transactionOptions).then(data=>{
                    console.log("join org is done"+data);
                    this.$root.$emit('needreloadteams', 'OK');
                    this.message= "your have join this team";
                    // this.isImageModalActive = "modal is-active";
                })
                })
            })
            });
        }
    }
    // watch:{
    //     teamname:function(){
    //         console.log('avateamname');
    //         th.getorgs(this.$store.getEosClient()).then(loadTeams=>{
    //             var dupteams =loadTeams.filter(item=>{
    //                 return item.orgname==this.teamname
    //             })
    //             if(dupteams==null || dupteams.length==0){
    //                 return "This team name is available"
    //             }else{
    //                  return "This team name is exist"
    //             }
    //         })
    //     }
    // }

}
</script>
