import Vue from 'vue'
import App from './App'
import router from './router'
import Eos from 'eosjs';
import ScatterJS from 'scatter-js/dist/scatter.esm';
import Buefy from 'buefy'
import 'buefy/dist/buefy.css'
import VueTimers from 'vue-timers'
Vue.use(Buefy,{
  defaultIconPack: 'fa',
});
Vue.use(VueTimers)
const config = require("./config");
Vue.config.productionTip = false;
import store from './libs/store'

Vue.prototype.$store = new store(); 
import teamhandler from './libs/teamhandler'
var th = new teamhandler(Vue.prototype.$store);
Vue.prototype.$th = th;
new Vue({
  el: '#app',
  router,

  render: h => h(App)
})
