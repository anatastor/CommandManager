
#include <stdio.h>
#include <stdlib.h>

#include "cmdmanager.h"


int test_func (cmd_arg *args)
{
    printf("\nThis is a test\n");
    printf("\nmsg: %s\n", args[0].s);
    printf ("\nmsg2: %s\n", args[1].s);
    return 0;
}


int baum (cmd_arg *args)
{
    printf("\nBaum\n");
    printf("\n%s\n", args[0].s);
    printf("\n%i\n", args[1].i);
}


int
main (int argc, char **argv)
{   
    cmd list[] = {
        {0, "test", test_func, "ss", "help"},
        {0, "baum", baum, "si", "help"}
    };
    // define the commands
    
    cmd_table *table = cmd_table_create (5);
    cmd_table_set (table, &list[0]); // add the function to the hashtable
    cmd_table_set (table, &list[1]); 

    char *input = calloc (255, sizeof(char));
    printf("\nPlease enter a function:");
    fgets(input, 255, stdin);

    cmd_execute (table, input); // execute the given command (input contains also the arguments as string)


    cmd_table_free (&table);

    return EXIT_SUCCESS;
}
