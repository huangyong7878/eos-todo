import Vue from 'vue'
import Router from 'vue-router'
import start from '@/components/start'
import exchangeComp from '@/components/exchanges' 
import teams from '@/components/teams'
import addteam from '@/components/addteam'
import jointeam from '@/components/jointeam'
import board from '@/components/board'
import rates from '@/components/rates'
import transfer from '@/components/transfer'
Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/start',
      name: 'start',
      component: start
    },
    {
      path:'/exchange',
      name: 'exchange',
      component:exchangeComp
    },
    {
      path:'/teams/:id/name/:orgname',
      name:'teams',
      component:teams
    },
    {
      path:'/addteam',
      name:'addteam',
      component:addteam
    },
    {
      path:'/jointeam',
      name:'jointeam',
      component:jointeam
    },
    {
      path:'/board',
      name:'board',
      component:board
    },
    {
      path:'/rates',
      name:'rates',
      component:rates
    },
    {
      path:'/transfer',
      name:'transfer',
      component:transfer
    }

  ]
})
