#ifndef UTILS_UNOWNED_PTR_H
#define UTILS_UNOWNED_PTR_H

#include <cassert>
#include <type_traits>
#include <utility>

namespace O
{

	#ifndef NDEBUG

		template <class T>
		class Unowned_Ptr
		{
			static_assert(!std::is_void<T>::value, "Unowned_Ptr<T> requires non-void T");
		public:
			// default construction
			constexpr Unowned_Ptr() noexcept : m_ptr(nullptr) {}
			constexpr Unowned_Ptr(nullptr_t) noexcept : m_ptr(nullptr) {}
			// construct from raw pointer
			constexpr Unowned_Ptr(T* p) noexcept : m_ptr(p) {}
			// allow construction from other Unowned_Ptr<U> if convertible
			template <class U, typename = std::enable_if_t<std::is_convertible<U*, T*>::value>>
			constexpr Unowned_Ptr(const Unowned_Ptr<U>& other) noexcept : m_ptr(other.get()) {}

			// default copy/move
			Unowned_Ptr(const Unowned_Ptr&) = default;
			Unowned_Ptr& operator=(const Unowned_Ptr&) = default;
			Unowned_Ptr(Unowned_Ptr&&) = default;
			Unowned_Ptr& operator=(Unowned_Ptr&&) = default;

			// assign from raw pointer
			Unowned_Ptr& operator=(T* p) noexcept { m_ptr = p; return *this; }

			// checks on deref
			constexpr T* operator->() const
			{
				assert(m_ptr != nullptr && "Unowned_Ptr::operator->() called on null pointer");
				return m_ptr;
			}

			constexpr T& operator*() const
			{
				assert(m_ptr != nullptr && "Unowned_Ptr::operator*() called on null pointer");
				return *m_ptr;
			}

			// implicit conversion to raw pointer (convenience)
			constexpr operator T*() const noexcept { return m_ptr; }
		private:
			T* m_ptr;
		};

	#else // NDEBUG 

		template <class T>
		using Unowned_Ptr = T*;

	#endif // NDEBUG
}

#endif UTILS_UNOWNED_PTR_H