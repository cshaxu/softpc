#include <assert.h>
#include "softpc.new/base/system/quick_ev.c"

static void callback(long value) { (void)value; }
static int encode(Q_CALLBACK_FN function, unsigned long *id)
{
    assert(function == callback);
    *id = 1;
    return TRUE;
}

int main(void)
{
    t_q_event first = {0}, second = {0}, orphan = {0};
    t_q_event *table[HASH_SIZE] = {0};
    Q_EVENT_SNAPSHOT_ENTRY entries[2];
    unsigned long count, hash[HASH_SIZE];
    first.func = second.func = callback;
    first.handle = 1; second.handle = 17;
    first.next = first.next_free = &second;
    second.previous = &first;
    table[1] = &first;
    assert(snapshot_capture_list(&first, table, entries, 2, &count, hash, encode));
    assert(count == 2 && hash[1] == 1 && entries[0].hash_next_index == 2);
    assert(entries[1].hash_next_index == 0);
    /* A valid hash prefix followed by a node absent from the ordered list. */
    second.next_free = &orphan;
    assert(!snapshot_capture_list(&first, table, entries, 2, &count, hash, encode));
    table[1] = &orphan;
    assert(!snapshot_capture_list(NULL, table, entries, 2, &count, hash, encode));
    puts("SoftPC queue archive membership: OK");
    return 0;
}
