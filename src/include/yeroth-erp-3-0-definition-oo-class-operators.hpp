/*
 * yeroth-erp-3-0-definition-oo-class-operators.hpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

# ifndef _YEROTH_ERP_3_0_DEFINITION_OO_CLASS_OPERATORS_HPP_
# define _YEROTH_ERP_3_0_DEFINITION_OO_CLASS_OPERATORS_HPP_


#include <cstdlib>


# define YEROTH_CLASS_OPERATORS  \
    inline void *operator new[](size_t size){ return calloc(1, size); } \
    inline void *operator new(size_t size){ return calloc(1, size); } \
    inline void operator delete[](void *ptr){ free(ptr); } \
    inline void operator delete[](void *ptr, size_t){ free(ptr); } \
    inline void operator delete(void *ptr){ free(ptr); } \
    inline void operator delete(void *ptr, size_t){ free(ptr); }
# endif //_YEROTH_ERP_3_0_DEFINITION_OO_CLASS_OPERATORS_HPP_
