#ifndef UTILS_ZIP_HPP
#define UTILS_ZIP_HPP

#include <utility>
#include <cassert>
#include <tuple>
#include <ranges>


namespace O
{

	// ----------------------------- Zip_View -----------------------------
	template <std::ranges::random_access_range R1, std::ranges::random_access_range R2>
	requires std::ranges::sized_range<R1> && std::ranges::sized_range<R2>
	class Zip_View 
	{
	public:
		using Size_Type = std::size_t;
		using ref1_t = std::ranges::range_reference_t<R1>;
		using ref2_t = std::ranges::range_reference_t<R2>;
		using elem1_t = std::remove_reference_t<ref1_t>;
		using elem2_t = std::remove_reference_t<ref2_t>;

	private:
		R1* m_r1;
		R2* m_r2;
		Size_Type m_size;

	public:
		constexpr Zip_View(R1& a, R2& b) noexcept: 
			m_r1(std::addressof(a)),
			m_r2(std::addressof(b)),
			m_size(static_cast<Size_Type>(std::ranges::size(a)))
		{
			assert(std::ranges::size(a) == std::ranges::size(b) && "Zip requires ranges of equal size");
		}

		// proxy type that is tuple-like via ADL get + std::tuple_size specialization (below)
		struct Proxy {
			ref1_t first;
			ref2_t second;
		};

		// Iterator (random-access not required; simple index Iterator)
		struct Iterator {
				using Difference_Type = std::ptrdiff_t;
				using Pointer = void;

				R1* r1;
				R2* r2;
				Size_Type idx;
				Size_Type size;

			constexpr Iterator(R1* a, R2* b, Size_Type i, Size_Type s) noexcept:
				r1(a), 
				r2(b), 
				idx(i), 
				size(s) 
			{

			}

			constexpr Proxy operator*() const noexcept
			{
				return Proxy{ 
					std::ranges::range_reference_t<R1>( (*r1)[idx] ),
					std::ranges::range_reference_t<R2>( (*r2)[idx] )
				};
			}

			constexpr bool operator==(const Iterator& o) const noexcept { return idx == o.idx; }
			constexpr bool operator!=(const Iterator& o) const noexcept { return !(*this == o); }

			constexpr Iterator& operator++() noexcept
			{
				++idx;
				return *this;
			}
			constexpr Iterator operator++(int) noexcept
			{
				Iterator tmp = *this;
				++(*this);
				return tmp;
			}
		};

		constexpr Iterator begin() const noexcept { return Iterator(m_r1, m_r2, 0u, m_size); }
		constexpr Iterator end()   const noexcept { return Iterator(m_r1, m_r2, m_size, m_size); }
		constexpr Size_Type size() const noexcept { return m_size; }
	};

	template <std::ranges::random_access_range R>
	requires std::ranges::sized_range<R>
	class Zip_Index_View {
	public:
		using Size_Type = std::size_t;
		using Ref_T = std::ranges::range_reference_t<R>;
		using Elem_T = std::remove_reference_t<Ref_T>;

	private:
		R* m_r;
		Size_Type m_size;

	public:
		constexpr Zip_Index_View(R& r) noexcept : 
			m_r(std::addressof(r)),
			m_size(static_cast<Size_Type>(std::ranges::size(r))) 
		{

		}

		struct Proxy
		{
			Ref_T p;
			Size_Type idx;
		};

		struct Iterator {
			R* r;
			Size_Type idx;
			Size_Type size;

			constexpr Iterator(R* rr, Size_Type i, Size_Type s) noexcept : 
				r(rr), 
				idx(i), 
				size(s)
			{

			}

			constexpr bool operator==(const Iterator& o) const noexcept { return idx == o.idx; }
			constexpr bool operator!=(const Iterator& o) const noexcept { return !(*this == o); }

			constexpr Iterator& operator++() noexcept { ++idx; return *this; }
			constexpr Iterator operator++(int) noexcept { Iterator tmp = *this; ++(*this); return tmp; }

			constexpr Proxy operator*() const noexcept 
			{
				return Proxy{ 
					std::ranges::range_reference_t<R>( (*r)[idx] ),
					idx
				};
			}
		};

		constexpr Iterator begin() const noexcept { return Iterator(m_r, 0u, m_size); }
		constexpr Iterator end()   const noexcept { return Iterator(m_r, m_size, m_size); }
		constexpr Size_Type size() const noexcept { return m_size; }
	};

	template <std::ranges::random_access_range R>
	requires std::ranges::sized_range<R>
	class Zip_Adjacent_Circular_View {
	public:
		using Size_Type = std::size_t;
		using Ref_T = std::ranges::range_reference_t<R>;
		using Elem_T = std::remove_reference_t<Ref_T>;

	private:
		R* m_r;
		Size_Type m_size;

	public:
		constexpr Zip_Adjacent_Circular_View(R& r) noexcept : 
			m_r(std::addressof(r)),
			m_size(static_cast<Size_Type>(std::ranges::size(r))) 
		{

		}

		struct Proxy
		{
			Ref_T p;
			Ref_T p_1;
		};

		struct Iterator {
			R* r;
			Size_Type idx;
			Size_Type size;

			constexpr Iterator(R* rr, Size_Type i, Size_Type s) noexcept : 
				r(rr), 
				idx(i), 
				size(s)
			{

			}

			constexpr bool operator==(const Iterator& o) const noexcept { return idx == o.idx; }
			constexpr bool operator!=(const Iterator& o) const noexcept { return !(*this == o); }

			constexpr Iterator& operator++() noexcept { ++idx; return *this; }
			constexpr Iterator operator++(int) noexcept { Iterator tmp = *this; ++(*this); return tmp; }

			constexpr Proxy operator*() const noexcept 
			{
				return Proxy{ 
					std::ranges::range_reference_t<R>( (*r)[idx] ),
					std::ranges::range_reference_t<R>( (*r)[(idx+1)%size] )
				};
			}
		};

		constexpr Iterator begin() const noexcept { return Iterator(m_r, 0u, m_size); }
		constexpr Iterator end()   const noexcept { return Iterator(m_r, m_size, m_size); }
		constexpr Size_Type size() const noexcept { return m_size; }
	};



	template <std::ranges::forward_range Range, typename Func>
	requires std::invocable<Func&, std::ranges::range_reference_t<Range>, std::size_t>
	constexpr void For_Each_Indexed(Range&& range, Func&& func) noexcept(noexcept(std::declval<Func&>()(*std::ranges::begin(std::declval<Range&>()), std::size_t{})))
	{
		using std::begin;
		using std::end;
		using std::data;
		using std::size;
		constexpr bool is_contig = std::ranges::contiguous_range<Range>;

		if constexpr (is_contig) {
			// Use pointer arithmetic for best performance on contiguous ranges (std::array, std::vector, C arrays)
			auto* ptr = data(range);
			std::size_t n = static_cast<std::size_t>(size(range));
			for (std::size_t i = 0; i < n; ++i)
			{
				// pass reference to element (no copy)
				std::invoke(func, ptr[i], i);
			}
		} else {
			std::size_t i = 0;
			for (auto&& elem : range)
			{
				std::invoke(func, elem, i++);
			}
		}
	}

	template <std::ranges::forward_range RangeA, std::ranges::forward_range RangeB, typename Func>
	requires std::invocable<Func&, std::ranges::range_reference_t<RangeA>, std::ranges::range_reference_t<RangeB>>
	constexpr void For_Each_Zip(RangeA&& a, RangeB&& b, Func&& func) noexcept(noexcept(std::declval<Func&>()(*std::ranges::begin(std::declval<RangeA&>()), *std::ranges::begin(std::declval<RangeB&>()))))
	{
		using std::begin;
		using std::end;
		using std::data;
		using std::size;

		constexpr bool a_contig = std::ranges::contiguous_range<RangeA>;
		constexpr bool b_contig = std::ranges::contiguous_range<RangeB>;

		if constexpr (a_contig && b_contig) {
			// Fastest path: both contiguous => pointer loop
			auto* pa = data(a);
			auto* pb = data(b);
			std::size_t na = static_cast<std::size_t>(size(a));
			std::size_t nb = static_cast<std::size_t>(size(b));
			assert(na == nb && "for_each_zip: ranges must have same size (debug assert)");
			std::size_t n = (na < nb) ? na : nb;
			for (std::size_t i = 0; i < n; ++i) 
			{
				std::invoke(func, pa[i], pb[i]);
			}
		} 
		else 
		{
			// Generic iterator path
			auto itA = begin(a);
			auto itB = begin(b);
			auto endA = end(a);
			auto endB = end(b);
			for (; itA != endA && itB != endB; ++itA, ++itB)
			{
				std::invoke(func, *itA, *itB);
			}
		}
	}

} // namespace O



#endif //UTILS_ZIP_HPP
