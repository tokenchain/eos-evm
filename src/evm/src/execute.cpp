#include <set>
#include <tuple>
#include <evm/execute.h>
#include <evm/vm.h>
#include <evm/external.h>
#include <evm/hex.h>

finalization_result_t Execute::callWithStackDepth(
  params_t& params,
  size_t stackDepth,
  External& external,
  AccountState& accountState,
  env_t& env,
  Call& call
  /* tracer */
  /* vm_tracer */
) {

  VM vm(params);

  Memory mem {};

  exec_result_t vm_result = vm.execute(
    mem, 
    accountState, 
    external, 
    call, 
    env
  );

  switch (vm_result.first) {
    case STOPPED:
      {
        Finalization finalization {
          uint256_t(0),
          false,
          ReturnData::empty()
        };

        return std::make_pair(
          FinalizationResult::FINALIZATION_OK,
          finalization
        );
      }
    case DONE:
      {
        gas_left_t gasLeft = std::get<gas_left_t>(vm_result.second);
        switch (gasLeft.first) {
          case KNOWN:
            {
              uint256_t gasLeftValue = std::get<uint256_t>(gasLeft.second); 

              Finalization finalization {
                gasLeftValue,
                true,
                ReturnData::empty()
              };

              return std::make_pair(
                FinalizationResult::FINALIZATION_OK,
                finalization
              );
            }
          case NEEDS_RETURN:
            {
              NeedsReturn needsReturn = std::get<NeedsReturn>(gasLeft.second);

              Finalization finalization {
                needsReturn.gasLeft,
                needsReturn.apply,
                needsReturn.data
              };
   
              return std::make_pair(
                FinalizationResult::FINALIZATION_OK,
                finalization
              );
            }
        }
      }
    case VM_TRAP:
      return std::make_pair(
        FinalizationResult::FINALIZATION_ERROR,
        0
      );
    case VM_OUT_OF_GAS:
      return std::make_pair(
        FinalizationResult::FINALIZATION_OUT_OF_GAS,
        0
      );
    case TRACE:
      {
        uint8_t position = std::get<uint8_t>(vm_result.second);
        return std::make_pair(
          FinalizationResult::FINALIZATION_TRACE,
          position
        );
      }
    case CONTINUE:
      // In reality this option is never reached
      return std::make_pair(
        FinalizationResult::FINALIZATION_ERROR,
        0
      );
  };
}