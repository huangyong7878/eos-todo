#include <algorithm>
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include "eosio.token.hpp"
#include "utils.hpp"
#include "types.hpp"
#include <string>

namespace nineseals
{
using namespace eosio;
using namespace std;

class Todo : public contract
{
  using contract::contract;

  static const account_name code_account = N(todo);
  static const int nst_rate = 100.0000;

public:
  Todo(account_name self) : contract(self), _orgs(_self, _self), _refs(_self, _self), _users(_self, _self),_todos(_self,_self) {}
  void exchange(const account_name &from, const account_name &to, const asset &quantity, const string &memo);

  void deposit(const account_name &from, const account_name &to, const asset &quantity, const string &memo);
  //@abi action
  void createorg(account_name orgowner,string& nickname, string &orgname, string &secret);

  //@abi action
  void jointeam(account_name who, string &nickname, string &orgname, string &secret);
  //@abi action
  void addtodo(account_name creator, account_name checker, string& orgname, string &title, string &content, uint64_t expiration,  asset& bonus,string &secret);
  //@abi action
  void removeorg(account_name orgowner, string &orgname);
   //@abi action
  void checktodo(account_name checker, uint64_t todoid,uint64_t r,string &secret);
  //@abi action
  void closetodo(account_name who,uint64_t todoid);

private:
  tb_orgs _orgs;
  tb_refs _refs;
  tb_users _users;
  tb_todos _todos;
  // struct org{
  //     uint64_t id;
  //     string orgname;
  //     account_name orgowner;
  //     uint64_t primary_key() const { return id; }

  //     EOSLIB_SERIALIZE(org, (orgname)(orgowner))
  // };
  // typedef multi_index<N(org), org> orgindex;
  //actiontype-randomref
  int parsememo(string memo, string *actiontype, string *random)
  {
    //erase any space
    memo.erase(std::remove_if(memo.begin(),
                              memo.end(),
                              [](unsigned char x) { return std::isspace(x); }),
               memo.end());
    size_t sep_count = std::count(memo.begin(), memo.end(), '-');
    //  print("memo:",memo);
    // print("count:",sep_count);
    // eosio_assert(sep_count == 1, "invalid memo");
    if(sep_count!=1)
      return 0;
    size_t pos;
    string container;
    pos = sub2sep(memo, &container, '-', 0, true);
    eosio_assert(!container.empty(), "no actiontype");
    *actiontype = container;
    print("action:", *actiontype);
    container = memo.substr(++pos);
    *random = container;
    return 1;
    //print("raandom:",*random);
  }

  //calculate how much NST we need send back to payer
  asset cal_fee_rollback(const asset &quantity)
  {
    asset rollbackfee = (quantity < NST_COST_EXCHANGE) ? NST_COST_ZERO : (quantity - NST_COST_EXCHANGE);
    return rollbackfee;
  }
  template <typename... Args>
  void send_defer_action(uint64_t id, Args &&... args)
  {
    transaction trx;
    trx.actions.emplace_back(std::forward<Args>(args)...);
    trx.send(id, _self, false);
  }

  //cal ref base on creator name and a random secret
  uint64_t calref(account_name name, string &random)
  {
    checksum256 result;
    string temp = uint64_string(name) + random;
    const char *tempchars = temp.c_str();
    sha256(tempchars, strlen(tempchars), &result);
    uint64_t ref = uint64_hash(result);
    return ref;
  }

  //calculate org id base one orgname
  uint64_t calorgid(string &orgname)
  {
    checksum256 result;
    string temp = orgname;
    const char *tempchars = temp.c_str();
    sha256(tempchars, strlen(tempchars), &result);
    uint64_t ref = uint64_hash(result);
    return ref;
  }

  //find if ref exist
  todo_ref find_or_error_ref(const uint64_t &id)
  {
    auto itr = _refs.find(id);
    eosio_assert(itr != _refs.end(), "ref not found");
    return *itr;
  }

  //set ref.result as completed
  void r_ref(const uint64_t &id)
  {
    auto itr = _refs.find(id);
    eosio_assert(itr != _refs.end(), "ref not found");
    _refs.modify(itr, 0, [&](auto& ref) {
      ref.r = 1;
      print("r is 0 now");
    });
    
  }

  //check if  the org is exist
  void checkdup_org(const uint64_t &id)
  {
    auto itr = _orgs.find(id);
    eosio_assert(itr == _orgs.end(), "org has exist");
  }

  uint64_t getactiontype(const string &typestr)
  {
    if (typestr == ACTION_CREATE_ORG)
      return NST_CREATE_ORG;
    if(typestr == ACTION_JOIN_TEAM)
      return NST_JOIN_TEAM;
    
    if(typestr == ACTION_ADD_TODO)
      return NST_ADD_TODO;
    
    if(typestr == ACTION_CHECK_TODO)
      return NST_CHECK_TODO;

    return NST_UNKOWN;
  }
};

//  EOSIO_ABI(Todo, (createorg)(getorg));

extern "C"
{
  void apply(uint64_t receiver, uint64_t code, uint64_t action)
  {
    Todo thiscontract(receiver);

    if ((code == N(eosio.token)) && (action == N(transfer)))
    {
      execute_action(&thiscontract, &Todo::exchange);
      return;
    }

    print("action",action);
    print("N(transfer)",N(transfer));
    //parse nstoken
    if (code == NST_CONTRACT && (action == N(transfer)))
    {

      execute_action(&thiscontract, &Todo::deposit);
      return;
    }
    if (code != receiver)
      return;

    switch (action)
    {
      EOSIO_API(Todo, (createorg)(deposit)(jointeam)(addtodo)(removeorg)(closetodo)(checktodo))
    };
    eosio_exit(0);
  }
}

} // namespace nineseals
