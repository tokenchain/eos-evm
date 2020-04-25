#include "catch.hpp"
#include <memory>
#include <evm/utils.hpp>
#include <evm/vm.h>
#include <evm/hex.hpp>

#include <evm/call.hpp>
#include <evm/gasometer.hpp>
#include <evm/big_int.hpp>
#include "external_mock.hpp"
#include "test_utils.hpp"

TEST_CASE("Call the code of another contract using CALL", "[message_call]") {
  bytes_t codeBytes = Hex::hexToBytes("60806040526004361061001e5760003560e01c80635c60da1b14610126575b7f2230ca5690d71a74605e39bd522eea4bff3859bc627abd4ac5e473fcb8c5c3b05a6040518082815260200191505060405180910390a16000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff165a34600036604051808383808284378083019250505092505050600060405180830381858888f193505050503d80600081146100e5576040519150601f19603f3d011682016040523d82523d6000602084013e6100ea565b606091505b5050507f2230ca5690d71a74605e39bd522eea4bff3859bc627abd4ac5e473fcb8c5c3b05a6040518082815260200191505060405180910390a1005b34801561013257600080fd5b5061013b61017d565b604051808273ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200191505060405180910390f35b6000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff168156fea265627a7a72315820a640f191ec583fbf9eec25b23042882a593697e120f7cea5230f7784081c042364736f6c63430005100032");
  bytes_t childCodeBytes = Hex::hexToBytes("60806040527fe1f2fff362a4d171d703cc8eb57b9a51dfaa0c2eb2c706aa8ad32011aabf6d0b5a6040518082815260200191505060405180910390a16000604257fe5b00fea265627a7a7231582032423d6c27d3e2eb9e2afa78c5a73bbc112a163dd8f1d29f6a0b05237bfeac9964736f6c63430005100032");
  bytes_t dataBytes = bytes_t();
  env_t env = Utils::env();
  uint256_t address = uint256_t(0xaa0e9a);
  uint256_t codeAddress = uint256_t(0xea0e9b);

  std::shared_ptr<Context> context = std::make_shared<Context>(
    env.chainId,
    env.blockNumber,
    env.timestamp,
    env.gasLimit,
    env.coinbase,
    env.difficulty,
    env.blockHash,
    codeAddress, /* codeAddress */
    uint256_t(0xf9313a), /* codeHash */
    uint256_t(0x193821), /* codeVersion */
    address, /* address */
    uint256_t(0xea0e9e), /* sender */
    uint256_t(0x1283fe), /* origin */
    4600000,
    uint256_t(0),
    uint256_t(1), /* value */
    std::make_shared<bytes_t>(codeBytes),
    std::make_shared<bytes_t>(dataBytes)
  );

  std::shared_ptr<ExternalMock> external = std::make_shared<ExternalMock>();
  std::shared_ptr<std::vector<uint256_t>> stackItems = std::make_shared<std::vector<uint256_t>>();
  std::shared_ptr<StackMachine> stack = std::make_shared<StackMachine>(stackItems);
  std::shared_ptr<Gasometer> gasometer = std::make_shared<Gasometer>(context->gas);
  VM vm(stack, gasometer);
  std::shared_ptr<PendingState> pendingState = std::make_shared<PendingState>();
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> mem = std::make_shared<Memory>(memoryBytes);
  Operation operation = Operation();

  uint256_t childContractAddress = uint256_t(0x1283ae);
  pendingState->putState(uint256_t(0), childContractAddress, codeAddress);
  external->codeResponder.push_back(std::make_pair(childContractAddress, childCodeBytes));
  external->balanceResponder.push_back(std::make_pair(address, 100));

  // when
  exec_result_t vm_result = vm.execute(0, operation, context, mem, pendingState, external);

  // then
  REQUIRE(ExecResult::DONE_VOID == vm_result.first);

  CHECK(2 == pendingState->logs.size());

  CHECK(1 == pendingState->logs[0].topics.size());
  CHECK("0000000000000000000000000000000000000000000000000000000000463093" ==
    Hex::bytesToHex(pendingState->logs[0].data)
  );

  CHECK(1 == pendingState->logs[1].topics.size());
  CHECK("00000000000000000000000000000000000000000000000000000000000117e6" ==
    Hex::bytesToHex(pendingState->logs[1].data)
  );
}