#include "catch.hpp"
#include <memory>
#include <evm/utils.h>
#include <evm/vm.h>
#include <evm/hex.h>
#include <evm/return_data.h>
#include <evm/call.h>
#include <evm/gasometer.h>
#include "external_mock.h"

TEST_CASE("Call contract code", "[call]") {
  std::string bytecode_str = "608060405234801561001057600080fd5b5060405161001d9061007e565b604051809103906000f080158015610039573d6000803e3d6000fd5b506000806101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff16021790555061008a565b60978061027083390190565b6101d7806100996000396000f3fe60806040526004361061001e5760003560e01c80635c60da1b14610126575b7f2230ca5690d71a74605e39bd522eea4bff3859bc627abd4ac5e473fcb8c5c3b05a6040518082815260200191505060405180910390a16000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff165a34600036604051808383808284378083019250505092505050600060405180830381858888f193505050503d80600081146100e5576040519150601f19603f3d011682016040523d82523d6000602084013e6100ea565b606091505b5050507f2230ca5690d71a74605e39bd522eea4bff3859bc627abd4ac5e473fcb8c5c3b05a6040518082815260200191505060405180910390a1005b34801561013257600080fd5b5061013b61017d565b604051808273ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200191505060405180910390f35b6000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff168156fea265627a7a723158207e4cffa921fb616e3caaba571cbae9f0921d689f6907c32716d57a11edd79a3a64736f6c634300051000326080604052348015600f57600080fd5b5060798061001e6000396000f3fe60806040527fe1f2fff362a4d171d703cc8eb57b9a51dfaa0c2eb2c706aa8ad32011aabf6d0b5a6040518082815260200191505060405180910390a16000604257fe5b00fea265627a7a72315820b1bb98ed58a6a38794448e22218e5747af62c2899d3c369514b32aa65007658c64736f6c63430005100032";
  bytes_t codeBytes = Hex::hexToBytes(bytecode_str);
  bytes_t emptyBytes = bytes_t();
  params_t params =  Utils::params(codeBytes, emptyBytes);
  env_t env = Utils::env();
  std::shared_ptr<Context> context = std::make_shared<Context>(
    env.chainId,
    env.blockNumber,
    env.timestamp,
    env.gasLimit,
    env.coinbase,
    env.difficulty,
    env.blockHash,
    params.address,
    params.codeHash,
    params.codeVersion,
    params.address,
    params.sender,
    params.origin,
    params.gas,
    params.gasPrice,
    params.value,
    std::make_shared<bytes_t>(params.code),
    std::make_shared<bytes_t>(params.data)
  );

  std::shared_ptr<ExternalMock> external = std::make_shared<ExternalMock>();
  std::shared_ptr<std::vector<uint256_t>> stackItems = std::make_shared<std::vector<uint256_t>>();
  std::shared_ptr<StackMachine> stack = std::make_shared<StackMachine>(stackItems);
  VM vm(context, stack);

  std::shared_ptr<Call> call = std::make_shared<Call>(0);
  std::shared_ptr<AccountState> accountState = std::make_shared<AccountState>(external);
  std::shared_ptr<Memory> mem = std::make_shared<Memory>();

  // when
  exec_result_t vm_result = vm.execute(mem, accountState, external, call);

  // then
  REQUIRE(ExecResult::DONE == vm_result.first);

  // and then
  gas_left_t gasLeft = std::get<gas_left_t>(vm_result.second);

  REQUIRE(GasType::NEEDS_RETURN == gasLeft.first);

  // and then
  NeedsReturn needsReturn = std::get<NeedsReturn>(gasLeft.second);

  CHECK("" == 
    Hex::bytesToHex(needsReturn.data.mem)
  );
}