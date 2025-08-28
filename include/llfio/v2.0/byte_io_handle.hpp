/* A handle to something
(C) 2015-2020 Niall Douglas <http://www.nedproductions.biz/> (20 commits)
File Created: Dec 2015


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License in the accompanying file
Licence.txt or at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


Distributed under the Boost Software License, Version 1.0.
    (See accompanying file Licence.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LLFIO_IO_HANDLE_H
#define LLFIO_IO_HANDLE_H

#include "handle.hpp"

//! \file byte_io_handle.hpp Provides a byte-orientated i/o handle

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)  // dll interface
#endif

LLFIO_V2_NAMESPACE_EXPORT_BEGIN

/*! \class byte_io_handle
\brief A handle to something capable of scatter-gather byte i/o.
*/
class LLFIO_DECL byte_io_handle : public handle
{
  friend class byte_io_multiplexer;

public:
  using path_type = handle::path_type;
  using extent_type = handle::extent_type;
  using size_type = handle::size_type;
  using mode = handle::mode;
  using creation = handle::creation;
  using caching = handle::caching;
  using flag = handle::flag;

  //! The kinds of write reordering barrier which can be performed.
  enum class barrier_kind : uint8_t
  {
    nowait_view_only,  //!< Barrier mapped data only, non-blocking. This is highly optimised on NV-DIMM storage, but consider using `nvram_barrier()` for even better performance.
    wait_view_only,  //!< Barrier mapped data only, block until it is done. This is highly optimised on NV-DIMM storage, but consider using `nvram_barrier()` for even better performance.
    nowait_data_only,  //!< Barrier data only, non-blocking. This is highly optimised on NV-DIMM storage, but consider using `nvram_barrier()` for even better performance.
    wait_data_only,  //!< Barrier data only, block until it is done. This is highly optimised on NV-DIMM storage, but consider using `nvram_barrier()` for even better performance.
    nowait_all,  //!< Barrier data and the metadata to retrieve it, non-blocking.
    wait_all     //!< Barrier data and the metadata to retrieve it, block until it is done.
  };

  //! The scatter buffer type used by this handle. Guaranteed to be `TrivialType` and `StandardLayoutType`.
  //! Try to make address and length 64 byte, or ideally, `page_size()` aligned where possible.
  struct buffer_type
  {
    //! Type of the pointer to memory.
    using pointer = byte *;
    //! Type of the pointer to memory.
    using const_pointer = const byte *;
    //! Type of the iterator to memory.
    using iterator = byte *;
    //! Type of the iterator to memory.
    using const_iterator = const byte *;
    //! Type of the length of memory.
    using size_type = size_t;

  private:
    struct _implict_span_constructor_tag
    {
    };

  public:
    //! Default constructor
    buffer_type() = default;
    //! Constructor
    constexpr buffer_type(pointer data, size_type len) noexcept
        : _data(data)
        , _len(len)
    {
    }
    //! Constructor taking any type acceptable to `span`.
    LLFIO_TEMPLATE(class T)
    LLFIO_TREQUIRES(LLFIO_TPRED(!std::is_same<typename std::decay<T>::type, buffer_type>::value && std::is_constructible<span<byte>, T>::value))
    constexpr buffer_type(T &&s, _implict_span_constructor_tag = {}) noexcept
        : _data(span<byte>(s).data())
        , _len(span<byte>(s).size())
    {
    }
    buffer_type(const buffer_type &) = default;
    buffer_type(buffer_type &&) = default;
    buffer_type &operator=(const buffer_type &) = default;
    buffer_type &operator=(buffer_type &&) = default;
    ~buffer_type() = default;

    // Emulation of this being a span<byte> in the TS

    //! Returns if this buffer is empty
    LLFIO_NODISCARD constexpr bool empty() const noexcept { return _len == 0; }
    //! Returns the address of the bytes for this buffer
    constexpr pointer data() noexcept { return _data; }
    //! Returns the address of the bytes for this buffer
    constexpr const_pointer data() const noexcept { return _data; }
    //! Returns the number of bytes in this buffer
    constexpr size_type size() const noexcept { return _len; }

    //! Returns an iterator to the beginning of the buffer
    constexpr iterator begin() noexcept { return _data; }
    //! Returns an iterator to the beginning of the buffer
    constexpr const_iterator begin() const noexcept { return _data; }
    //! Returns an iterator to the beginning of the buffer
    constexpr const_iterator cbegin() const noexcept { return _data; }
    //! Returns an iterator to after the end of the buffer
    constexpr iterator end() noexcept { return _data + _len; }
    //! Returns an iterator to after the end of the buffer
    constexpr const_iterator end() const noexcept { return _data + _len; }
    //! Returns an iterator to after the end of the buffer
    constexpr const_iterator cend() const noexcept { return _data + _len; }

  private:
    friend constexpr inline void _check_iovec_match();
    pointer _data;
    size_type _len;
  };

  //! The gather buffer type used by this handle. Guaranteed to be `TrivialType` and `StandardLayoutType`.
  //! Try to make address and length 64 byte, or ideally, `page_size()` aligned where possible.
  struct const_buffer_type
  {
    //! Type of the pointer to memory.
    using pointer = const byte *;
    //! Type of the pointer to memory.
    using const_pointer = const byte *;
    //! Type of the iterator to memory.
    using iterator = const byte *;
    //! Type of the iterator to memory.
    using const_iterator = const byte *;
    //! Type of the length of memory.
    using size_type = size_t;

  private:
    struct _implict_span_constructor_tag
    {
    };

  public:
    //! Default constructor
    const_buffer_type() = default;
    //! Constructor
    constexpr const_buffer_type(pointer data, size_type len) noexcept
        : _data(data)
        , _len(len)
    {
    }
    //! Converting constructor from non-const buffer type
    constexpr const_buffer_type(buffer_type b) noexcept
        : _data(b.data())
        , _len(b.size())
    {
    }
    //! Constructor taking any type acceptable to `span`.
    LLFIO_TEMPLATE(class T)
    LLFIO_TREQUIRES(LLFIO_TPRED(!std::is_same<typename std::decay<T>::type, buffer_type>::value && std::is_constructible<span<const byte>, T>::value))
    constexpr const_buffer_type(T &&s, _implict_span_constructor_tag = {}) noexcept
        : _data(span<const byte>(s).data())
        , _len(span<const byte>(s).size())
    {
    }
    const_buffer_type(const const_buffer_type &) = default;
    const_buffer_type(const_buffer_type &&) = default;
    const_buffer_type &operator=(const const_buffer_type &) = default;
    const_buffer_type &operator=(const_buffer_type &&) = default;
    ~const_buffer_type() = default;

    // Emulation of this being a span<byte> in the TS

    //! Returns if this buffer is empty
    LLFIO_NODISCARD constexpr bool empty() const noexcept { return _len == 0; }
    //! Returns the address of the bytes for this buffer
    constexpr pointer data() noexcept { return _data; }
    //! Returns the address of the bytes for this buffer
    constexpr const_pointer data() const noexcept { return _data; }
    //! Returns the number of bytes in this buffer
    constexpr size_type size() const noexcept { return _len; }

    //! Returns an iterator to the beginning of the buffer
    constexpr iterator begin() noexcept { return _data; }
    //! Returns an iterator to the beginning of the buffer
    constexpr const_iterator begin() const noexcept { return _data; }
    //! Returns an iterator to the beginning of the buffer
    constexpr const_iterator cbegin() const noexcept { return _data; }
    //! Returns an iterator to after the end of the buffer
    constexpr iterator end() noexcept { return _data + _len; }
    //! Returns an iterator to after the end of the buffer
    constexpr const_iterator end() const noexcept { return _data + _len; }
    //! Returns an iterator to after the end of the buffer
    constexpr const_iterator cend() const noexcept { return _data + _len; }

  private:
    pointer _data;
    size_type _len;
  };
#ifndef NDEBUG
  static_assert(std::is_trivial<buffer_type>::value, "buffer_type is not a trivial type!");
  static_assert(std::is_trivial<const_buffer_type>::value, "const_buffer_type is not a trivial type!");
  static_assert(std::is_standard_layout<buffer_type>::value, "buffer_type is not a standard layout type!");
  static_assert(std::is_standard_layout<const_buffer_type>::value, "const_buffer_type is not a standard layout type!");
#endif

  struct _registered_buffer_type : std::enable_shared_from_this<_registered_buffer_type>, span<byte>
  {
    using span<byte>::span;
    explicit _registered_buffer_type(span<byte> o)
        : span<byte>(o)
    {
    }
    _registered_buffer_type() = default;
    _registered_buffer_type(const _registered_buffer_type &) = default;
    _registered_buffer_type(_registered_buffer_type &&) = default;
    _registered_buffer_type &operator=(const _registered_buffer_type &) = default;
    _registered_buffer_type &operator=(_registered_buffer_type &&) = default;
    ~_registered_buffer_type() = default;
  };
  //! The registered buffer type used by this handle.
  using registered_buffer_type = std::shared_ptr<_registered_buffer_type>;

  //! The scatter buffers type used by this handle. Guaranteed to be `TrivialType` apart from construction, and `StandardLayoutType`.
  using buffers_type = span<buffer_type>;
  //! The gather buffers type used by this handle. Guaranteed to be `TrivialType` apart from construction, and `StandardLayoutType`.
  using const_buffers_type = span<const_buffer_type>;
#ifndef NDEBUG
  // Is trivial in all ways, except default constructibility
  static_assert(std::is_trivially_copyable<buffers_type>::value, "buffers_type is not trivially copyable!");
  // static_assert(std::is_trivially_assignable<buffers_type, buffers_type>::value, "buffers_type is not trivially assignable!");
  // static_assert(std::is_trivially_destructible<buffers_type>::value, "buffers_type is not trivially destructible!");
  // static_assert(std::is_trivially_copy_constructible<buffers_type>::value, "buffers_type is not trivially copy constructible!");
  // static_assert(std::is_trivially_move_constructible<buffers_type>::value, "buffers_type is not trivially move constructible!");
  // static_assert(std::is_trivially_copy_assignable<buffers_type>::value, "buffers_type is not trivially copy assignable!");
  // static_assert(std::is_trivially_move_assignable<buffers_type>::value, "buffers_type is not trivially move assignable!");
#if !defined(_MSC_VER) || _MSC_VER < 1926
  static_assert(std::is_standard_layout<buffers_type>::value, "buffers_type is not a standard layout type!");
#endif
#endif

  //! The i/o request type used by this handle. Guaranteed to be `TrivialType` apart from construction, and `StandardLayoutType`.
  template <class T> struct io_request
  {
    T buffers{};
    extent_type offset{0};
    constexpr io_request() {}  // NOLINT (defaulting this breaks clang and GCC, so don't do it!)
    constexpr io_request(T _buffers, extent_type _offset = (extent_type) -1)
        : buffers(std::move(_buffers))
        , offset(_offset)
    {
    }
  };
#ifndef NDEBUG
  // Is trivial in all ways, except default constructibility
  static_assert(std::is_trivially_copyable<io_request<buffers_type>>::value, "io_request<buffers_type> is not trivially copyable!");
  // static_assert(std::is_trivially_assignable<io_request<buffers_type>, io_request<buffers_type>>::value, "io_request<buffers_type> is not trivially
  // assignable!"); static_assert(std::is_trivially_destructible<io_request<buffers_type>>::value, "io_request<buffers_type> is not trivially destructible!");
  // static_assert(std::is_trivially_copy_constructible<io_request<buffers_type>>::value, "io_request<buffers_type> is not trivially copy constructible!");
  // static_assert(std::is_trivially_move_constructible<io_request<buffers_type>>::value, "io_request<buffers_type> is not trivially move constructible!");
  // static_assert(std::is_trivially_copy_assignable<io_request<buffers_type>>::value, "io_request<buffers_type> is not trivially copy assignable!");
  // static_assert(std::is_trivially_move_assignable<io_request<buffers_type>>::value, "io_request<buffers_type> is not trivially move assignable!");
#if !defined(_MSC_VER) || _MSC_VER < 1926
  static_assert(std::is_standard_layout<io_request<buffers_type>>::value, "io_request<buffers_type> is not a standard layout type!");
#endif
#endif
  //! The i/o result type used by this handle. Guaranteed to be `TrivialType` apart from construction.
  template <class T> struct io_result : public LLFIO_V2_NAMESPACE::result<T>
  {
    using Base = LLFIO_V2_NAMESPACE::result<T>;
    size_type _bytes_transferred{static_cast<size_type>(-1)};

#if defined(_MSC_VER) && _MSC_VER < 1930 /*VS2022*/ && !defined(__clang__)  // workaround MSVC parsing bug
    constexpr io_result()
        : Base()
    {
    }
    template <class... Args>
    constexpr io_result(Args &&...args)
        : Base(std::forward<Args>(args)...)
    {
    }
#else
    using Base::Base;
    io_result() = default;
#endif
    ~io_result() = default;
    io_result &operator=(io_result &&) = default;  // NOLINT
#if LLFIO_EXPERIMENTAL_STATUS_CODE
    io_result(const io_result &) = delete;
    io_result &operator=(const io_result &) = delete;
#else
    io_result(const io_result &) = default;
    io_result &operator=(const io_result &) = default;
#endif
    io_result(io_result &&) = default;  // NOLINT
    //! Returns bytes transferred
    size_type bytes_transferred() noexcept
    {
      if(_bytes_transferred == static_cast<size_type>(-1))
      {
        _bytes_transferred = 0;
        for(auto &i : this->value())
        {
          _bytes_transferred += i.size();
        }
      }
      return _bytes_transferred;
    }
  };
#if !defined(NDEBUG) && !LLFIO_EXPERIMENTAL_STATUS_CODE
  // Is trivial in all ways, except default constructibility
  static_assert(std::is_trivially_copyable<io_result<buffers_type>>::value, "io_result<buffers_type> is not trivially copyable!");
// static_assert(std::is_trivially_assignable<io_result<buffers_type>, io_result<buffers_type>>::value, "io_result<buffers_type> is not trivially assignable!");
// static_assert(std::is_trivially_destructible<io_result<buffers_type>>::value, "io_result<buffers_type> is not trivially destructible!");
// static_assert(std::is_trivially_copy_constructible<io_result<buffers_type>>::value, "io_result<buffers_type> is not trivially copy constructible!");
// static_assert(std::is_trivially_move_constructible<io_result<buffers_type>>::value, "io_result<buffers_type> is not trivially move constructible!");
// static_assert(std::is_trivially_copy_assignable<io_result<buffers_type>>::value, "io_result<buffers_type> is not trivially copy assignable!");
// static_assert(std::is_trivially_move_assignable<io_result<buffers_type>>::value, "io_result<buffers_type> is not trivially move assignable!");
//! \todo Why is io_result<buffers_type> not a standard layout type?
// static_assert(std::is_standard_layout<result<buffers_type>>::value, "result<buffers_type> is not a standard layout type!");
// static_assert(std::is_standard_layout<io_result<buffers_type>>::value, "io_result<buffers_type> is not a standard layout type!");
#endif

public:
  //! Default constructor
  constexpr byte_io_handle() {}  // NOLINT
  ~byte_io_handle() = default;
  //! Construct a handle from a supplied native handle
  constexpr explicit byte_io_handle(native_handle_type h, flag flags)
      : handle(h, flags)
  {
  }
  //! Explicit conversion from handle permitted
  explicit constexpr byte_io_handle(handle &&o) noexcept
      : handle(std::move(o))
  {
  }
  //! Move construction permitted
  byte_io_handle(byte_io_handle &&) = default;
  //! No copy construction (use `clone()`)
  byte_io_handle(const byte_io_handle &) = delete;
  //! Move assignment permitted
  byte_io_handle &operator=(byte_io_handle &&) = default;
  //! No copy assignment
  byte_io_handle &operator=(const byte_io_handle &) = delete;

protected:
  //! The virtualised implementation of `max_buffers()` used.
  LLFIO_HEADERS_ONLY_VIRTUAL_SPEC size_t _do_max_buffers() const noexcept;
  //! The virtualised implementation of `read()` used if no multiplexer has been set.
  LLFIO_HEADERS_ONLY_VIRTUAL_SPEC io_result<buffers_type> _do_read(io_request<buffers_type> reqs, deadline d) noexcept;
  //! The virtualised implementation of `write()` used.
  LLFIO_HEADERS_ONLY_VIRTUAL_SPEC io_result<const_buffers_type> _do_write(io_request<const_buffers_type> reqs, deadline d) noexcept;
  //! The virtualised implementation of `barrier()` used.
  LLFIO_HEADERS_ONLY_VIRTUAL_SPEC io_result<const_buffers_type> _do_barrier(io_request<const_buffers_type> reqs, barrier_kind kind, deadline d) noexcept;

public:
  /*! \brief The *maximum* number of buffers which a single read or write syscall can (atomically)
  process at a time for this specific open handle. On POSIX, this is known as `IOV_MAX`.
  Preferentially uses any i/o multiplexer set over the virtually overridable per-class implementation.

  Note that the actual number of buffers accepted for a read or a write may be significantly
  lower than this system-defined limit, depending on available resources. The `read()` or `write()`
  call will return the buffers accepted at the time of invoking the syscall.

  Note also that some OSs will error out if you supply more than this limit to `read()` or `write()`,
  but other OSs do not. Some OSs guarantee that each i/o syscall has effects atomically visible or not
  to other i/o, other OSs do not.

  OS X does not implement scatter-gather file i/o syscalls. Thus this function will always return
  `1` in that situation.

  Microsoft Windows *may* implement scatter-gather i/o under certain handle configurations.
  Most of the time for non-socket handles this function will return `1`.

  For handles which implement i/o entirely in user space, and thus syscalls are not involved,
  this function will return `0`.
  */
  size_t max_buffers() const noexcept { return _do_max_buffers(); }

  /*! \brief Read data from the open handle, preferentially using any i/o multiplexer set over the
  virtually overridable per-class implementation.

  \warning Depending on the implementation backend, **very** different buffers may be returned than you
  supplied. You should **always** use the buffers returned and assume that they point to different
  memory and that each buffer's size will have changed.

  \return The buffers read, which may not be the buffers input. The size of each scatter-gather
  buffer returned is updated with the number of bytes of that buffer transferred, and the pointer
  to the data may be \em completely different to what was submitted (e.g. it may point into a
  memory map).
  \param reqs A scatter-gather and offset request.
  \param d An optional deadline by which the i/o must complete, else it is cancelled.
  Note function may return significantly after this deadline if the i/o takes long to cancel.
  \errors Any of the values POSIX read() can return, `errc::timed_out`, `errc::operation_canceled`. `errc::not_supported` may be
  returned if deadline i/o is not possible with this particular handle configuration (e.g.
  reading from regular files on POSIX or reading from a non-overlapped HANDLE on Windows).
  \mallocs The default synchronous implementation in file_handle performs no memory allocation.
  */
  LLFIO_MAKE_FREE_FUNCTION
  io_result<buffers_type> read(io_request<buffers_type> reqs, deadline d = deadline()) noexcept { return _do_read(reqs, d); }
  //! \overload
  LLFIO_MAKE_FREE_FUNCTION
  io_result<size_type> read(extent_type offset, std::initializer_list<buffer_type> lst, deadline d = deadline()) noexcept
  {
    buffer_type *_reqs = reinterpret_cast<buffer_type *>(alloca(sizeof(buffer_type) * lst.size()));
    memcpy(_reqs, lst.begin(), sizeof(buffer_type) * lst.size());
    io_request<buffers_type> reqs(buffers_type(_reqs, lst.size()), offset);
    auto ret = read(reqs, d);
    if(ret)
    {
      return ret.bytes_transferred();
    }
    return std::move(ret).error();
  }

  LLFIO_DEADLINE_TRY_FOR_UNTIL(read)

  /*! \brief Write data to the open handle, preferentially using any i/o multiplexer set over the
  virtually overridable per-class implementation.

  \warning Depending on the implementation backend, not all of the buffers input may be written.
  For example, with a zeroed deadline, some backends may only consume as many buffers as the system has available write slots
  for, thus for those backends this call is "non-blocking" in the sense that it will return immediately even if it
  could not schedule a single buffer write. Another example is that some implementations will not
  auto-extend the length of a file when a write exceeds the maximum extent, you will need to issue
  a `truncate(newsize)` first.

  \return The buffers written, which may not be the buffers input. The size of each scatter-gather
  buffer returned is updated with the number of bytes of that buffer transferred.
  \param reqs A scatter-gather and offset request.
  \param d An optional deadline by which the i/o must complete, else it is cancelled.
  Note function may return significantly after this deadline if the i/o takes long to cancel.
  \errors Any of the values POSIX write() can return, `errc::timed_out`, `errc::operation_canceled`. `errc::not_supported` may be
  returned if deadline i/o is not possible with this particular handle configuration (e.g.
  writing to regular files on POSIX or writing to a non-overlapped HANDLE on Windows).
  \mallocs The default synchronous implementation in file_handle performs no memory allocation.
  */
  LLFIO_MAKE_FREE_FUNCTION
  io_result<const_buffers_type> write(io_request<const_buffers_type> reqs, deadline d = deadline()) noexcept { return _do_write(reqs, d); }
  //! \overload
  LLFIO_MAKE_FREE_FUNCTION
  io_result<size_type> write(extent_type offset, std::initializer_list<const_buffer_type> lst, deadline d = deadline()) noexcept
  {
    const_buffer_type *_reqs = reinterpret_cast<const_buffer_type *>(alloca(sizeof(const_buffer_type) * lst.size()));
    memcpy(_reqs, lst.begin(), sizeof(const_buffer_type) * lst.size());
    io_request<const_buffers_type> reqs(const_buffers_type(_reqs, lst.size()), offset);
    auto ret = write(reqs, d);
    if(ret)
    {
      return ret.bytes_transferred();
    }
    return std::move(ret).error();
  }

  LLFIO_DEADLINE_TRY_FOR_UNTIL(write)

  /*! \brief Issue a write reordering barrier such that writes preceding the barrier will reach
  storage before writes after this barrier, preferentially using any i/o multiplexer set over the
  virtually overridable per-class implementation.

  \warning **Assume that this call is a no-op**. It is not reliably implemented in many common
  use cases, for example if your code is running inside a LXC container, or if the user has mounted
  the filing system with non-default options. Instead open the handle with `caching::reads` which
  means that all writes form a strict sequential order not completing until acknowledged by the
  storage device. Filing system can and do use different algorithms to give much better performance
  with `caching::reads`, some (e.g. ZFS) spectacularly better.

  \warning Let me repeat again: consider this call to be a **hint** to poke the kernel with a stick
  to go start to do some work sooner rather than later. **It may be ignored entirely**.

  \warning For portability, you can only assume that barriers write order for a single handle
  instance. You cannot assume that barriers write order across multiple handles to the same inode,
  or across processes.

  \return The buffers barriered, which may not be the buffers input. The size of each scatter-gather
  buffer is updated with the number of bytes of that buffer barriered.
  \param reqs A scatter-gather and offset request for what range to barrier. May be ignored on
  some platforms which always write barrier the entire file. Supplying a default initialised reqs
  write barriers the entire file.
  \param kind Which kind of write reordering barrier to perform.
  \param d An optional deadline by which the i/o must complete, else it is cancelled.
  Note function may return significantly after this deadline if the i/o takes long to cancel.
  \errors Any of the values POSIX fdatasync() or Windows NtFlushBuffersFileEx() can return.
  \mallocs None.
  */
  LLFIO_MAKE_FREE_FUNCTION
  LLFIO_HEADERS_ONLY_VIRTUAL_SPEC io_result<const_buffers_type> barrier(io_request<const_buffers_type> reqs = io_request<const_buffers_type>(),
                                                                        barrier_kind kind = barrier_kind::nowait_data_only, deadline d = deadline()) noexcept
  {
    return _do_barrier(reqs, kind, d);
  }
  //! \overload
  LLFIO_MAKE_FREE_FUNCTION
  io_result<const_buffers_type> barrier(barrier_kind kind, deadline d = deadline()) noexcept { return barrier(io_request<const_buffers_type>(), kind, d); }

  LLFIO_DEADLINE_TRY_FOR_UNTIL(barrier)
};
static_assert((sizeof(void *) == 4 && sizeof(byte_io_handle) == 16) || (sizeof(void *) == 8 && sizeof(byte_io_handle) == 24),
              "byte_io_handle is not 20 or 32 bytes in size!");

// BEGIN make_free_functions.py
/*! \brief Read data from the open handle.

\warning Depending on the implementation backend, **very** different buffers may be returned than you
supplied. You should **always** use the buffers returned and assume that they point to different
memory and that each buffer's size will have changed.

\return The buffers read, which may not be the buffers input. The size of each scatter-gather
buffer is updated with the number of bytes of that buffer transferred, and the pointer to
the data may be \em completely different to what was submitted (e.g. it may point into a
memory map).
\param self The object whose member function to call.
\param reqs A scatter-gather and offset request.
\param d An optional deadline by which the i/o must complete, else it is cancelled.
Note function may return significantly after this deadline if the i/o takes long to cancel.
\errors Any of the values POSIX read() can return, `errc::timed_out`, `errc::operation_canceled`. `errc::not_supported` may be
returned if deadline i/o is not possible with this particular handle configuration (e.g.
reading from regular files on POSIX or reading from a non-overlapped HANDLE on Windows).
\mallocs The default synchronous implementation in file_handle performs no memory allocation.
*/
inline byte_io_handle::io_result<byte_io_handle::buffers_type> read(byte_io_handle &self, byte_io_handle::io_request<byte_io_handle::buffers_type> reqs,
                                                                    deadline d = deadline()) noexcept
{
  return self.read(std::forward<decltype(reqs)>(reqs), std::forward<decltype(d)>(d));
}
/*! \brief Write data to the open handle.

\warning Depending on the implementation backend, not all of the buffers input may be written and
the some buffers at the end of the returned buffers may return with zero bytes written.
For example, with a zeroed deadline, some backends may only consume as many buffers as the system has available write slots
for, thus for those backends this call is "non-blocking" in the sense that it will return immediately even if it
could not schedule a single buffer write. Another example is that some implementations will not
auto-extend the length of a file when a write exceeds the maximum extent, you will need to issue
a `truncate(newsize)` first.

\return The buffers written, which may not be the buffers input. The size of each scatter-gather
buffer is updated with the number of bytes of that buffer transferred.
\param self The object whose member function to call.
\param reqs A scatter-gather and offset request.
\param d An optional deadline by which the i/o must complete, else it is cancelled.
Note function may return significantly after this deadline if the i/o takes long to cancel.
\errors Any of the values POSIX write() can return, `errc::timed_out`, `errc::operation_canceled`. `errc::not_supported` may be
returned if deadline i/o is not possible with this particular handle configuration (e.g.
writing to regular files on POSIX or writing to a non-overlapped HANDLE on Windows).
\mallocs The default synchronous implementation in file_handle performs no memory allocation.
*/
inline byte_io_handle::io_result<byte_io_handle::const_buffers_type>
write(byte_io_handle &self, byte_io_handle::io_request<byte_io_handle::const_buffers_type> reqs, deadline d = deadline()) noexcept
{
  return self.write(std::forward<decltype(reqs)>(reqs), std::forward<decltype(d)>(d));
}
//! \overload
inline byte_io_handle::io_result<byte_io_handle::size_type>
write(byte_io_handle &self, byte_io_handle::extent_type offset, std::initializer_list<byte_io_handle::const_buffer_type> lst, deadline d = deadline()) noexcept
{
  return self.write(std::forward<decltype(offset)>(offset), std::forward<decltype(lst)>(lst), std::forward<decltype(d)>(d));
}
// END make_free_functions.py

LLFIO_V2_NAMESPACE_END

#if LLFIO_HEADERS_ONLY == 1 && !defined(DOXYGEN_SHOULD_SKIP_THIS)
#define LLFIO_INCLUDED_BY_HEADER 1
#ifdef _WIN32
#include "detail/impl/windows/byte_io_handle.ipp"
#else
#include "detail/impl/posix/byte_io_handle.ipp"
#endif
#undef LLFIO_INCLUDED_BY_HEADER
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
