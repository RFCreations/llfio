/* Integration test kernel for issue #73 Windows directory junctions cannot be opened with symlink_handle
(C) 2025 Niall Douglas <http://www.nedproductions.biz/> (2 commits)
File Created: Dec 2025


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

#include "../test_kernel_decl.hpp"

template <class T> struct print;

static inline void TestIssue0170()
{
  namespace llfio = LLFIO_V2_NAMESPACE;
#if LLFIO_EXPERIMENTAL_STATUS_CODE
  namespace outcome_e = OUTCOME_V2_NAMESPACE::experimental;
  llfio::file_io_error e0(llfio::generic_error(llfio::errc::state_not_recoverable));
  (void) e0.path1();

  using type1 = outcome_e::detail::safe_get_make_status_code_result<llfio::file_io_error>::type;
  static_assert(outcome_e::is_status_code<type1>::value, "make_status_code not working!");

  outcome_e::error e1(make_status_code(e0.clone()));
  (void) e1;

  outcome_e::error e2(std::move(e0));
  (void) e2;

  auto r = []() -> outcome_e::status_result<int>
  {
    auto c = []() -> llfio::result<int>
    { return llfio::file_io_error(llfio::generic_error(llfio::errc::state_not_recoverable)); };
    // This should implicitly drop the extra state in a LLFIO file_io_error
    OUTCOME_TRY(c());
    return llfio::success();
  }();
  BOOST_CHECK(r.has_error());
#else
  namespace outcome = OUTCOME_V2_NAMESPACE;
  auto r = []() -> outcome::result<int>
  {
    auto c = []() -> llfio::result<int>
    { return llfio::error_info(llfio::generic_error(llfio::errc::state_not_recoverable)); };
    // This should implicitly drop the extra state in a LLFIO error_info
    OUTCOME_TRY(c());
    return llfio::success();
  }();
  BOOST_CHECK(r.has_error());
#endif
  BOOST_CHECK(true);
}

KERNELTEST_TEST_KERNEL(regression, llfio, issues, 0170, "Tests issue #0170 file_io_error no longer converts to error",
                       TestIssue0170())
