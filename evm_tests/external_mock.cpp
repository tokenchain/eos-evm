#include "external_mock.h"

ExternalMock::ExternalMock() {
  logSpy = log_spy_t();
  codeSpy = code_spy_t();
  suicideSpy = suicide_spy_t();
  codeResponder = code_responder_t();
  balanceResponder = balance_responder_t();
  storageResponder = storage_responder_t();
}

void ExternalMock::log(std::vector<uint256_t>& topics, bytes_t& data) { 
  logSpy.push_back(std::make_pair(topics, data));
}

std::shared_ptr<bytes_t> ExternalMock::code(uint256_t address) {
  codeSpy.push_back(address);
  for (int i = 0; i < codeResponder.size(); i++) {
    if (codeResponder[i].first == address)
      return  std::make_shared<bytes_t>(codeResponder[i].second);
  }
  return std::make_shared<bytes_t>(bytes_t());
}

double ExternalMock::balance(uint256_t address) { 
  for (int i = 0; i < balanceResponder.size(); i++) {
    if (balanceResponder[i].first == address)
      return balanceResponder[i].second;
  }
  return 0.0; 
};

bytes_t ExternalMock::storageAt(uint256_t address) { 
  for (int i = 0; i < storageResponder.size(); i++) {
    if (storageResponder[i].first == address)
      return storageResponder[i].second;
  }
  return bytes_t();
};

void ExternalMock::suicide(uint256_t address) {
  suicideSpy.push_back(address);
}