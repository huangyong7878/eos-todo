export default class store {
    constructor() {
        this.debug = true,
            this.state = {
                account: null,
                scatter: null,
                eosclient: null
            }
    }
    setAccount(newValue) {
        this.state.account = newValue
    }
    setScatter(newValue) {
        this.state.scatter = newValue
    }
    setEosClient(newValue) {
        this.state.eosclient = newValue
    }
    getEosClient() {
        return this.state.eosclient;
    }
    getScatter() {
        return this.state.scatter
    }
    getAccount() {
        return this.state.account
    }
}
