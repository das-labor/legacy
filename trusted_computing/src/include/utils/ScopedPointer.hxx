#ifndef ScopedPointer_hxx_included
#define ScopedPointer_hxx_included

#include <stdexcept>
#include <TypeInfo.hxx>

namespace sirrix {
    namespace utils {
        /**
         * @brief Exception thrown if it is tried to dereference a null pointer.
         */
        class NullPointerException : public std::runtime_error {
        public:
            inline NullPointerException( const std::string &what );
        };

        /**
         * @brief Template helping to automatically delete heap objects.
         *
         * A ScopedPointer of type T acts as a replacement for T*, a pointer
         * to T. In constrast to a pointer, a ScopedPointer automatically
         * deletes the object during execution of the SmartPointers
         * destructor. This allow, e.g., to automatically delete a heap
         * object when an exception is thrown or if the parent class is
         * deleted.
         *
         * The reference operators, dereference operators, and the cast
         * operators ensure that the object behaves similar to the real
         * pointer, except that references and dereferences of null
         * pointers result in a thrown NullPointerException.
         *
         * Example:
         * <pre>
         *    void scopedPointerDemo() {
         *        ScopedPointer<TYPE> myInt( new TYPE );
         *
         *        TYPE *var = myInt;
         *
         *        (*myInt).doSomething();
         *        myInt->doSomething();
         *    } // automatically deletion of the myInt object.
         * </pre>
         *
         * @note See unit test file ScopedPointerTests.cxx for more examples on how
         *       to use this template class.
         *
         * @warning Please note that the main and only task of a ScopedPointer is to
         *          destruct an object at the end of the scope of the ScopedPointer.
         *          That is, an existing normal pointer becomes invalid, so use
         *          the assignment operator and the cast operator with caution.
         *
         * @note    If you need a more complex smart pointer supporting copying, use
         *          the SharedPointer class.
         *
         * @author Christian Stueble (stueble@sirrix.com)
         */
         template<typename TYPE>
         class ScopedPointer {
         public:
            /// Create a new auto ptr based on the given pointer.
            inline ScopedPointer( TYPE *ptr=0 );
            /// Deletes the managed object.
            inline ~ScopedPointer();

            /// Assigns a new object to this class. Please note that
            /// the old object is not deleted.
            inline ScopedPointer<TYPE>& operator=( TYPE *ptr );

            /// @return Reference operator to the managed object.
            inline TYPE* operator->();
            /// @return Const reference oprator to the managed object.
            inline const TYPE* operator->() const;

            /// @return Dereference operator to the managed object.
            inline TYPE& operator*();
            /// @return Const dereference oprator to the managed object.
            inline const TYPE& operator*() const;

            /// Casts this class into type @c TYPE*
            inline operator TYPE* ();
            /// Casts this class into type @c TYPE*
            inline operator const TYPE* () const;

         private:
            ScopedPointer( const ScopedPointer<TYPE> & );         /// disable copying
            ScopedPointer& operator=( ScopedPointer<TYPE> & );    /// disable copying

            TYPE *myObj;
         };

         // -------------- inline implementations ------------------
         NullPointerException::NullPointerException( const std::string &what ) :
            std::runtime_error( what )
         {}

         template<typename TYPE>
         ScopedPointer<TYPE>::ScopedPointer( TYPE *ptr ) :
            myObj( ptr )
         {}
         template<typename TYPE>
         ScopedPointer<TYPE>::~ScopedPointer() {
            delete myObj;
         }
         template<typename TYPE>
         ScopedPointer<TYPE>& ScopedPointer<TYPE>::operator=( TYPE *ptr ) {
            myObj = ptr;
            return *this;
         }
         template<typename TYPE>
         TYPE* ScopedPointer<TYPE>::operator->() {
            if ( myObj==0 )
                throw NullPointerException( typeInfo( *this ).getType() );
            return myObj;
         }
         template<typename TYPE>
         const TYPE* ScopedPointer<TYPE>::operator->() const {
            if ( myObj==0 )
                throw NullPointerException( typeInfo( *this ).getType() );
            return myObj;
         }
         template<typename TYPE>
         TYPE& ScopedPointer<TYPE>::operator*() {
            if ( myObj==0 )
                throw NullPointerException( typeInfo( *this ).getType() );
            return *myObj;
         }
         template<typename TYPE>
         const TYPE& ScopedPointer<TYPE>::operator*() const {
            if ( myObj==0 )
                throw NullPointerException( typeInfo( *this ).getType() );
            return *myObj;
         }
         template<typename TYPE>
         ScopedPointer<TYPE>::operator TYPE* () {
            return myObj;
         }
         template<typename TYPE>
         ScopedPointer<TYPE>::operator const TYPE* () const {
            return myObj;
         }
    }
}

#endif /*AUTOPTR_HXX_*/
