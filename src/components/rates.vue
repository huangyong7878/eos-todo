<template>
    <b-table :data="prices" :columns="columns"></b-table>
</template>

<script>
const config = require("../config.js");
export default {
     name: "rates",
    data(){
        return {
            prices:[
                {'id':1,'item':'create team','cost':config.eos.costs.creatorg+'NST','desccription':'the costs for create a new team'},
                {'id':2,'item':'join team','cost':config.eos.costs.joinorg+'NST','desccription':'the costs for join an existteam'},
                {'id':3,'item':'create a todo','cost':config.eos.costs.addtodo+'NST','desccription':'the costs for add a todo item'},    
            ],
            columns:[
                {
                    field: 'id',
                    label: 'ID',
                    width: '40',
                    numeric: true
                },
                {
                    field:'item',
                    label:'Item',
                },
                 {
                    field:'cost',
                    label:'Cost',
                },
                 {
                    field:'desccription',
                    label:'Desccription',
                }
            ]
        }
    }
}
</script>
