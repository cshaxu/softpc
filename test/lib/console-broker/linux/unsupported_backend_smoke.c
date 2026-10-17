#include "lib/types/test.h"
#include "lib/console-broker/linux/console.c"
int main(void)
{
    console_broker_backend *backend = (void *)1;
    lib_test_assert(console_broker_backend_create(&backend) == LIB_STATUS_UNSUPPORTED && !backend);
    lib_test_assert(console_broker_backend_request_cooked_line(LIB_NULL) == LIB_STATUS_UNSUPPORTED);
    return 0;
}
