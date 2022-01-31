# DO NOT EDIT, GENERATED BY SCRIPT
set(llfio_HEADERS
  "include/kvstore/kvstore.hpp"
  "include/llfio.hpp"
  "include/llfio/llfio.hpp"
  "include/llfio/ntkernel-error-category/include/ntkernel-error-category/config.hpp"
  "include/llfio/ntkernel-error-category/include/ntkernel-error-category/detail/ntkernel-table.ipp"
  "include/llfio/ntkernel-error-category/include/ntkernel-error-category/detail/ntkernel_category_impl.ipp"
  "include/llfio/ntkernel-error-category/include/ntkernel-error-category/ntkernel_category.hpp"
  "include/llfio/revision.hpp"
  "include/llfio/v2.0/algorithm/clone.hpp"
  "include/llfio/v2.0/algorithm/contents.hpp"
  "include/llfio/v2.0/algorithm/difference.hpp"
  "include/llfio/v2.0/algorithm/handle_adapter/cached_parent.hpp"
  "include/llfio/v2.0/algorithm/handle_adapter/combining.hpp"
  "include/llfio/v2.0/algorithm/handle_adapter/xor.hpp"
  "include/llfio/v2.0/algorithm/reduce.hpp"
  "include/llfio/v2.0/algorithm/shared_fs_mutex/atomic_append.hpp"
  "include/llfio/v2.0/algorithm/shared_fs_mutex/base.hpp"
  "include/llfio/v2.0/algorithm/shared_fs_mutex/byte_ranges.hpp"
  "include/llfio/v2.0/algorithm/shared_fs_mutex/lock_files.hpp"
  "include/llfio/v2.0/algorithm/shared_fs_mutex/memory_map.hpp"
  "include/llfio/v2.0/algorithm/shared_fs_mutex/safe_byte_ranges.hpp"
  "include/llfio/v2.0/algorithm/summarize.hpp"
  "include/llfio/v2.0/algorithm/traverse.hpp"
  "include/llfio/v2.0/algorithm/trivial_vector.hpp"
  "include/llfio/v2.0/byte_io_handle.hpp"
  "include/llfio/v2.0/byte_io_multiplexer.hpp"
  "include/llfio/v2.0/byte_socket_handle.hpp"
  "include/llfio/v2.0/byte_socket_source.hpp"
  "include/llfio/v2.0/config.hpp"
  "include/llfio/v2.0/deadline.h"
  "include/llfio/v2.0/detail/impl/byte_io_multiplexer.ipp"
  "include/llfio/v2.0/detail/impl/byte_socket_handle.ipp"
  "include/llfio/v2.0/detail/impl/byte_socket_source.ipp"
  "include/llfio/v2.0/detail/impl/cached_parent_handle_adapter.ipp"
  "include/llfio/v2.0/detail/impl/clone.ipp"
  "include/llfio/v2.0/detail/impl/config.ipp"
  "include/llfio/v2.0/detail/impl/dynamic_thread_pool_group.ipp"
  "include/llfio/v2.0/detail/impl/fast_random_file_handle.ipp"
  "include/llfio/v2.0/detail/impl/getaddrinfo_category.hpp"
  "include/llfio/v2.0/detail/impl/map_handle.ipp"
  "include/llfio/v2.0/detail/impl/path_discovery.ipp"
  "include/llfio/v2.0/detail/impl/path_view.ipp"
  "include/llfio/v2.0/detail/impl/posix/byte_io_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/byte_socket_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/directory_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/file_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/fs_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/import.hpp"
  "include/llfio/v2.0/detail/impl/posix/lockable_byte_io_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/map_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/mapped_file_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/path_discovery.ipp"
  "include/llfio/v2.0/detail/impl/posix/path_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/pipe_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/process_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/stat.ipp"
  "include/llfio/v2.0/detail/impl/posix/statfs.ipp"
  "include/llfio/v2.0/detail/impl/posix/storage_profile.ipp"
  "include/llfio/v2.0/detail/impl/posix/symlink_handle.ipp"
  "include/llfio/v2.0/detail/impl/posix/test/io_uring_multiplexer.ipp"
  "include/llfio/v2.0/detail/impl/posix/utils.ipp"
  "include/llfio/v2.0/detail/impl/reduce.ipp"
  "include/llfio/v2.0/detail/impl/safe_byte_ranges.ipp"
  "include/llfio/v2.0/detail/impl/storage_profile.ipp"
  "include/llfio/v2.0/detail/impl/test/null_multiplexer.ipp"
  "include/llfio/v2.0/detail/impl/traverse.ipp"
  "include/llfio/v2.0/detail/impl/windows/byte_io_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/byte_io_multiplexer.ipp"
  "include/llfio/v2.0/detail/impl/windows/byte_socket_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/byte_socket_sources/schannel.ipp"
  "include/llfio/v2.0/detail/impl/windows/directory_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/file_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/fs_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/import.hpp"
  "include/llfio/v2.0/detail/impl/windows/lockable_byte_io_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/map_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/mapped_file_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/path_discovery.ipp"
  "include/llfio/v2.0/detail/impl/windows/path_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/pipe_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/process_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/stat.ipp"
  "include/llfio/v2.0/detail/impl/windows/statfs.ipp"
  "include/llfio/v2.0/detail/impl/windows/storage_profile.ipp"
  "include/llfio/v2.0/detail/impl/windows/symlink_handle.ipp"
  "include/llfio/v2.0/detail/impl/windows/test/iocp_multiplexer.ipp"
  "include/llfio/v2.0/detail/impl/windows/utils.ipp"
  "include/llfio/v2.0/directory_handle.hpp"
  "include/llfio/v2.0/dynamic_thread_pool_group.hpp"
  "include/llfio/v2.0/fast_random_file_handle.hpp"
  "include/llfio/v2.0/file_handle.hpp"
  "include/llfio/v2.0/fs_handle.hpp"
  "include/llfio/v2.0/handle.hpp"
  "include/llfio/v2.0/llfio.hpp"
  "include/llfio/v2.0/lockable_byte_io_handle.hpp"
  "include/llfio/v2.0/logging.hpp"
  "include/llfio/v2.0/map_handle.hpp"
  "include/llfio/v2.0/mapped.hpp"
  "include/llfio/v2.0/mapped_file_handle.hpp"
  "include/llfio/v2.0/multiplex.hpp"
  "include/llfio/v2.0/native_handle_type.hpp"
  "include/llfio/v2.0/path_discovery.hpp"
  "include/llfio/v2.0/path_handle.hpp"
  "include/llfio/v2.0/path_view.hpp"
  "include/llfio/v2.0/pipe_handle.hpp"
  "include/llfio/v2.0/process_handle.hpp"
  "include/llfio/v2.0/stat.hpp"
  "include/llfio/v2.0/statfs.hpp"
  "include/llfio/v2.0/status_code.hpp"
  "include/llfio/v2.0/storage_profile.hpp"
  "include/llfio/v2.0/symlink_handle.hpp"
  "include/llfio/v2.0/utils.hpp"
  "include/llfio/version.hpp"
)
