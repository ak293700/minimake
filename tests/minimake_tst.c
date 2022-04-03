#include <criterion/criterion.h>

#include "../src/h_file/Variable.h"

Test(Easy, add)
{
    Variable *var = Variable_get(NULL, NULL, 10);
    cr_assert_eq(var, NULL);
}
