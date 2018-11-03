#include "todo.hpp"

namespace nineseals
{

//anytime when someone transfer NST to this account
void Todo::deposit(const account_name &from, const account_name &to, const asset &quantity, const string &memo)
{
    if (to != _self)
        return;
    eosio_assert(quantity.is_valid(), "invalid quantity");
    eosio_assert(quantity.amount > 0, "must deposit positive quantity");
    string actiontype;
    string random;
    int v = parsememo(memo, &actiontype, &random);
    if(v==0)
        return;

    //calculate ref base one random,and from
    uint64_t ref_id = calref(from, random);
    print("ref:", ref_id);

    uint64_t ntype = getactiontype(actiontype);
    print("ntype:", ntype);
    print("NST_CREATE_ORG:", NST_CREATE_ORG);
    print("NST_JOIN_TEAM:", NST_JOIN_TEAM);
    print("quantity", quantity);
    asset cost = NST_COST_ZERO;
    char rollback = 0;
    //calculate the platform fee
    switch (ntype)
    {
    case NST_CREATE_ORG:
        print("NST_COST_CREATE_ORG112");
        print("NST_COST_CREATE_ORG:", NST_COST_CREATE_ORG);
        if (quantity < NST_COST_CREATE_ORG)
            rollback = 1;
        else
            cost =  NST_COST_CREATE_ORG;
        break;
    case NST_JOIN_TEAM:
        if (quantity < NST_COST_JOIN_TEAM)
            rollback = 1;
        else
            cost =  NST_COST_JOIN_TEAM;
        break;
    case NST_ADD_TODO:
        if (quantity < NST_COST_ADD_TODO)
            rollback = 1;
        else
            cost = NST_COST_ADD_TODO;
        break;   
    }
    print("rollback", rollback);
    if (rollback)
    {

        auto rollbackfee = cal_fee_rollback(quantity);

        if (rollbackfee.amount > 0)
        {
            print("rollback now:", rollbackfee);
            auto t = std::make_tuple(_self, from, rollbackfee, std::string("todorb"));
            print("message:", std::get<0>(t), ",", std::get<1>(t), ",", std::get<2>(t), ",", std::get<3>(t));
            action(permission_level{_self, N(active)}, N(nsealstokens), N(transfer), t).send();
        }

        return;
    }
    asset realpay = quantity - cost;
    print("real pay(cut exchange cast",realpay);
    _refs.emplace(_self, [&](auto &ref) {
        ref.ref = ref_id;
        ref.type = ntype;
        ref.payer = from;
        ref.pay = realpay;
        ref.r = 0;
    });
}
/**
     * 
     * anyone transfer 1EOS,then we need to transfer rate NST to from
     * 
     **/

void Todo::exchange(const account_name &from, const account_name &to, const asset &quantity, const string &memo)
{
    string prefix("exchange");
    if (!memo.compare(0, prefix.size(), prefix))
    {
        print("this is just a pure exchange,we just give eos as rate\n");
        asset payout = asset(quantity.amount * Todo::nst_rate, NST_SYMBOL);
        print("payout", payout);
        auto t = std::make_tuple(_self, from, payout, std::string("todoexchange"));
        print("message:", std::get<0>(t), ",", std::get<1>(t), ",", std::get<2>(t), ",", std::get<3>(t));
        action(permission_level{_self, N(active)},
               N(nsealstokens),
               N(transfer),
               t)
            .send();
    }
}

/**
     * 
     * 
     * 
     **/
void Todo::createorg(account_name orgowner, string &nickname, string &orgname, string &secret)
{
    require_auth(orgowner);
    uint64_t ref_id = calref(orgowner, secret);

    todo_ref ref = find_or_error_ref(ref_id);
    eosio_assert(ref.r == 0, "this ref has been used");
    // eosio_assert(ref.type=="")

    uint64_t org_id = calorgid(orgname);
    //first,we can find if this name is duplicated
    checkdup_org(org_id);
    //start to create org
    _orgs.emplace(_self, [&](auto &org) {
        org.orgname = orgname;
        org.creator = orgowner;
        // org.members = map<account_name,string>();
        org.id = org_id;
    });

    _users.emplace(_self, [&](auto &user) {
        user.orgid = org_id;
        user.useraccount = orgowner;
        user.teamname = orgname;
        user.nickname = nickname;
        user.id = _users.available_primary_key();
    });
    r_ref(ref_id);
}

//@abi action
void Todo::jointeam(account_name who, string &nickname, string &orgname, string &secret)
{
    require_auth(who);
    uint64_t ref_id = calref(who, secret);

    todo_ref ref = find_or_error_ref(ref_id);
    eosio_assert(ref.r == 0, "this ref has been used");
    eosio_assert(ref.type == NST_JOIN_TEAM, "not a join team action");
    eosio_assert(ref.payer == who, "not the same user");
    

    uint64_t org_id = calorgid(orgname);
    auto itr = _orgs.find(org_id);
    eosio_assert(itr != _orgs.end(), "this org is not exist");
    todo_org org = *itr;

    auto index = _users.get_index<N(by_org)>();
    auto user_itor = index.find(org_id);
    for (user_itor = index.begin(); user_itor != index.end(); user_itor++)
    {
        auto user = *user_itor;
        eosio_assert(!(user.nickname == nickname),"nickname is duplicated");
    }
    _users.emplace(_self, [&](auto &user) {
        user.orgid = org_id;
        user.useraccount = who;
        user.nickname = nickname;
        user.teamname = orgname;
        user.id = _users.available_primary_key();
    });

    r_ref(ref_id);
}
/**
 * before invoie it, todoowner need deposite cost 
 * 
 **/
//@abi action
void Todo::addtodo(account_name todoowner, account_name checker, string& orgname,string &title, string &content, uint64_t expiration, asset& bonus, string &secret)
{
    
    require_auth(todoowner);
    
    uint64_t ref_id = calref(todoowner, secret);
    todo_ref ref = find_or_error_ref(ref_id);
    eosio_assert(ref.r == 0, "this ref has been used");
    eosio_assert(ref.type == NST_ADD_TODO, "not a create todo action");
     eosio_assert(ref.payer == todoowner, "not the same user");
     eosio_assert(ref.pay>=bonus,"the asset is not enough");

    int64_t org_id = calorgid(orgname);
    auto itr = _orgs.find(org_id);
    eosio_assert(itr != _orgs.end(), "this org is not exist");
    
    _todos.emplace(_self,[&] (auto &item){
        item.id = _todos.available_primary_key();
        item.orgid = org_id;
        item.owner = todoowner ;
        item.payer = checker;
        item.title = title;
        item.content = content;
        item.endtime = expiration;
        item.ownerpay = ref.pay ;
        item.bonus = bonus;
        item.r = 0;
    });
    r_ref(ref_id);
}

/**
 * 
 * 
 *
 **/ 
//abi action
void Todo::checktodo(account_name checker, uint64_t todoid,uint64_t r,string &secret)
{
    require_auth(checker);
    print("secret",secret);
    print("r",r);
    uint64_t ref_id  = -1;
    auto itr = _todos.find(todoid);
    eosio_assert(itr != _todos.end(), "todo not found");
    todo_item item = *itr;
    eosio_assert(item.payer==checker,"the checker is not the payer");
    asset payback = item.ownerpay;
    if(r == 1){
        ref_id = calref(checker, secret);
        print("received ref_id",ref_id);
        todo_ref ref = find_or_error_ref(ref_id);
        eosio_assert(ref.r == 0, "this ref has been used");
        eosio_assert(ref.type == NST_CHECK_TODO, "not a check todo action");
        eosio_assert(ref.payer == checker, "not the same user");
        eosio_assert(ref.pay>=item.bonus,"the asset is not enough");
        payback += ref.pay;
    }
    //transfer the bonus plus 
    auto t = std::make_tuple(_self, r==1?item.owner:item.payer, payback, std::string("todobonus"));        
    print("message:", std::get<0>(t), ",", std::get<1>(t), ",", std::get<2>(t), ",", std::get<3>(t));
    action(permission_level{_self, N(active)}, N(nsealstokens), N(transfer), t).send();

    _todos.modify(itr, 0, [&](auto& item) {
      item.r = r==1?1:2;      
    });

    if(r == 1)
        r_ref(ref_id);

     
    

}
//abi action
void Todo::closetodo(account_name who,uint64_t todoid){
     require_auth(who);
     auto itr = _todos.find(todoid);
    eosio_assert(itr != _todos.end(), "todo not found");

    todo_item item = *itr;
    eosio_assert(item.owner==who,"you are not the owner");

     _todos.modify(itr, 0, [&](auto& item) {
      item.r = 3;      
    });
    
}
//abi action
void Todo::removeorg(account_name orgowner, string &orgname)
{
    
}

} // namespace nineseals