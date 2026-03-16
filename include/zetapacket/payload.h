#pragma once

#include <cstdint>

namespace zetapacket {

struct PayloadHeader {
  uint64_t seq;
  uint64_t tx_ts_ns;
  uint32_t flow_id;
  uint32_t reserved;
};

static_assert(sizeof(PayloadHeader) == 24);

}  // namespace zetapacket
