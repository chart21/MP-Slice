#include "config.h"
#include "searchMain_template.hpp"

int main(int argc, char *argv[])
{
#if PROCESS_NUM > 1


pid_t child_pid, wpid;
int status = 0;

//Father code (before child processes start)

for (int id=0; id<PROCESS_NUM; id++) {
    if ((child_pid = fork()) == 0) {
        base_port += PROCESS_NUM*(PROCESS_NUM-1)*id;
        search_main(argc, argv, child_pid, PROCESS_NUM); //child code
        exit(0);
    }
}


while ((wpid = wait(&status)) > 0); // this way, the father waits for all the child processes 

//Father code (After all child processes end)
#else
    search_main(argc, argv, 0, 1); //child code
#endif
return 0;
}
