#pragma once

#include <cstdint>
#include <string>

namespace zetapacket {

enum class Backend {
  KernelUdp,
  AfXdp,
  Dpdk,
};

struct Options {
  Backend backend{Backend::KernelUdp};
  std::string iface{"eth0"};
  uint16_t port{9000};
  uint32_t queue{0};
  uint32_t seconds{10};

  uint32_t batch{64};
  int cpu{-1};
  bool busy_poll{false};

  bool af_xdp_zero_copy{false};
  uint32_t umem_frames{4096};
  uint32_t frame_size{2048};
  uint32_t rx_ring{2048};
  uint32_t fill_ring{2048};

  std::string dpdk_args;
  uint16_t dpdk_burst{64};
  uint16_t dpdk_mbufs{8192};

  uint32_t flows{1};
  uint32_t packet_size{128};
  std::string out{"out.csv"};
  std::string xdp_obj_path;

  std::string config_string() const;
};

bool parse_cli(int argc, char** argv, Options& opt);

}  // namespace zetapacket
