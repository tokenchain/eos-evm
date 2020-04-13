#include <vector>
#include <evm/types.h>
#include <evm/return_data.h>
#include <evm/call.h>
#include <evm/vm_result.h>
#include <evm/gasometer.h>
#include <evm/trap.h>

class Utils {
  public:
    static void printOpcode(uint8_t value);
    static void printTier(uint8_t value);
    static void printOpcodeHex(uint8_t value);
    static void printInstruction(unsigned int value);
    static void printInstructionList();
    static void printInstructionRequirements(InstructionRequirements instructionRequirements);
    static void printTrap(const trap_t& trap);
    static void printJumps(const jump_set_t& jumps);
    static std::string uint256_2str(const uint256_t& value);
    static uint256_t bigIntFromBigEndianBytes(std::string bytecode_str);
    static env_t env();
    static params_t params(bytes_t code, bytes_t data);
    static params_t createParams(bytes_t code, bytes_t data);
    static gas_t gasLeft(exec_result_t vm_result);
};