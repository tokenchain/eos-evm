#include "byte_reader.h"
#include "stack.h"
#include <evm/types.h>
#include "account_state.h"
#include "vm_result.h"
#include <evm/memory.h>
#include <evm/external.h>
#include <evm/gasometer.h>

class VM {
  public:
    VM();
  private:
    uint8_t last_stack_ret_len;
    ReturnData returnData;

    exec_result_t step(
      jump_set_t& jumps, 
      Memory& memory,
      StackMachine& stack,
      ByteReader& reader, 
      AccountState& accountState,
      Gasometer& gasometer,
      params_t& params,
      External& external,
      Call& call,
      env_t env
    );

    exec_result_t stepInner(
      jump_set_t& jumps, 
      Memory& memory,
      StackMachine& stack,
      ByteReader& reader, 
      AccountState& accountState,
      Gasometer& gasometer,
      params_t& params,
      External& external,
      Call& call,
      env_t env
    );

    instruction_result_t executeInstruction(
      gas_t gas,
      gas_t providedGas,
      instruct_t instruction, 
      Memory& memory,
      StackMachine& stack,
      ByteReader& reader, 
      AccountState& accountState,
      params_t& params,
      External& external,
      Call& call,
      env_t env
    );

  public:
    exec_result_t execute(
      Memory& memory,
      StackMachine& stack, 
      AccountState& accountState,
      Gasometer& gasometer,
      params_t& params,
      External& external,
      Call& call,
      env_t env
    );
};