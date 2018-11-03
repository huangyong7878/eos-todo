import ScatterJS from "scatter-js/dist/scatter.esm";
const config = require('../config');
import Eos from "eosjs";
export default class wallethandler {
    constructor(store) {
        this.store = store;
    }
    async connecttoscatter() {
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
}
