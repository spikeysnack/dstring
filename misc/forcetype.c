/* forcetype.c */
/* gcc -o forcetype forcetype.c */

/** forced typing macro makes types 
 * appear to compiler as selected type
 *
 * 2 gcc macros that select one of two expressions.
 * 
 *  __builtin_choose_expr( exp1, exp2)  returns exp1 if exp1 is true otherwise exp2
 *   In the below example  the expressions are chained  as in:
 *    __builtin_choose_expr( funcA , __builtin_choose_expr( FuncB , C))
 * so you get funcA OR (funcB OR FuncC) the first one that = true.
 *
 *  __builtin_types_compatible_p(type, type)  returns true or false if
 *  the first type is compatible with the second. This allows checking the
 *  type of the argument against another known type. 
 *  These are hacks, and probably are not what programmers want to be doing.
 */

#define _GNU_SOURCE 
#include <stdlib.h>
#include <stdio.h>

struct record; 
const char* int_to_string(int i);
const char* float_to_string(float f);
const char* double_to_string(double d);
const char* size_t_to_string(size_t sz);
const char* record_to_string(struct record* r);

/* make the compiler treat x as the given type 
 * no matter what
 */
#define FORCETYPE(x, type) *(type *)(__typeof__(x) []){ x }

   
/* filter down x though type filter 
 * and perform the selected
 * function for x's type 
 */  
#define STRINGIFY(x)                                              \
  __builtin_choose_expr(                                          \
   __builtin_types_compatible_p(__typeof__(x),int),               \
    int_to_string(FORCETYPE(x, int)),                             \
                                                                  \
   __builtin_choose_expr(                                         \
   __builtin_types_compatible_p(__typeof__(x), float),            \
   float_to_string(FORCETYPE(x, float)),			  \
                                                                  \
   __builtin_choose_expr(                                         \
   __builtin_types_compatible_p(__typeof__(x), double),           \
   double_to_string(FORCETYPE(x, double)),			  \
                                                                  \
   __builtin_choose_expr(					  \
   __builtin_types_compatible_p(__typeof__(x), size_t),           \
   size_t_to_string(FORCETYPE(x, size_t)),			  \
                                                                  \
   __builtin_choose_expr(					  \
   __builtin_types_compatible_p(__typeof__(x), struct record*),   \
   record_to_string(FORCETYPE(x, struct record*)),		  \
   x                                                              \
)))))


/* test to see if function selection works*/
int main( int argc, const char*** argv, const char*** env)
{
  int x = 1;

  float y = FORCETYPE(x, float);
  const char* str = STRINGIFY(y);
  fprintf(stdout , "F:\t%s\n" , str);


  double dd = FORCETYPE(x, double);
  str = STRINGIFY(dd);
  fprintf(stdout , "F:\t%s\n" , str);

  exit(EXIT_SUCCESS);
}


/* these return their function name stringified*/

const char* int_to_string(int i)
{
  return __FUNCTION__;
}

const char* float_to_string(float f)
{
  return __FUNCTION__;
}

const char* double_to_string(double d)
{
  return __FUNCTION__;
}

const char* size_t_to_string(size_t sz)
{
  return __FUNCTION__;
}

const char* record_to_string(struct record* r)
{
  return __FUNCTION__;
}






