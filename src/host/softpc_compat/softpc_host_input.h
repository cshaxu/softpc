#ifndef SOFTPC_HOST_INPUT_H
#define SOFTPC_HOST_INPUT_H

/*
 * Standalone host input boundary.
 *
 * The original nt_keycd host algorithm owns the Scan-1-to-SoftPC-key-number
 * tables.  Runtime code may use this narrow translation endpoint, but must
 * not inspect the keyboard controller's private tables or state.
 */
int softpc_host_scan1_to_key(unsigned int scan_code);

#endif /* SOFTPC_HOST_INPUT_H */
