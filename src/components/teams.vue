<template>
       <div>
         <div id="cards" class="card">
           <article class="tile is-child notification is-primary">
            <p class="title">Team {{teamname}}</p>
            <div class="control">
             <button class="button is-link" v-on:click="showtododialog">Create your todo</button>
          </div>
          </article> 
          


        </div>
   
         <br> 
          <b-field grouped group-multiline>
           
            <b-field label="Items per page">
                <b-input type="number" v-model="perPage"></b-input>
            </b-field> 
                  
          </b-field>
          <br>
         <section class="content" >
             <div class="field">
              <b-switch type="is-info" v-model="showdoing">Doing</b-switch>
              <b-switch  type="is-success" v-model="showdone">Done</b-switch>
              <b-switch type="is-warning" v-model="showdelay">Delay</b-switch>
              <b-switch  type="is-danger" v-model="showclosed">Closed</b-switch>
            </div>
         </section>
        <nav class="navbar" role="navigation" aria-label="main navigation">
          <div id="navbarBasicExample" class="navbar-menu">
            <div class="navbar-start">
                <a class="navbar-item" v-on:click="moveleft">>
                  Left
                </a>
            </div>
            <div class="navbar-end">
                <a class="navbar-item" v-on:click="moveright">
                  right
                </a>
            </div>
          </div>
        </nav>          
         
        </b-pagination>
        <p>
        </p>  
        <div class="card" v-for="card in todocards" :key="card.id">
          <todocard :teamid="teamid" :todoid="card.id" :content="card.content" :owner="card.owner" :payer="card.payer" :title="card.title" :bonus="card.bonus" :status="card.r" :dudate="card.endtime"></todocard>  
        </div>
        <b-modal :active.sync="isCardModalActive"  scroll="keep">
           <div class="modal-background"></div>
           <div class="modal-card">
            <!-- <header class="modal-card-head">
              <p class="modal-card-title">New TODO</p>
              <button class="delete" aria-label="close"></button>
            </header> -->
           <form action="">
                <div class="modal-card" style="width: auto">
                    <header class="modal-card-head">
                        <p class="modal-card-title">New TODO</p>
                    </header>
                    <section class="modal-card-body">
                        <b-field label="">
                            <b-input
                                type="text"
                                v-model="newtodo.title"
                                placeholder="todo title"
                                icon="newspaper"
                                required>
                            </b-input>
                        </b-field>  
                        <b-field >
                            <b-input
                              
                                v-model="newtodo.bonus"                                      
                                placeholder="NST Bonus"
                                icon="coins"
                                expanded
                                required>
                            </b-input>      
                          <b-select  placeholder="From" v-model="newtodo.payer" icon="users" expanded>
                            <option v-for="mate in teammates" :key="mate.id" :value="mate.useraccount">{{mate.nickname}}</option>                          
                          </b-select>
                        </b-field>
                         <b-field label="" >
                          <b-datepicker
                              placeholder="day"
                              icon="calendar" v-model="newtodo.day"  editable>
                          </b-datepicker>
                         
                           <b-timepicker v-model="time"
                            placeholder="time" expanded icon="clock">

                            <button class="button is-primary"
                              @click="time = new Date()">
                              <b-icon icon="clock"></b-icon>
                              <span>Now</span>
                            </button>

                            <button class="button is-danger"
                              @click="time = null">
                              <b-icon icon="close"></b-icon>
                              <span>Clear</span>
                              </button>
                          </b-timepicker>
                        </b-field>
                          

                      
                        <b-field label="Content">
                         <b-input type="textarea" v-model="newtodo.content" ></b-input>
                        </b-field>                       
                    </section>
                    <footer class="modal-card-foot">
                        <button class="button" type="button" @click="isCardModalActive=false">Close</button>
                        <button class="button is-primary" @click="addtodo">Create</button>
                    </footer>
                </div>
            </form>
          </div>
        </b-modal>
       </div>                 
</template>

<script>
import Vue from 'vue'
const config = require("../config.js");
let contrat = null;
import todocard from '@/components/todocard'
// import addtodocard from '@/components/todos'
export default {
  name: "teams",
  data(){
    return {
      teamid:this.$route.params.id,
      teamname:this.$route.params.orgname,
      isCardModalActive:false,
      newtodo:{},
      teammates:[],
      time:new Date(),
      showdoing:true,
      showdone:true,
      showdelay:false,
      showclosed:false,
      left:false,
      perPage:10,      
      lower:0,
      upper:0,
      todosrs:{leftmore:false,rigthmore:true},
      todocards:[]
    };  
  },
   components: {
    todocard,
    // addtodocard
  },
   methods:{
     moveleft(){
      //  if( this.todosrs.leftmore)
        this.loadtodo(-1);
     },
     moveright(){
      // if( this.todosrs.rigthmore)
       this.loadtodo(1);
     },
    async loadtodo(r){
      var rs =[];
      if(this.showdoing)
        rs.push(0);
      if(this.showdone)
        rs.push(1) ;
      if(this.showdelay)   
        rs.push(2);
      if(this.showclosed)
        rs.push(3); 
      this.todocards=[];
      var that = this;
      await this.$th.loadtodo(this.teamid,this.perPage,r<0?this.lower:this.upper,r<0,rs).then(todors=>{
        var rows = todors.todos;
        if(r<0){
          this.todosrs.leftmore = todors.more;
        }
        else
          this.todosrs.rigthmore = todors.more;
        if(rows.length!=0){
          this.lower =  rows[0].id; 
          this.upper = rows[rows.length-1].id+1;
          if(r<0)
            this.todosrs.rigthmore = true;
          else  
            this.todosrs.leftmore = true;
        } else{
          if(r<0){
            this.lower =0 ;
            this.upper = 0;
          }else{
            this.upper = this.upper+1
            this.lower = this.upper;
          }
        }
        this.todocards = rows.filter(item=>{
          console.log(item.r);
         
          return true;
        });
        
        // console.log(todos);
        that.teammates = [];
        that.loadteammates(that.teamid);
      })
    },
    async loadteammates(){
      this.teammates = [];
      var that =this;
       console.log("before",that.teammates);
      this.$th.loadteammates(this.teamid).then(teammates=>{
        that.teammates = teammates.filter((mate=>{
          return mate.orgid==that.teamid;
        }))
        console.log("after",that.teammates);
      })
    },
    closedialog(){
      this.isCardModalActive = false;
    },
    addtodo(){
      console.log('addtodo');
      var newtodo = this.newtodo;
      this.isCardModalActive=false;
      var day = newtodo.day.getDay();
      // this.newtodo.day.setTime(this.time.getTime());
      newtodo.day.setHours(this.time.getHours());
      newtodo.day.setMinutes(this.time.getMinutes());
      
      newtodo.duedate= newtodo.day.getTime();
     
      var nstbonus = Number(newtodo.bonus).toFixed(4);
      newtodo.nstbonus = nstbonus;
      console.log(this.newtodo);
    
      // checker, teamname, title, content, timestamp, bonus
      this.$th.addtodo(newtodo.payer,this.teamname,newtodo.title,newtodo.content,newtodo.duedate,newtodo.nstbonus);

    },
    showtododialog(){
      // console.log('addtodo');
      this.newtodo = {};
      this.isCardModalActive=true;
     
      // addtodo(checker, teamname, title, content, timestamp, bonus) 
      // var newtodo = this.newtodo;
      //  this.$th.addtodo(newtodo.payer,)
      // this.todocards.unshift({todoid:1,title:"todo1",content:"what you would like to do",owner:"brayan",payer:"balong",bonus:"200.0000 NST",status:'2',dudate:"2018-10-23"})  ;
      
    },

    resetpage(){
      this.todosrs.rigthmore = true;
      this.todosrs.leftmore = false;
      this.lower = 0;
      this.upper = 0;
    }

  },
  async mounted(){
    console.log('mounted')
    this.loadtodo(1);
    this.$root.$emit('needreloadteams', 'OK');
    
  },
   watch: {
   showdoing:function(newval,oldval){
        this.resetpage();
        this.loadtodo(1);
    },
    showdone:function(newval,oldval){
      this.resetpage();
      this.loadtodo(1);
    },
    showclosedfunction(newval,oldval){
      this.resetpage();
      this.loadtodo(1);
    },
    showdelay:function(newval,oldval){
      this.resetpage();
      this.loadtodo(1);
    },  
    perPage:function(newval,oldval){
        this.resetpage();
      this.loadtodo(1);
    },
   '$route' (to, from) {
     this.teamid = to.params.id;
     this.teamname = to.params.orgname;
      this.$root.$emit('needreloadteams', 'OK');
     this.resetpage();
      this.loadtodo();
     
    },
    
    
  }
};
</script>
<style scoped>

</style>


