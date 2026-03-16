#pragma once

#include <cstdint>
#include <vector>

namespace zetapacket {

class SeqTracker {
 public:
  explicit SeqTracker(uint32_t flows);
  uint64_t observe(uint32_t flow_id, uint64_t seq);
  uint64_t total_gaps() const { return gaps_; }

 private:
  std::vector<uint64_t> last_;
  uint64_t gaps_{0};
};

}  // namespace zetapacket
