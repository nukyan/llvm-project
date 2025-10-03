//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___ALGORITHM_SIFT_DOWN_H
#define _LIBCPP___ALGORITHM_SIFT_DOWN_H

#include <__algorithm/iterator_operations.h>
#include <__assert>
#include <__config>
#include <__iterator/iterator_traits.h>
#include <__utility/move.h>
#include <__utility/pair.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

_LIBCPP_BEGIN_NAMESPACE_STD

template <class _AlgPolicy, bool __assume_both_children, class _Compare, class _RandomAccessIterator>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX14 void
__choose_child(_RandomAccessIterator& __left,
               __iter_diff_t<_RandomAccessIterator>& __pos,
               __iter_diff_t<_RandomAccessIterator> __bound,
               _Compare&& __comp) {
  if (__assume_both_children || __pos < __bound) {
    _RandomAccessIterator __right = _IterOps<_AlgPolicy>::next(__left);
    bool __result                 = __comp(*__left, *__right);
    if _LIBCPP_CONSTEXPR (__libcpp_is_contiguous_iterator<_RandomAccessIterator>::value)
      __left += __result;
    else
      __left = __result ? __right : __left;
    __pos += __result;
  }
}

template <class _AlgPolicy, bool __assume_both_children, class _Compare, class _RandomAccessIterator>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX14 void
__sift_down(_RandomAccessIterator __first,
            _Compare&& __comp,
            __iter_diff_t<_RandomAccessIterator> __len,
            __iter_diff_t<_RandomAccessIterator> __start) {
  using _Ops = _IterOps<_AlgPolicy>;

  if (__len < 2)
    return;

  // left-child of __start is at 2 * __start + 1
  // right-child of __start is at 2 * __start + 2
  __iter_diff_t<_RandomAccessIterator> __child = 2 * __start + 1;
  _RandomAccessIterator __child_i = __first + __child, __start_i = __first + __start;

  __choose_child<_AlgPolicy, __assume_both_children>(__child_i, __child, __len - 1, __comp);

  // check if we are in heap-order
  if (__comp(*__child_i, *__start_i))
    // we are, __start is larger than its largest child
    return;

  __iter_value_type<_RandomAccessIterator> __top(_Ops::__iter_move(__start_i));
  do {
    // we are not in heap-order, swap the parent with its largest child
    *__start_i = _Ops::__iter_move(__child_i);
    __start_i  = __child_i;

    if (__len / 2 - 1 < __child)
      break;

    // recompute the child based off of the updated parent
    __child   = 2 * __child + 1;
    __child_i = __first + __child;

    __choose_child<_AlgPolicy, __assume_both_children>(__child_i, __child, __len - 1, __comp);

    // check if we are in heap-order
  } while (!__comp(*__child_i, __top));
  *__start_i = std::move(__top);
}

template <class _AlgPolicy, class _Compare, class _RandomAccessIterator>
_LIBCPP_HIDE_FROM_ABI
_LIBCPP_CONSTEXPR_SINCE_CXX14 std::pair<_RandomAccessIterator, __iter_diff_t<_RandomAccessIterator>>
__floyd_sift_down(_RandomAccessIterator __first, _Compare&& __comp, __iter_diff_t<_RandomAccessIterator> __len) {
  _LIBCPP_ASSERT_INTERNAL(__len > 0, "shouldn't be called unless __len > 0");
  using _Ops = _IterOps<_AlgPolicy>;

  __iter_diff_t<_RandomAccessIterator> __child = 1;
  _RandomAccessIterator __hole = __first, __child_i = __first;

  while (__child <= __len / 2) {
    __child_i += __child;
    __child *= 2;

    __choose_child<_AlgPolicy, true>(__child_i, __child, __len, __comp);

    // swap __hole with its largest child
    *__hole = _Ops::__iter_move(__child_i);
    __hole  = __child_i;
  }
  // if __hole is now a leaf, we're done
  return std::make_pair(__hole, __child);
}

_LIBCPP_END_NAMESPACE_STD

_LIBCPP_POP_MACROS

#endif // _LIBCPP___ALGORITHM_SIFT_DOWN_H
