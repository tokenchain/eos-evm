#include "catch.hpp"
#include <evm/utils.h>
#include <evm/vm.h>
#include <evm/hex.h>
#include <evm/return_data.h>
#include <evm/call.h>
#include <evm/gasometer.h>
#include "external_mock.h"

TEST_CASE("Log empty (LOG0)", "[log]") {
  // given
  std::string bytecode_str = "60006000a0";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  exec_result_t result = vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK(99619 == Utils::gasLeft(result));

  // then
  CHECK(1 == ext.logSpy.size());
  CHECK(0 == ext.logSpy[0].first.size());
  CHECK(0 == ext.logSpy[0].second.size());
}

TEST_CASE("Log sender (LOG1)", "[log]") {
  // given
  std::string bytecode_str = "60ff6000533360206000a1";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  exec_result_t result = vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK(98974 == Utils::gasLeft(result));

  // then
  CHECK(1 == ext.logSpy.size());
  CHECK(1 == ext.logSpy[0].first.size());
  CHECK("0000000000000000000000000000000000000000000000000000000000ea0e9e" == 
    Utils::uint256_2str(ext.logSpy[0].first[0])
  );
  CHECK("ff00000000000000000000000000000000000000000000000000000000000000" == 
    Hex::bytesToHex(ext.logSpy[0].second)
  );
}

TEST_CASE("Log origin and sender (LOG2)", "[log]") {
  // given
  std::string bytecode_str = "60ff600053333260206000a2";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  exec_result_t result = vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK(98977 == Utils::gasLeft(result));

  // then
  CHECK(1 == ext.logSpy.size());
  CHECK(2 == ext.logSpy[0].first.size());
  CHECK("00000000000000000000000000000000000000000000000000000000001283fe" == 
    Utils::uint256_2str(ext.logSpy[0].first[0])
  );
  CHECK("0000000000000000000000000000000000000000000000000000000000ea0e9e" == 
    Utils::uint256_2str(ext.logSpy[0].first[1])
  );
  CHECK("ff00000000000000000000000000000000000000000000000000000000000000" == 
    Hex::bytesToHex(ext.logSpy[0].second)
  );
}

TEST_CASE("Log caller, origin, sender (LOG3)", "[log]") {
  // given
  std::string bytecode_str = "60ff60005333323060206000a3";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  exec_result_t result = vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK(98977 == Utils::gasLeft(result));

  // then
  CHECK(1 == ext.logSpy.size());
  CHECK(3 == ext.logSpy[0].first.size());
  CHECK("0000000000000000000000000000000000000000000000000000000000ea0e9a" == 
    Utils::uint256_2str(ext.logSpy[0].first[0])
  );
  CHECK("00000000000000000000000000000000000000000000000000000000001283fe" == 
    Utils::uint256_2str(ext.logSpy[0].first[1])
  );
  CHECK("0000000000000000000000000000000000000000000000000000000000ea0e9e" == 
    Utils::uint256_2str(ext.logSpy[0].first[2])
  );
  CHECK("ff00000000000000000000000000000000000000000000000000000000000000" == 
    Hex::bytesToHex(ext.logSpy[0].second)
  );
}

TEST_CASE("Log number, caller, origin and sender (LOG4)", "[log]") {
  // given
  std::string bytecode_str = "60ff6000534333323060206000a4";
  params_t params =  Utils::params(Hex::hexToBytes(bytecode_str), bytes_t());
  ExternalMock ext {};
  VM vm(params);
  Call call(0);
  AccountState accountState(&ext);
  Memory mem {};
  env_t env = Utils::env();

  // when
  exec_result_t result = vm.execute(mem, accountState, ext, call, env);

  // then
  CHECK(79935 == Utils::gasLeft(result));

  // then
  CHECK(1 == ext.logSpy.size());
  CHECK(4 == ext.logSpy[0].first.size());
  CHECK("0000000000000000000000000000000000000000000000000000000000ea0e9a" == 
    Utils::uint256_2str(ext.logSpy[0].first[0])
  );
  CHECK("00000000000000000000000000000000000000000000000000000000001283fe" == 
    Utils::uint256_2str(ext.logSpy[0].first[1])
  );
  CHECK("0000000000000000000000000000000000000000000000000000000000ea0e9e" == 
    Utils::uint256_2str(ext.logSpy[0].first[2])
  );
  CHECK("0000000000000000000000000000000000000000000000000000000000f950e1" == 
    Utils::uint256_2str(ext.logSpy[0].first[3])
  );
  CHECK("ff00000000000000000000000000000000000000000000000000000000000000" == 
    Hex::bytesToHex(ext.logSpy[0].second)
  );
}