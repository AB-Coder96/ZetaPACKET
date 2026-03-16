#pragma once

#include "zetapacket/cli.h"
#include "zetapacket/stats.h"

namespace zetapacket {

bool run_kernel_udp_backend(const Options& opt, RunStats& stats);
bool run_af_xdp_backend(const Options& opt, RunStats& stats);
bool run_dpdk_backend(const Options& opt, RunStats& stats);

}  // namespace zetapacket
