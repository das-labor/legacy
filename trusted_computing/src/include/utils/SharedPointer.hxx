#ifndef SharedPointer_hxx_included
#define SharedPointer_hxx_included

#include <stdexcept>
#include <ScopedPointer.hxx>
#include <Types.hxx>
#include <iostream>

namespace sirrix {
	namespace utils {
      /**
       * @brief A smart pointer implementation including reference counting.
       *
       * A SharedPointer of type T acts as a replacement for T*, a pointer
       * to T. In contrast to a pointer, a sharedPointer automatically
       * deletes the object during execution of the last SharedPointer
       * pointing to that object. Thus, the SharedPointer counts the number
       * of instances representing the same pointer.
       *
       * The reference operators, dereference operators, and the cast
       * operators ensure that the object behaves similar to the real
       * pointer, except that references and dereferences of null
       * pointers result in a thrown NullPointerException.
       *
       * Example:
       * <pre>
       *    void sharedPointerDemo() {
       *        SharedPointer<TYPE> myInt( new TYPE );
       *
       *        {
       *        	SharedPointer<TYPE> sharedObj( new TYPE );
       *          myInt = sharedObj;
       *		  }
       *        myInt->doSomething();
       *    } // automatically deletion of the myInt object.
       * </pre>
       *
       * @note See unit test file SharedPointerTests.cxx for more examples on how
       *       to use this template class.
       *
       * @author Christian Stueble (stueble@sirrix.com)
       */
      template<typename TYPE>
      class SharedPointer {
         template <typename N> friend class SharedPointer;

      public:
         /// Create a new smart pointer based on the given pointer.
         inline SharedPointer( TYPE *ptr=0 );
         /// Copy constructor.
         inline SharedPointer( const SharedPointer<TYPE> & );
         /// Copy constructor for derived shared pointers.
         template <typename N>
         inline SharedPointer<TYPE>( const SharedPointer<N> & );

         /// Deletes the managed object.
         inline ~SharedPointer();

         /// Assignment operator.
         inline SharedPointer<TYPE>& operator=( const SharedPointer<TYPE> & );

         /// Assigns a new object to this class. Please note that
         /// the old object is not deleted.
         inline SharedPointer<TYPE>& operator=( TYPE *ptr );

         /// @return Reference operator to the managed object.
         inline TYPE* operator->();
         /// @return Const reference oprator to the managed object.
         inline const TYPE* operator->() const;

         /// @return Dereference operator to the managed object.
         inline TYPE& operator*();
         /// @return Const dereference oprator to the managed object.
         inline const TYPE& operator*() const;

         /// Casts this class into type @c TYPE*.
         inline operator TYPE* ();
         /// Casts this class into type @c TYPE*.
         inline operator const TYPE* () const;

         /// @return the number of shared references.
         inline UInt32 getReferenceCount() const;

         /// @return TRUE, if pointer is null.
         inline bool isNull() const;

      private:
         inline void decrease();  ///< Decreases the reference counter.
         inline void increase();  ///< Increases the reference counter.

         TYPE   *myObj;
         UInt32 *myCounter;
      };

      // -------------- inline implementations ------------------
      template<typename TYPE>
      SharedPointer<TYPE>::SharedPointer( TYPE *ptr ) :
         myObj( ptr ),
         myCounter( new UInt32( 0 ) )
      {
         increase();
      }
      template<typename TYPE>
      template<typename N>
      SharedPointer<TYPE>::SharedPointer( const SharedPointer<N> &r ) :
         myObj( r.myObj ),
         myCounter( r.myCounter )
      {
         increase();
      }
      template<typename TYPE>
      SharedPointer<TYPE>::SharedPointer( const SharedPointer<TYPE> &r ) :
         myObj( r.myObj ),
         myCounter( r.myCounter )
      {
         increase();
      }
      template<typename TYPE>
      SharedPointer<TYPE>& SharedPointer<TYPE>::operator=( const SharedPointer<TYPE> &r ) {
         if (this == &r)
            return *this;

         decrease();
         myObj = r.myObj;
         myCounter = r.myCounter;
         increase();

         return *this;
      }
      template<typename TYPE>
      SharedPointer<TYPE>::~SharedPointer() {
         decrease();
      }
      template<typename TYPE>
      SharedPointer<TYPE>& SharedPointer<TYPE>::operator=( TYPE *ptr ) {
         if ( myObj == ptr )
            return *this;

         decrease();
         myObj = ptr;
         myCounter = new UInt32( 0 );
         return *this;
      }
      template<typename TYPE>
      void SharedPointer<TYPE>::decrease() {
         (*myCounter)--;
         if (*myCounter == 0) {
            delete myCounter;
            delete myObj;
         }
      }
      template<typename TYPE>
      void SharedPointer<TYPE>::increase() {
         (*myCounter)++;
      }
      template<typename TYPE>
      TYPE* SharedPointer<TYPE>::operator->() {
         if ( myObj==0 )
            throw NullPointerException( typeInfo( *this ).getType() );
         return myObj;
      }
      template<typename TYPE>
      const TYPE* SharedPointer<TYPE>::operator->() const {
         if ( myObj==0 )
            throw NullPointerException( typeInfo( *this ).getType() );
         return myObj;
      }
      template<typename TYPE>
      TYPE& SharedPointer<TYPE>::operator*() {
         if ( myObj==0 )
            throw NullPointerException( typeInfo( *this ).getType() );
         return *myObj;
      }
      template<typename TYPE>
      const TYPE& SharedPointer<TYPE>::operator*() const {
         if ( myObj==0 )
            throw NullPointerException( typeInfo( *this ).getType() );
         return *myObj;
      }
      template<typename TYPE>
      SharedPointer<TYPE>::operator TYPE* () {
         return myObj;
      }
      template<typename TYPE>
      SharedPointer<TYPE>::operator const TYPE* () const {
         return myObj;
      }
      template<typename TYPE>
      UInt32 SharedPointer<TYPE>::getReferenceCount() const {
         return *myCounter;
      }
      template<typename TYPE>
      bool SharedPointer<TYPE>::isNull() const {
         return myObj == 0;
      }
	}
}

#endif /*AUTOPTR_HXX_*/
