// Copyright (C) 2007, 2008 by Sirrix AG security technologies
// Copyright (C) 2010 by Philipp Deppenwiese
//
// This file is part of the microTSS framework. The microTSS 
// framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//

#ifndef SINGLETON_HXX
#define SINGLETON_HXX

namespace sirrix {
    namespace utils {
        /** 
         * @brief       Template to create a Singleton.
         * @author      Christian Stueble (stueble@sirrix.com)
         *
         * A simple template class to make a singleton from
         * an ordinary class. This class ensures that the
         * singleton object is destroyed, but it cannot give
         * any guarantees on the ordering of destructions. 
         * Therefore you have to be careful if your singleton
         * instance accesses other singletons.
         * 
         * This singleton implementation also allows to set the
         * object to be used as singleton using setInstance().
         * 
         * Usage: If you have a class @c MyClass, you can use it
         * as a singleton with Singleton<MyClass>::getInstance().
         * 
         * @note It is better to create a normal class and make
         *       a singleton from it using this template, since
         *       this allows to create other instances of that
         *       object for, e.g., testing purposes.
         */
        template <class TYPE, class DEFAULT=TYPE> class Singleton {
        public:
            /// @return Reference to the protected singleton object.
            inline static TYPE& getInstance();
          
            /// Sets a new object derived from @c TYPE as the new instance.
            inline static void setInstance( TYPE *instance );
            
        private:
            Singleton();                               ///< Prevents direct instantiation of the singleton.
            Singleton( const Singleton& );             ///< Prevents copying of the singleton.
            
            inline static TYPE& getDefaultInstance();  ///< Returns the default singleton instance.
            
            static TYPE *myInstance;                   ///< Points to the current singleton instance.
        };
    
        /**
         * @example SingletonExample.cxx
         *
         * This is an example on how to use the singleton template
         * Singleton to make a singleton from an arbitrary class.
         */
         
        // -------------------- inline implementation ---------------------
        template <class TYPE, class DEFAULT> 
        TYPE* Singleton<TYPE,DEFAULT>::myInstance = 0;
        
        template <class TYPE, class DEFAULT> 
        TYPE& Singleton<TYPE,DEFAULT>::getInstance() {
            if (myInstance == 0)
                myInstance = &getDefaultInstance();
                
            return *myInstance;
        }
        
        template <class TYPE, class DEFAULT> 
        void Singleton<TYPE,DEFAULT>::setInstance( TYPE *instance ) {
            myInstance = instance;   
        }
        
        template <class TYPE, class DEFAULT> 
        TYPE& Singleton<TYPE,DEFAULT>::getDefaultInstance() {
            static DEFAULT object;
            return object;
        }
    }
}

#endif
