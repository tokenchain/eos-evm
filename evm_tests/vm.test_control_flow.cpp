#include "catch.hpp"
#include <memory>
#include <evm/utils.hpp>
#include <evm/vm.h>
#include <evm/hex.hpp>
#include <evm/return_data.h>
#include <evm/call.h>
#include <evm/gasometer.hpp>
#include <evm/big_int.hpp>
#include "external_mock.hpp"

TEST_CASE("Conditional jump to destination truthy", "[jumps]") {
  // given
  // (PUSH1 ((60)06))
	// (PUSH1 ((60)03))
  // (LT (10))
  // (PUSH1 ((60)0F))
  // (JUMPI (57))
  // (PUSH1 ((60)06))
	// (PUSH1 ((60)03))
	// (MUL (02))
  // (STOP (00))
  // (JUMPDEST (5B))
  // (PUSH1 ((60)02))
  // (PUSH1 ((60)10))
	// (DIV (04))
  std::string bytecode_str = "6006600310600F57600660030200025B6002601604";
  bytes_t codeBytes = Hex::hexToBytes(bytecode_str);
  env_t env = Utils::env();
  std::shared_ptr<Context> context = std::make_shared<Context>(
    env.chainId,
    env.blockNumber,
    env.timestamp,
    env.gasLimit,
    env.coinbase,
    env.difficulty,
    env.blockHash,
    uint256_t(0xea0e9a), /* address */
    uint256_t(0xf9313a), /* codeHash */
    uint256_t(0x193821), /* codeVersion */
    uint256_t(0xea0e9a), /* address */
    BigInt::fromHex("cd1722f3947def4cf144679da39c4c32bdc35681"),
    uint256_t(0x1283fe), /* origin */
    100000,
    uint256_t(0),
    uint256_t(0),
    std::make_shared<bytes_t>(codeBytes),
    std::make_shared<bytes_t>()
  );

  std::shared_ptr<ExternalMock> external = std::make_shared<ExternalMock>();
  std::shared_ptr<std::vector<uint256_t>> stackItems = std::make_shared<std::vector<uint256_t>>();
  std::shared_ptr<StackMachine> stack = std::make_shared<StackMachine>(stackItems);
  std::shared_ptr<Gasometer> gasometer = std::make_shared<Gasometer>(context->gas);
  VM vm(stack, gasometer);


  std::shared_ptr<Call> call = std::make_shared<Call>(0);
  std::shared_ptr<account_store_t> cacheItems = std::make_shared<account_store_t>();
  std::shared_ptr<AccountState> accountState = std::make_shared<AccountState>(cacheItems);
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> mem = std::make_shared<Memory>(memoryBytes);
  Operation operation = Operation();

  // when
  vm.execute(operation, context, mem, accountState, external, call);

  // then
  CHECK("000000000000000000000000000000000000000000000000000000000000000b" == 
    Utils::uint256_2str(vm.stack->top())
  );
}

TEST_CASE("Conditional jump to destination not true", "[jumps]") {
  // given
  // (PUSH1 ((60)03))
	// (PUSH1 ((60)06))
  // (LT (10))
  // (PUSH1 ((60)0F))
  // (JUMPI (57))
  // (PUSH1 ((60)06))
	// (PUSH1 ((60)03))
	// (MUL (02))
  // (STOP (00))
  // (JUMPDEST (5B))
  // (PUSH1 ((60)02))
  // (PUSH1 ((60)10))
	// (DIV (04))
  std::string bytecode_str = "6003600610600F57600660030200025B6002601604";
  bytes_t codeBytes = Hex::hexToBytes(bytecode_str);
  env_t env = Utils::env();
  std::shared_ptr<Context> context = std::make_shared<Context>(
    env.chainId,
    env.blockNumber,
    env.timestamp,
    env.gasLimit,
    env.coinbase,
    env.difficulty,
    env.blockHash,
    uint256_t(0xea0e9a), /* address */
    uint256_t(0xf9313a), /* codeHash */
    uint256_t(0x193821), /* codeVersion */
    uint256_t(0xea0e9a), /* address */
    BigInt::fromHex("cd1722f3947def4cf144679da39c4c32bdc35681"),
    uint256_t(0x1283fe), /* origin */
    100000,
    uint256_t(0),
    uint256_t(0),
    std::make_shared<bytes_t>(codeBytes),
    std::make_shared<bytes_t>()
  );

  std::shared_ptr<ExternalMock> external = std::make_shared<ExternalMock>();
  std::shared_ptr<std::vector<uint256_t>> stackItems = std::make_shared<std::vector<uint256_t>>();
  std::shared_ptr<StackMachine> stack = std::make_shared<StackMachine>(stackItems);
  std::shared_ptr<Gasometer> gasometer = std::make_shared<Gasometer>(context->gas);
  VM vm(stack, gasometer);


  std::shared_ptr<Call> call = std::make_shared<Call>(0);
  std::shared_ptr<account_store_t> cacheItems = std::make_shared<account_store_t>();
  std::shared_ptr<AccountState> accountState = std::make_shared<AccountState>(cacheItems);
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> mem = std::make_shared<Memory>(memoryBytes);
  Operation operation = Operation();

  // when
  vm.execute(operation, context, mem, accountState, external, call);

  // then
  CHECK("0000000000000000000000000000000000000000000000000000000000000012" == 
    Utils::uint256_2str(vm.stack->top())
  );
}

TEST_CASE("Unconditional jump to destination", "[jumps]") {
  // given
  // (PUSH1 ((60)03))
	// (PUSH1 ((60)06))
  // (PUSH1 ((60)08))
  // (JUMP (56))
	// (MUL (02))
  // (JUMPDEST (5B))
	// (DIV (04))
  std::string bytecode_str = "60036006600856025B04";
  bytes_t codeBytes = Hex::hexToBytes(bytecode_str);
  env_t env = Utils::env();
  std::shared_ptr<Context> context = std::make_shared<Context>(
    env.chainId,
    env.blockNumber,
    env.timestamp,
    env.gasLimit,
    env.coinbase,
    env.difficulty,
    env.blockHash,
    uint256_t(0xea0e9a), /* address */
    uint256_t(0xf9313a), /* codeHash */
    uint256_t(0x193821), /* codeVersion */
    uint256_t(0xea0e9a), /* address */
    BigInt::fromHex("cd1722f3947def4cf144679da39c4c32bdc35681"),
    uint256_t(0x1283fe), /* origin */
    100000,
    uint256_t(0),
    uint256_t(0),
    std::make_shared<bytes_t>(codeBytes),
    std::make_shared<bytes_t>()
  );

  std::shared_ptr<ExternalMock> external = std::make_shared<ExternalMock>();
  std::shared_ptr<std::vector<uint256_t>> stackItems = std::make_shared<std::vector<uint256_t>>();
  std::shared_ptr<StackMachine> stack = std::make_shared<StackMachine>(stackItems);
  std::shared_ptr<Gasometer> gasometer = std::make_shared<Gasometer>(context->gas);
  VM vm(stack, gasometer);


  std::shared_ptr<Call> call = std::make_shared<Call>(0);
  std::shared_ptr<account_store_t> cacheItems = std::make_shared<account_store_t>();
  std::shared_ptr<AccountState> accountState = std::make_shared<AccountState>(cacheItems);
  std::shared_ptr<bytes_t> memoryBytes = std::make_shared<bytes_t>();
  std::shared_ptr<Memory> mem = std::make_shared<Memory>(memoryBytes);
  Operation operation = Operation();

  // when
  vm.execute(operation, context, mem, accountState, external, call);

  // then
  CHECK("0000000000000000000000000000000000000000000000000000000000000002" == 
    Utils::uint256_2str(vm.stack->top())
  );
}