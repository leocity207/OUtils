#ifndef UTILS_COMB_RANGES_H
#define UTILS_COMB_RANGES_H

#include <ranges>
#include <cstddef>
#include <type_traits>
#include <iterator>

namespace O::Comb {

	/**
	 * @brief A strided view selecting elements at fixed compile-time intervals.
	 *
	 * The view exposes elements at indices:
	 *   Shift, Shift + Width, Shift + 2*Width, ...
	 *
	 * @tparam Width  Stride between selected elements (must be > 0)
	 * @tparam Shift  Initial offset (0 <= Shift < Width)
	 * @tparam V      Underlying range type
	 */
	template <std::size_t Width, std::size_t Shift, std::ranges::forward_range V>
	requires (Width > 0) && (Shift < Width) && std::ranges::viewable_range<V>
	class View : public std::ranges::view_interface<View<Width, Shift, V>> {
	private:
		std::views::all_t<V> m_container;

		template <typename BaseIt>
		class Iterator_Impl
		{
		public:
			using value_type = std::iter_value_t<BaseIt>;
			using reference = std::iter_reference_t<BaseIt>;
			using difference_type = std::iter_difference_t<BaseIt>;
			using iterator_concept = std::conditional_t<
				std::random_access_iterator<BaseIt>,
				std::random_access_iterator_tag,
				std::forward_iterator_tag>;

			Iterator_Impl() = default;
			Iterator_Impl(BaseIt it, BaseIt end) noexcept;

			reference operator*() const noexcept;

			Iterator_Impl& operator++() noexcept;
			Iterator_Impl operator++(int) noexcept;

			bool operator==(const Iterator_Impl& other) const noexcept;
			bool operator!=(const Iterator_Impl& other) const noexcept;

			// Random access enhancements. Use a different template parameter name (It)
			template <typename It = BaseIt>
				requires std::random_access_iterator<It>
			Iterator_Impl& operator+=(difference_type n) noexcept;

			template <typename It = BaseIt>
				requires std::random_access_iterator<It>
			difference_type operator-(const Iterator_Impl& other) const noexcept;

			template <typename It = BaseIt>
				requires std::random_access_iterator<It>
			reference operator[](difference_type n) const noexcept;

		private:
			BaseIt m_it{};
			BaseIt m_end{};
		};

	public:
		using iterator = Iterator_Impl<std::ranges::iterator_t<V>>;
		using const_iterator = Iterator_Impl<std::ranges::iterator_t<const V>>;

		View() = default;
		explicit View(V base);

		iterator begin();
		iterator end();

		const_iterator begin() const requires std::ranges::forward_range<const V>;
		const_iterator end() const requires std::ranges::forward_range<const V>;

		std::size_t size() const requires std::ranges::sized_range<V>;
	};

	/**
	 * Factory helper (deduces range type).
	 */
	template <std::size_t Width, std::size_t Shift, std::ranges::viewable_range R>
	View<Width, Shift, R> Make_Comb(R&& r);

	/**
	 * Convenience view for even indices.
	 */
	template <std::ranges::viewable_range R>
	View<2, 0, R> Even(R&& r);

	/**
	 * Convenience view for odd indices.
	 */
	template <std::ranges::viewable_range R>
	View<2, 1, R> Odd(R&& r);

} // namespace O::Comb

#include "comb.hpp"

#endif // UTILS_COMB_RANGES_H