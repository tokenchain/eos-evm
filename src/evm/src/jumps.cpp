#include <evm/instruction.h>
#include <evm/jumps.h>
#include <evm/opcode.h>

jump_set_t Jumps::findDestinations(std::shared_ptr<bytes_t> bytes) {
  jump_set_t jumps = jump_set_t();

  int position = 0;

  while (position < bytes->size()) {
    char index = bytes->at(position);
    int instruction = Instruction::values[index];

    if (Instruction::opcode(instruction) == Opcode::JUMPDEST) {
      jumps.insert(position);
    } else {
      position += Instruction::pushBytes(instruction);
    }

    position++;
  }

  return jumps;
}

uint64_t Jumps::verifyJump(uint256_t position, jump_set_t& validDestinations) {
  uint64_t jump = static_cast<uint64_t>(position);
  if (validDestinations.find(jump) != validDestinations.end()) return jump;
  return INVALID_ARGUMENT;
}