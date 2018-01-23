/******************************************************************************/
/*
* @file   fastlist.h
* @author Aditya Harsh
* @brief  Fast linked-list implementation.
*/
/******************************************************************************/

#pragma once

#include <memory>

namespace atl
{
    // forward declare
    template <typename T>
    class ListIter;

    // allows multiple items to be passed into the constructor
    constexpr static struct list_initialization_t {}
    list_initialization;

    /**
     * @brief Linked list implementation.
     * 
     * @tparam T 
     */
    template <typename T>
    class FastList
    {
    public:
        /**
         * @brief Default constructor.
         * 
         */
        FastList() noexcept : head_(nullptr), tail_(nullptr), size_(0), last_(nullptr), last_index_(0) {}

        /**
         * @brief Allows for initializer construction
         * 
         * @param in_l 
         */
        template <typename... Args>
        FastList(list_initialization_t, Args&&... args) : FastList()
        {
                try
                {
                    (static_cast<void>(emplace_back(args)), ...);
                }
                catch(...)
                {
                    // clear existing nodes
                    clear();

                    // keep throwing
                    throw;
                }
        }

        /**
        * @brief Copy constructor.
        * 
        * @param rhs 
        */
        FastList(const FastList& rhs) 
        {
            // build the list
            Node* temp = rhs.head_;

            while (temp)
            {
                try
                {
                    emplace_back(temp->data_);
                }
                catch(...)
                {
                    // clear existing nodes
                    clear();

                    // keep throwing
                    throw;
                }
                
                temp = temp->next_;
            }
        }

        /**
         * @brief Move constructor.
         * 
         * @param rhs 
         */
        FastList(FastList&& rhs) noexcept
        {
            // build the list
            head_ = std::exchange(rhs.head_, nullptr);
            tail_ = std::exchange(rhs.tail_, nullptr);
            size_ = std::exchange(rhs.size_, 0);
        }

        /**
         * @brief Assignment
         * 
         * @param rhs 
         * @return FastList& 
         */
        FastList& operator=(const FastList& rhs)
        {
            // exit out early
            if (this == &rhs) return *this;

            // clear allocated nodes
            clear();

            // build the list
            Node* temp = rhs.head_;

            while (temp)
            {
                try
                {
                    emplace_back(temp->data_);
                }
                catch(...)
                {
                    // clear existing nodes
                    clear();

                    // keep throwing
                    throw;
                }
                
                temp = temp->next_;
            }

            return *this;
        }

        /**
         * @brief Assignment
         * 
         * @param rhs 
         * @return FastList& 
         */
        FastList& operator=(FastList&& rhs) noexcept
        {
            // exit out early
            if (this == &rhs) return *this;

            FastList tmp {std::move(rhs)};
            tmp.swap(*this);

            return *this;
        }

        /**
         * @brief Destructor clears the list.
         * 
         */
        ~FastList() noexcept
        {
            clear();
        }

        /**
         * @brief Clears the list.
         * 
         */
        void clear() noexcept
        {
            Node* current;

            while (head_)
            {
                current = head_;
                head_ = head_->next_;
                delete current;
            }

            // reset values
            head_ = nullptr;
            tail_ = nullptr;
            last_ = nullptr;
            size_ = 0;
            last_index_ = 0;
        }

        /**
         * @brief Emplaced data on back.
         * 
         * @param data 
         * @return T& 
         */
        template <typename... Args>
        T& emplace_back(Args&&... args)
        {
            // allocate the node
            std::unique_ptr<Node> node = std::make_unique<Node>(nullptr, std::forward<Args>(args)...);

            if (tail_)
            {
                tail_->next_ = node.get();
                tail_ = tail_->next_;
            }
            else
            {
                head_ = node.get();
                tail_ = head_;
            }

            // release the pointer
            node.release();

            // increment total size
            ++size_;

            // return reference to data
            return tail_->data_;
        }
        
        /**
         * @brief Emplaces data on front.
         * 
         * @param data 
         * @return T& 
         */
        template <typename... Args>
        T& emplace_front(Args&&... args)
        {
            // allocate the node
            std::unique_ptr<Node> node = std::make_unique<Node>(nullptr, std::forward<Args>(args)...);

            Node* front = node.get();
            head_ = front;

            // release the pointer
            node.release();

            // increment total size
            ++size_;

            // return reference to data
            return head_->data_;
        }

        /**
         * @brief Subscript operator overload.
         * 
         * @param index 
         * @return T& 
         */
        T& operator[] (unsigned index)
        {
            if (index >= size_)
                throw std::runtime_error("Invalid index");

            // prevents N^2 search on subsequent indexes
            if (last_)
            {
                if (index >= last_index_)
                {
                    // move to the correct index
                    while (last_index_ < index)
                    {
                        last_ = last_->next_;
                        ++last_index_;
                    }
                    return last_->data_;
                }
            }

            Node* temp = head_;
            
            // move to position
            for (unsigned i = 0; i < index; ++i)
                temp = temp->next_;

            // store last information
            last_ = temp;
            last_index_ = index;

            return temp->data_;
        }

        /**
         * @brief Removes from index.
         * 
         * @param index 
         */
        void remove(unsigned index)
        {
            if (index >= size_)
                throw std::runtime_error("Invalid index");

            --size_;

            Node* temp = head_;

            if (!index)
            {
                if (tail_ == head_)
                {
                    delete head_;
                    head_ = nullptr;
                    tail_ = nullptr;
                }
                else
                {
                    head_ = head_->next_;
                    delete temp;
                }
                return;
            }

            // use last index to increase speed
            if (last_ && (index > last_index_))
            {
                while (index > (last_index_ + 1))
                {
                    last_ = last_->next_;
                    ++last_index_;
                }

                Node* target = last_->next_;
                last_->next_ = target->next_;
                delete target;

                return;
            }
            
            // move to position
            for (unsigned i = 0; i < index - 1; ++i)
                temp = temp->next_;

            Node* target = temp->next_;
            temp->next_ = target->next_;

            if (target == tail_)
                tail_ = temp;

            // delete the node
            delete target;

            // set last values
            last_ = temp;
            last_index_ = index - 1;
        }

        /**
         * @brief Returns the front of the list. Undefined behavior if the list is empty.
         * 
         * @return T& 
         */
        T& front() noexcept
        {
            return head_->data_;
        }

        /**
         * @brief Removes from the front.
         * 
         */
        void pop_front()
        {
            remove(0);
        }

        /**
         * @brief Returns the back of the list. Undefined behavior if the list is empty.
         * 
         * @return T& 
         */
        T& back() noexcept
        {
            return tail_->data_;
        }

        /**
         * @brief Removes from the back.
         * 
         */
        void pop_back()
        {
            remove(size_ - 1);
        }

        /**
         * @brief Gets the size of the list.
         * 
         * @return unsigned size const 
         */
        unsigned size() const noexcept
        {
            return size_;
        }

        /**
         * @brief Gets whether or not the list is empty.
         * 
         * @return true 
         * @return false 
         */
        bool empty() const noexcept
        {
            return !size_;
        }

        /**
         * @brief Swaps two lists
         * 
         * @param other 
         */
        void swap(FastList& other) noexcept
        {
            head_ = std::exchange(other.head_, head_);
            tail_ = std::exchange(other.tail_, tail_);
            size_ = std::exchange(other.size_, size_);
        }

        /**
         * @brief Iterator implementation.
         * 
         * @return ListIter<T> begin
         */
        ListIter<T> begin() noexcept
        {
            return ListIter<T>(*this, 0);
        }

        /**
         * @brief Iterator implementation.
         * 
         * @return ListIter<T> end
         */
        ListIter<T> end() noexcept
        {
            return ListIter<T>(*this, size_);
        }

    private:
        // data struct
        struct Node
        {
            // constructor
            Node(Node* next, T data) : next_(next), data_(data) {}

            Node* next_;
            T data_;
        };

        // head of the list
        Node* head_;
        // tail of the list
        Node* tail_;
        // size of the list
        unsigned size_;

        // speeds up observing
        Node* last_;
        unsigned last_index_;
    };

    /**
     * @brief Iterator class for the list
     * 
     * @tparam T 
     */
    template <typename T>
    class ListIter
    {
    public:
        /**
         * @brief Constructor for the iterator
         * 
         * @param list 
         * @param index 
         */
        ListIter(FastList<T>& list, unsigned index = 0) noexcept : list_(list), index_(index) {}

        /**
         * @brief Checks for range.
         * 
         * @param rhs 
         * @return true 
         * @return false 
         */
        bool operator!=(const ListIter<T>& rhs) noexcept
        {
            return (index_ != rhs.index_);
        }

        /**
         * @brief Increments iterator.
         * 
         * @return const ListIter<T>& 
         */
        const ListIter<T>& operator++() noexcept
        {
            ++index_;
            return *this;
        }

        /**
         * @brief Dereference.
         * 
         * @return T& operator* 
         */
        T& operator*() noexcept
        {
            return list_[index_];
        }

    private:
        // Reference to the list being looked at.
        FastList<T>& list_;
        // Current index.
        unsigned index_;
    };
}
