#include "debug.h"
#include "system.h"

int
main(int argc, char **argv)
{
        if (argc < 2)
                dbg_err("[main.c: main] missing name of program to load");

#ifdef TEST
        dbg_test();
#endif
        
        if (sys_reset())
                dbg_err("[main.c: main] system init failed");

#ifdef TEST
        
        dbg_test_init();
        dbg_test_inst();
        (void) argv;

#else
        
        sys_load(argv[1]);
        sys_cycle();

#endif
}
