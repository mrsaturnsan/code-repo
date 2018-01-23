/******************************************************************************/
/*
* @file   unique_ptr.h
* @author Aditya Harsh
* @brief  Unique pointer implementation
*/
/******************************************************************************/

#pragma once

namespace atd
{
    template <typename T>
    class unique_ptr
    {
    public:
        /**
         * @brief Constructor.
         * 
         */
        explicit unique_ptr(T* data = nullptr) noexcept : data_(data) {}

        /**
         * @brief Destructor.
         * 
         */
        ~unique_ptr() noexcept
        {
            delete data_;
        }

        /**
         * @brief Move constructor.
         * 
         * @param rhs 
         */
        unique_ptr(unique_ptr<T>&& rhs) noexcept
        {
            data_ = std::exchange(rhs.data_, nullptr);
        }

        /**
         * @brief Move assignment.
         * 
         * @param rhs 
         * @return unique_ptr<T>& 
         */
        unique_ptr<T>& operator=(unique_ptr<T>&& rhs) noexcept
        {
            swap(unique_ptr<T>(std::move(rhs)));
        }

        /**
         * @brief Swaps two unique pointers.
         * 
         * @param rhs 
         */
        void swap(unique_ptr<T>& rhs) noexcept
        {
            std::swap(data_, rhs.data_);
        }

        /**
         * @brief Gets the raw pointer.
         * 
         * @return T* get 
         */
        T* get() noexcept
        {
            return data_;
        }

        /**
         * @brief Releases the pointer.
         * 
         */
        void release() noexcept
        {
            data_ = nullptr;
        }

    private:
        unique_ptr(const unique_ptr<T>&) = delete;
        unique_ptr<T>& operator=(const unique_ptr<T>&) = delete;

        T* data_;
    };

    /**
     * @brief Makes a unique pointer.
     * 
     * @tparam T 
     * @tparam Args 
     * @param args 
     * @return unique_ptr<T> 
     */
    template <typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args)
    {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
