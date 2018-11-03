<template>
    <div class="card ">
    <header class="card-header">
        <p class="card-header-title">
            {{title}}
        </p>
        <!-- <a href="#" class="card-header-icon" aria-label="more options">
        <span class="icon">
            <i class="fas fa-angle-down" aria-hidden="true"></i>
        </span>
        </a> -->
    </header>
    <div class="card-content">
        <div class="content">
        <div class="subtitle">
            {{content}}
        </div>
        
        <p>
            <span class="icon">
                <i class="fas fa-stopwatch"></i>
            </span>
            Due Date {{dudate}}
        </p>
        <p>
            <span class="icon">
                <i class="fas fa-user"></i>
            </span>
            owner {{owner}}
        </p>
        
        <p>
            <span class="icon">
                <i class="fas fa-user-tie"></i>
            </span>
            checker {{payer}}
        </p>

        <p>
            <span class="icon">
                <i class="fas fa-heart"></i>
            </span>
            bonus {{bonus}}
        </p>

        <p v-bind:class="statusClsObj">
            <span class="icon">
                <i class="fas fa-thermometer-half"></i>
            </span>
            status {{cardstatus}}
        </p>
      
      
        </div>
    </div>
    <footer class="card-footer">
        <a class="card-footer-item button  is-outlined"   v-on:click="donetodo" :disabled="disablestatus" >Done</a>
        <a href="#" class="card-footer-item button is-primary  is-outlined"  v-on:click="delaytodo" :disabled="disablestatus" >Delay</a>
        <a href="#" class="card-footer-item button  is-primary is-outlined"  v-on:click="closetodo" :disabled="disablestatus" >Delete</a>
    </footer>
    </div>
</template>
<script>
const config = require("../config.js");
export default {
    name:'todocard',
    data:function(){
        return {
            cardstatus:this.getstatusstr(this.status)   ,
            disablestatus:this.status!=0,
            statusClsObj:this.getstatusclsobj(this.status)
        }
    },
    props:[        
            'teamid',
            'todoid',
            'title',
            'content',
            'dudate',
            'status',
            'owner',
            'payer',
            'bonus'
    ],
    methods:{
       getstatusclsobj(status){
           switch(status){
               case 0:
                return 'has-text-primary';
              case 1:
                return 'has-text-success';
              case 2:
                return 'has-text-warning';
              case 3:
                return 'has-text-grey-light';      
           }
       },
       getstatusstr(status){
           switch(status){
               case 0:            
                return "doing";
               case 1:
                return "done";
               case 2:
                return "fail";
               case 3:    
                return "closed";
           }
       } ,
       donetodo(){
            //first transfer 200 NST with createorg memo
            const account = this.$store.getAccount();
            var ts = Date.parse(new Date());
            const transactionOptions = { authorization:[`${account.name}@${account.authority}`] };
            const eos = this.$store.getEosClient();
          
            const secret = ts;
            const transferMsg = this.bonus ;
        
            eos.contract(config.eos.tokenCode).then((contract)=>{
            console.log(transferMsg)
            var that = this;
            contract.transfer(account.name,config.eos.contractName,transferMsg,'checktodo-'+ts).then(()=>{
                console.log("transfer is done");
                eos.contract(config.eos.contractName).then((contract)=>{
                contract.checktodo(account.name, that.todoid, 1, secret, transactionOptions).then(data=>{
                    console.log("todo:"+that.todoid+ "done "+data);
                    that.cardstatus = that.getstatusstr(1);
                })
                })
            })
            });
       } ,
        delaytodo(){
            const account = this.$store.getAccount();
            var ts = Date.parse(new Date());
            const transactionOptions = { authorization:[`${account.name}@${account.authority}`] };
            const eos = this.$store.getEosClient();
            const secret = ts;
            var that = this;
            eos.contract(config.eos.contractName).then((contract)=>{
                contract.checktodo(account.name, that.todoid, 2, secret, transactionOptions).then(data=>{
                    console.log("todo:"+that.todoid+ "delay "+data);
                    that.cardstatus = that.getstatusstr(2);
            });
           });
        },
        closetodo(){
             const account = this.$store.getAccount();
            var ts = Date.parse(new Date());
            const transactionOptions = { authorization:[`${account.name}@${account.authority}`] };
            const eos = this.$store.getEosClient();
            const secret = ts;
            var that = this;
            eos.contract(config.eos.contractName).then((contract)=>{
                contract.closetodo(account.name, that.todoid,transactionOptions).then(data=>{
                    console.log("todo:"+that.todoid+ "close "+data);
                    that.cardstatus = that.getstatusstr(3);
            });
           });
        }
    }
    
}
</script>
