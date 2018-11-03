const config = require("../config.js");
import ScatterJS from "scatter-js/dist/scatter.esm";

import Eos from "eosjs";
// import wallethandler from './wallethandler'
//import Promise from 'es6-promise';

console.log('Promise', Promise)
export default class teamhandler {
  constructor(store) {
    this.store = store;
    // this.wh = new wallethandler(store);
  }
  async getBalance() {
    const eos = this.store.getEosClient();
    const account = this.store.getAccount();
    var nstbalcne = await eos
      .getCurrencyBalance(
        config.eos.tokenCode,
        account.name,
        config.eos.tokenName
      )
      .then(data => {
        if (data != null && data.length != 0) {
          return data[0];
        }
      })
    return nstbalcne;
  }

  async getEosBalance() {
    const eos = this.store.getEosClient();
    const account = this.store.getAccount();
    var eosbalcne = await eos
      .getCurrencyBalance("eosio.token", account.name, "EOS").then(data => {
        if (data != null && data.length != 0) {
          return data[0];
        } else
          return 0;
      })
    return eosbalcne;
  }

  async _getteamsbyaccount() {
    const eos = this.store.getEosClient();
    const account = this.store.getAccount();
    const table = await eos.getTableRows({
      scope: config.eos.contractName,
      code: config.eos.contractName,
      table: 'users',
      json: true,
      index_position: '3',
      key_type: 'i64',
      limit: 1000,
      lower_bound: account.name
    });
    console.log(table)
    return table.rows;
  }

  async _getAllTeam() {
    const eos = this.store.getEosClient();
    const account = this.store.getAccount();
    const table = await eos.getTableRows({
      scope: config.eos.contractName,
      code: config.eos.contractName,
      table: 'orgs',
      limit: 1000,
      json: true
    });
    console.log(table)
    return table.rows;
  }

  async getallorgs() {
    console.log("getallorgs")
    var that = this;
    return that._connecttoscatter().then(result => {
      if (result) {
        return that._getAllTeam();
      } else
        throw new Error("can't connect to Scatter")

    });
  }
  async getaccountorgs() {
    var that = this;

    return that._connecttoscatter().then(result => {
      if (result) {
        const account = this.store.getAccount();
        return that._getteamsbyaccount().then(rows => {
          var filterrows = rows.filter(item => {
            return item.useraccount == account.name;
          })
          return filterrows;
        });
      } else
        throw new Error("can't connect to Scatter")

    });

  }
  async _gettodotable(limits, lastid, left) {
    const eos = this.store.getEosClient();
    const account = this.store.getAccount();
    if (left) {
      const table = await eos.getTableRows({
        scope: config.eos.contractName,
        code: config.eos.contractName,
        table: 'todos',
        index_position: '1',
        key_type: 'i64',
        // limit: limits,
        // lower_bound: teamid,
        json: true,
        upper_bound: lastid
      });
      console.log(table)
      return table;
    } else {
      const table = await eos.getTableRows({
        scope: config.eos.contractName,
        code: config.eos.contractName,
        table: 'todos',
        index_position: '1',
        key_type: 'i64',
        // limit: limits,
        // lower_bound: teamid,
        json: true,
        lower_bound: lastid
      });
      console.log(table)
      return table;
    }

  }
  
  async readLimitedTodo(teamid,table,result,limits,left,rs) {
    var rows = table.rows;
    result.more = table.more;
    
    var rowId = -1;
    
    var filterTodos = rows.filter(item => {
      return(item.orgid == teamid && rs.includes(item.r)); 
    })

    var shift = limits-filterTodos.length-result.todos.length;
    if(left && shift<0){
      for(var i=shift;i<0;i++)
        filterTodos.shift();
    }
    if(!left )
    {
      if(shift<0){
        shift = 0-shift;
        for(var i=0;i<shift;i++)
          filterTodos.pop();
        }      
    }
    filterTodos.filter(item=>{
      result.todos.push(item);
    });
    var count = limits - result.todos.length;
    if (!result.more || count==0)
      return result;
    if (left)
      rowId = rows[0].id;
    else
      rowId = rows[rows.length - 1].id + 1;
    
    return this._gettodotable(limits, rowId, left).then(table=>{
      return this.readLimitedTodo(teamid,table,result,limits,left,rs);
    })
    
  }
  async loadtodo(teamid, limits, lastid, left,rs) {
    var that = this;
    console.log('limits', limits)
    var rs =await that._connecttoscatter().then(result => {
      if (result) {
        var result = {};
        var todos = [];
        result.todos = todos;
        result.more = true;
        var rowId = lastid;
        return that._gettodotable(limits, rowId, left).then(table => {
          return  that.readLimitedTodo(teamid,table,result,limits,left,rs);
        });

      } else
        throw new Error("can't connect to Scatter")

    });
    return rs;
  }

  async addtodo(checker, teamname, title, content, timestamp, bonus) {
    const store = this.store;
    const account = this.store.getAccount();
    let contrat = null;
    var ts = Date.parse(new Date());
    const transactionOptions = { authorization: [`${account.name}@${account.authority}`] };
    const eos = store.getEosClient();
    const orgname = teamname;
    const secret = ts;
    const transferMsg = (Number(bonus) + Number(config.eos.costs.addtodo)).toFixed(4) + " " + config.eos.tokenName;
    const payer = checker;
    const todotitle = title;
    const todocontent = content;
    const tododuetime = timestamp;
    const todobonus = bonus + " " + config.eos.tokenName;

    return await this._connecttoscatter().then((result)=>eos.contract(config.eos.tokenCode).then((contract) => {
      console.log(transferMsg)
      return contract.transfer(account.name, config.eos.contractName, transferMsg, 'addtodo-' + ts).then(() => {
        console.log("transfer is done");
        eos.contract(config.eos.contractName).then((contract) => {
          console.log(account.name, payer, orgname, todotitle, todocontent, tododuetime, todobonus, secret, transactionOptions);
          contract.addtodo(account.name, payer, orgname, todotitle, todocontent, tododuetime, todobonus, secret, transactionOptions).then(data => {

            return true;
          })
        })
      })
    }));
  }
  //status : 1 done 0 delay 2 close
  async updatetodod(todoid, status, bonnus) {
    const store = this.store;
    const account = this.store.getAccount();
    var ts = Date.parse(new Date());
    const transactionOptions = { authorization: [`${account.name}@${account.authority}`] };
    const eos = this.store.getEosClient();
    const secret = ts;
    const transferMsg = bonnus + " " + config.eos.tokenName;
    const id = todoid;

    if (status == 1 || status == 0) {
      if (status == 1) {
        //first transfer bonnus
        return contract.transfer(account.name, config.eos.contractName, transferMsg, 'checktodo-' + ts)
          .then(() => {
            console.log("transfer for checktod is done");
            return eos.contract(config.eos.contractName).then((contract) => {
              return contract.checktodo(account.name, id, 1, secret, transactionOptions).then(data => {
                console.log("this todo has been done" + data);
                return true;
                // this.isImageModalActive = "modal is-active";
              });
            });
          });
      } else {

      }
    }

  }
  async _connecttoscatter() {
    var that = this;
    const result = await ScatterJS.scatter.connect("nsttodo").then(connected => {
      if (connected) {
        that.store.setScatter(ScatterJS.scatter);
        window.scatter = null;
        const eosOptions = { expireInSeconds: 60 };
        var eos = ScatterJS.scatter.eos(config.eos.network, Eos, eosOptions);
        that.store.setEosClient(eos);
        return ScatterJS.scatter.getIdentity({ accounts: [config.eos.network] })
          .then(id => {
            if (!id) {
              // this.isCardModalActive = true;
              return false;
            }
            var account = id.accounts[0];
            that.store.setAccount(account);
            return true
          })

      } else
        return false;
    });
    return result;
  }
  async _getusersbyteamid(teamid) {
    console.log("_getusersbyteamid");
    const eos = this.store.getEosClient();
    const account = this.store.getAccount();
    const tid = teamid;
    console.log('tid', tid);

    const table = await eos.getTableRows({
      scope: config.eos.contractName,
      code: config.eos.contractName,
      table: 'users',
      index_position: 2,
      key_type: 'i64',
      lower_bound: tid,
      json: true
    });
    console.log(table)
    return table.rows;
  }
  async loadteammates(teamid) {
    var that = this;
    // return that._connecttoscatter().then(result => {
    //   if (result) {
    //     return that._getusersbyteamid(teamid);
    //   } else
    //     throw new Error("can't connect to Scatter")

    // });
    var result = await that._connecttoscatter();
      if (result) {
        return that._getusersbyteamid(teamid);
      } else
        throw new Error("can't connect to Scatter")
  }
  // async getorg(eos, orgid) {
  //   const orgtable = await eos.getTableRows({
  //     scope: config.eos.contractName,
  //     code: config.eos.contractName,
  //     table: 'orgs',
  //     json: true,
  //     // table_key:'id',
  //     lower_bound: orgid
  //   });
  //   return orgtable.rows;
  // }

  // async getorgs() {
  //   this.wh.connectScatter().then(result => {
  //     console.log(result)
  //   })

  //  .then(()=>{
  //   const table =  eos.getTableRows({
  //     scope: config.eos.contractName,
  //     code: config.eos.contractName,
  //     table: 'users',
  //     json: true,
  //     //index_position
  //     index_position:3,
  //     key_type:'i64',
  //     lower_bound:account.name
  //   });
  //   var rows = table.rows;
  //   var orgs = new Array();
  //   const that = this;
  //   for(var row of rows){
  //     var orgtable =  that.getorg(eos,row.orgid);
  //     if(orgtable!=null && orgtable.length!=0){
  //       orgs.push(orgtable[0].orgname);
  //     }

  //   }
  //   return orgs

  //  });
  // }
}

// const STORE = require("store");
// var store = new STORE();
// var tm = new teamhandler(store);
// const tmlz = tm.loadtodo("2734096528",2,0,false);
// tmlz.then(data=>{
//   console.log(data);
// })


