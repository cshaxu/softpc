#include <assert.h>

#include <windows.h>

#include "insignia.h"
#include "timestmp.h"

int main(void)
{
    QTIMESTAMP before;
    QTIMESTAMP after;
    IUH elapsed;

    host_q_write_timestamp(&before);
    Sleep(20u);
    host_q_write_timestamp(&after);
    elapsed = host_q_timestamp_diff(&before, &after);

    /* The original quick-event contract measures elapsed wall time in
       microseconds.  Leave broad scheduler headroom while rejecting the old
       process-time implementation, which reports approximately zero here. */
    assert(elapsed >= 5000u);
    assert(elapsed < 1000000u);
    return 0;
}
