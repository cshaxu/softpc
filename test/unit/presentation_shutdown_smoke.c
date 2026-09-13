/* Exercise the real application terminal boundary in a child process, linking
 * the real app collaborators without starting a machine or reading media. */
#include "app/presentation.c"
#include <assert.h>

int main(int argc,char **argv)
{
    (void)argv;
    if(argc>1) {
        softpc_host_require_status(LIB_STATUS_IO_ERROR, "KVM destroy");
        return 99;
    }
    softpc_host_require_status(LIB_STATUS_OK, "KVM destroy");
    char executable[MAX_PATH], command[MAX_PATH+32];
    STARTUPINFOA startup={.cb=sizeof(startup)};
    PROCESS_INFORMATION child={0};
    DWORD exit_code;
    assert(GetModuleFileNameA(NULL,executable,sizeof(executable)));
    snprintf(command,sizeof(command),"\"%s\" --fail",executable);
    assert(CreateProcessA(NULL,command,NULL,NULL,FALSE,CREATE_NO_WINDOW,NULL,NULL,&startup,&child));
    assert(WaitForSingleObject(child.hProcess,3000)==WAIT_OBJECT_0);
    assert(GetExitCodeProcess(child.hProcess,&exit_code) && exit_code==EXIT_FAILURE);
    CloseHandle(child.hThread);CloseHandle(child.hProcess);
    return 0;
}
