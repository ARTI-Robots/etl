///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef __etl_iqueue__
#define __etl_iqueue__

#include <cstddef>

#include "queue_base.h"

namespace etl
{
  //***************************************************************************
  ///\ingroup queue
  ///\brief This is the base for all queues that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived queue.
  ///\code
  /// etl::queue<int, 10> myQueue;
  /// etl::iqueue<int>& iQueue = myQueue;
  ///\endcode
  /// \warning This queue cannot be used for concurrent access from multiple threads.
  /// \tparam T The type of item that the queue holds.
  //***************************************************************************
  template <typename T>
  class iqueue : public queue_base
  {
  public:

    typedef queue_base::size_type size_type;       ///< The type used for determining the size of queue.
    typedef T                     value_type;      ///< The type stored in the queue.
    typedef T&                    reference;       ///< A reference to the type used in the queue.
    typedef const T&              const_reference; ///< A const reference to the type used in the queue.
    typedef T*                    pointer;         ///< A pointer to the type used in the queue.
    typedef const T*              const_pointer;   ///< A const pointer to the type used in the queue.

    //*************************************************************************
    /// Adds an item to the queue.
    /// If ETL_USE_EXCEPTIONS is defined, throws an etl::queue_full_exception is the queue is already full,
    /// otherwise does nothing if full.
    ///\param item The item to push to the queue.
    //*************************************************************************
    void push(const_reference item)
    {
      if (!full())
      {
        buffer[in++] = item;
        in = (in == (MAX_SIZE - 1)) ? 0 : ++in;
        ++size;
      }
#ifdef ETL_USE_EXCEPTIONS
      else
      {
        throw queue_full_exception();
      }
#endif
    }

    //*************************************************************************
    /// Allows a possibly more efficient 'push' by moving to the next input item
    /// and returning a reference to it.
    /// This may eliminate a copy by allowing direct construction in-place.<br>
    /// If ETL_USE_EXCEPTIONS is defined, throws an etl::queue_full_exception is the queue is already full,
    /// otherwise does nothing if full.
    /// \return A reference to the position to 'push' to.
    //*************************************************************************
    reference push()
    {
      const size_type next = in;

      if (!full())
      {
        in = (in == (MAX_SIZE - 1)) ? 0 : ++in;
        ++size;
      }
#ifdef ETL_USE_EXCEPTIONS
      else
      {
        throw queue_full_exception();
      }
#endif

      return buffer[next];
    }

    //*************************************************************************
    /// Gets a reference to the item at the front of the queue.<br>
    /// If ETL_USE_EXCEPTIONS is defined, throws an etl::queue_empty_exception if the queue is empty.<br>
    /// If ETL_USE_EXCEPTIONS is not defined and the queue is empty, the return value is undefined.
    /// \return A reference to the item at the front of the queue.
    //*************************************************************************
    reference front()
    {
#ifdef ETL_USE_EXCEPTIONS
      if (empty())
      {
        throw queue_empty_exception();
      }
#endif

      return buffer[out];
    }

    //*************************************************************************
    /// Gets a const reference to the item at the front of the queue.<br>
    /// If ETL_USE_EXCEPTIONS is defined, throws an etl::queue_empty_exception if the queue is empty.<br>
    /// If ETL_USE_EXCEPTIONS is not defined and the queue is empty, the return value is undefined.
    /// \return A const reference to the item at the front of the queue.
    //*************************************************************************
    const_reference front() const
    {
#ifdef ETL_USE_EXCEPTIONS
      if (empty())
      {
        throw queue_empty_exception();
      }
#endif

      return buffer[out];
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    queue(T* buffer, size_type max_size)
      : queue_base(max_size),
        buffer(buffer)
    {
    }

  private:

    T* buffer; ///< The internal buffer.
  };
}
