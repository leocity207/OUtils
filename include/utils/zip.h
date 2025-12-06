#ifndef UTILS_ZIP_H
#define UTILS_ZIP_H

#include <ranges>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace O 
{

	// Zip view: iterate pairs (A&, B&) for random-access sized ranges of same size.
	template <std::ranges::random_access_range R1, std::ranges::random_access_range R2>
	requires std::ranges::sized_range<R1> && std::ranges::sized_range<R2>
	class Zip_View;

	// Zip-index view: iterate (A&, std::size_t)
	template <std::ranges::random_access_range R>
	requires std::ranges::sized_range<R>
	class Zip_Index_View;

	template <std::ranges::random_access_range R>
	requires std::ranges::sized_range<R>
	class Zip_Adjacent_Circular_View;

	// Factory helpers (take lvalue references to ranges)
	template <std::ranges::random_access_range R1, std::ranges::random_access_range R2>
	requires std::ranges::sized_range<R1> && std::ranges::sized_range<R2>
	constexpr Zip_View<R1, R2> Zip(R1& a, R2& b) noexcept
	{
		return Zip_View<R1, R2>(a, b);
	}

	template <std::ranges::random_access_range R>
	requires std::ranges::sized_range<R>
	constexpr Zip_Index_View<R> Zip_Index(R& a) noexcept
	{
		return Zip_Index_View<R>(a);
	}

	/**
	 * @brief Zip i and i+1 element of a container to use inside a for loop
	 * 
	 * @tparam R the type inside the container
	 * @param a the container
	 * @return a view inside the container wich return [a[i],a[i+1]]
	 * @note when i=n-1 we circle around the vector
	 */
	template <std::ranges::random_access_range R>
	requires std::ranges::sized_range<R>
	constexpr Zip_Adjacent_Circular_View<R> Zip_Adjacent_Circular(R& a) noexcept
	{
		return Zip_Adjacent_Circular_View<R>(a);
	}
	

	// for_each_indexed(range, func)
	// func(elem_ref, index)
	template <std::ranges::forward_range Range, typename Func>
	requires std::invocable<Func&, std::ranges::range_reference_t<Range>, std::size_t>
	constexpr void For_Each_Indexed(Range&& range, Func&& func) noexcept(noexcept(std::declval<Func&>()(*std::ranges::begin(std::declval<Range&>()), std::size_t{})));

	// for_each_zip(rangeA, rangeB, func)
	// func(a_ref, b_ref)
	template <std::ranges::forward_range RangeA, std::ranges::forward_range RangeB, typename Func>
	requires std::invocable<Func&, std::ranges::range_reference_t<RangeA>, std::ranges::range_reference_t<RangeB>>
	constexpr void For_Each_Zip(RangeA&& a, RangeB&& b, Func&& func) noexcept(noexcept(std::declval<Func&>()(*std::ranges::begin(std::declval<RangeA&>()), *std::ranges::begin(std::declval<RangeB&>()))));

} // namespace O

// include implementation
#include "zip.hpp"


#endif //UTILS_ZIP_H