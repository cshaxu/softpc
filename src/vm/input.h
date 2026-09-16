#ifndef VM_INPUT_H
#define VM_INPUT_H
#include "vm/machine.h"
#include "lib/kvm-base/event_interface.h"
int vm_keyboard_inject_machine_event(softpc_machine *machine,
    const kvm_input_event *event);
#endif
