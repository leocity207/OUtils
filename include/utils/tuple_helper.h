#ifndef UTILS_TUPLE_HELPER_H
#define UTILS_TUPLE_HELPER_H

#include <tuple>

namespace O
{
	/**
	 * @brief ``O::For_Each_In_Tuple(tuple, fn)`` provides a compact and readable way to apply a callable to every element of a ``std::tuple``.
	 * 
	 * @tparam Tuple type 
	 * @tparam F callable type
	 * 
	 * @param t the typle where to apply the function to
	 * @param f the function that will be called (f should be of auto type)
	 */
	template<class Tuple, class F>
	void For_Each_In_Tuple(Tuple& t, F&& f)
	{
		std::apply([&](auto&... e) {(f(e), ...);}, t);
	}
} // namespace O

#endif // UTILS_TUPLE_HELPER_H