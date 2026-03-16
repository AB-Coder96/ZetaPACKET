#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace zetapacket {

uint64_t now_ns();
std::string uname_string();
std::string cpu_model_string();
bool pin_thread_to_cpu(int cpu);
uint64_t fnv1a64(const void* data, size_t n, uint64_t seed = 14695981039346656037ull);
uint64_t fnv1a64_str(const std::string& s, uint64_t seed = 14695981039346656037ull);
int ifindex_from_name(const std::string& ifname);
bool ensure_parent_dir(const std::string& path);

}  // namespace zetapacket
