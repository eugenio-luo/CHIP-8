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
        
        if (sys_init())
                dbg_err("[main.c: main] system init failed");

#ifdef TEST
        dbg_test_init();
#endif
        
        sys_load(argv[1]);

#ifdef TEST
        dbg_test_load();
#endif

        sys_cycle();
}
