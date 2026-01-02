#ifndef UTILS_COMB_RANGES_HPP
#define UTILS_COMB_RANGES_HPP

#include "comb.h"

#include <utility>

namespace O::Comb {

	// ------------------- Iterator_Impl definitions -------------------
	// Note: two-level template-list is required for nested-template member definitions.

	// Constructor
	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	template <typename BaseIt>
	View<Width, Shift, V>::Iterator_Impl<BaseIt>::Iterator_Impl(BaseIt it, BaseIt end) noexcept
		: m_it(it), m_end(end) {}

	// dereference
	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	template <typename BaseIt>
	typename View<Width, Shift, V>::template Iterator_Impl<BaseIt>::reference
	View<Width, Shift, V>::Iterator_Impl<BaseIt>::operator*() const noexcept {
		return *m_it;
	}

	// pre-increment: advance Width times (stop at end)
	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	template <typename BaseIt>
	typename View<Width, Shift, V>::template Iterator_Impl<BaseIt>&
	View<Width, Shift, V>::Iterator_Impl<BaseIt>::operator++() noexcept {
		if (m_it == m_end) return *this;
		for (std::size_t i = 0; i < Width && m_it != m_end; ++i)
			++m_it;
		return *this;
	}

	// post-increment
	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	template <typename BaseIt>
	typename View<Width, Shift, V>::template Iterator_Impl<BaseIt>
	View<Width, Shift, V>::Iterator_Impl<BaseIt>::operator++(int) noexcept {
		Iterator_Impl tmp = *this;
		++(*this);
		return tmp;
	}

	// equality
	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	template <typename BaseIt>
	bool View<Width, Shift, V>::Iterator_Impl<BaseIt>::operator==(const Iterator_Impl& other) const noexcept {
		return m_it == other.m_it;
	}

	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	template <typename BaseIt>
	bool View<Width, Shift, V>::Iterator_Impl<BaseIt>::operator!=(const Iterator_Impl& other) const noexcept {
		return !(*this == other);
	}

	// operator+= (random-access)
	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	template <typename BaseIt>
	template <typename It>
	requires std::random_access_iterator<It>
	auto View<Width, Shift, V>::Iterator_Impl<BaseIt>::operator+=(difference_type n) noexcept
		-> typename View<Width, Shift, V>::template Iterator_Impl<BaseIt>&
	{
		m_it += n * static_cast<difference_type>(Width);
		return *this;
	}

	// difference (random-access)
	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	template <typename BaseIt>
	template <typename It>
	requires std::random_access_iterator<It>
	auto View<Width, Shift, V>::Iterator_Impl<BaseIt>::operator-(const Iterator_Impl& other) const noexcept
		-> typename View<Width, Shift, V>::template Iterator_Impl<BaseIt>::difference_type
	{
		return (m_it - other.m_it) / static_cast<difference_type>(Width);
	}

	// operator[] (random-access)
	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	template <typename BaseIt>
	template <typename It>
	requires std::random_access_iterator<It>
	auto View<Width, Shift, V>::Iterator_Impl<BaseIt>::operator[](difference_type n) const noexcept
		-> typename View<Width, Shift, V>::template Iterator_Impl<BaseIt>::reference
	{
		return *(m_it + n * static_cast<difference_type>(Width));
	}

	// ------------------- View member definitions -------------------

	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	View<Width, Shift, V>::View(V base)
		: m_container(std::views::all(base)) {}

	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	auto View<Width, Shift, V>::begin() -> iterator {
		auto b = std::ranges::begin(m_container);
		auto e = std::ranges::end(m_container);
		for (std::size_t i = 0; i < Shift && b != e; ++i)
			++b;
		return iterator{b, e};
	}

	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	auto View<Width, Shift, V>::end() -> iterator {
		auto e = std::ranges::end(m_container);
		return iterator{e, e};
	}

	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	auto View<Width, Shift, V>::begin() const -> const_iterator
	requires std::ranges::forward_range<const V> {
		auto b = std::ranges::begin(m_container);
		auto e = std::ranges::end(m_container);
		for (std::size_t i = 0; i < Shift && b != e; ++i)
			++b;
		return const_iterator{b, e};
	}

	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	auto View<Width, Shift, V>::end() const -> const_iterator
	requires std::ranges::forward_range<const V> {
		auto e = std::ranges::end(m_container);
		return const_iterator{e, e};
	}

	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	std::size_t View<Width, Shift, V>::size() const
	requires std::ranges::sized_range<V> {
		const std::size_t n = static_cast<std::size_t>(std::ranges::size(m_container));
		if (n <= Shift) return 0;
		return (n - Shift + Width - 1) / Width;
	}

	// helpers

	template <std::size_t Width, std::size_t Shift, std::ranges::viewable_range R>
	View<Width, Shift, R> Make_Comb(R&& r) {
		return View<Width, Shift, R>{std::forward<R>(r)};
	}

	template <std::ranges::viewable_range R>
	View<2, 0, R> Even(R&& r) {
		return Make_Comb<2, 0>(std::forward<R>(r));
	}

	template <std::ranges::viewable_range R>
	View<2, 1, R> Odd(R&& r) {
		return Make_Comb<2, 1>(std::forward<R>(r));
	}

} // namespace O::Comb

#endif // UTILS_COMB_RANGES_HPP
