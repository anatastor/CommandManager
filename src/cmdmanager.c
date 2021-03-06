
#include "cmdmanager.h"


cmd_table *
cmd_table_create (const int size)
{
    cmd_table *table = calloc (1, sizeof(cmd_table));
    if (table)
        cmd_table_init (table, size);

    return table;
}


void
cmd_table_init (cmd_table *table, const int size)
{
    table->size = size;
    table->entries = calloc (size, sizeof(cmd_table_entry));
    if (!table->entries)
        exit(1);
}


void
cmd_table_free_entry (struct cmd_table_entry *entry)
{
    if (entry->next)
        cmd_table_free_entry (entry->next);
    
    if (entry->command->dynamic)
    {
        free(entry->command->name);
        free(entry->command->args);
        free(entry->command->doc);
        free(entry->command);
    }

    free(entry);
}


void
cmd_table_free (cmd_table **table)
{   
    for (int i = 0; i < (*table)->size; i++)
    {
        if ((*table)->entries[i])
        {
            cmd_table_free_entry ((*table)->entries[i]);
            (*table)->entries[i] = NULL;
        }
    }

    free((*table)->entries);
    free(*table);
    *table = NULL;
}


int
cmd_table_hash (cmd_table *table, char *key)
{
    if (!table || !key)
        return -1;

    unsigned long int hash = 0;
    int i = 0;
    int size = strlen (key);

    while (hash < ULONG_MAX && i < size)
    {   
        hash += key[i];
        hash << 8;
        i++;
    }

    return hash % table->size;
}


struct cmd_table_entry *
cmd_table_entry_create (cmd *command)
{
    struct cmd_table_entry *entry = malloc (sizeof(cmd_table_entry));
    if (entry)
    {
        entry->command = command;
        entry->next = NULL;
    }

    return entry;
}


int
cmd_table_set (cmd_table *table, cmd *command)
{   
    if (! table || ! command)
        return 0;

    int hash = cmd_table_hash (table, command->name);
    if (! table->entries[hash])
    {
        table->entries[hash] = cmd_table_entry_create (command);
    }

    struct cmd_table_entry *entry = table->entries[hash];
    while (entry->next && strcmp (command->name, entry->command->name) != 0)
        entry = entry->next;

    entry->next = cmd_table_entry_create (command);

    return 1;
}


cmd *
cmd_table_get (cmd_table *table, char *key)
{   
    int hash = cmd_table_hash (table, key);
    struct cmd_table_entry *entry = NULL;
    entry = table->entries[hash];

    if (entry)
    {
        while (entry->next && strcmp(entry->command->name, key) != 0)
        {
            entry = entry->next; 
        }

        if (strcmp(entry->command->name, key) == 0)
        {
            return entry->command;
        }
    }
    return NULL;
}


void
cmd_parser_get_arg_string (cmd_arg *arg)
{   
    char delim = '\"';
    char *token = strtok (NULL, &delim);
    token = strtok (NULL, &delim);

    arg->s = token;
}


char *
cmd_parser_get_token (const char args)
{   
    char *token = NULL;

    if (args == 's')
        token = strtok (NULL, "\"");
    else
        token = strtok (NULL, DELIM);

    return token;
}


cmd_arg *
cmd_parser_get_args (const char *args_s)
{   
    int i = 0;
    int size = strlen (args_s);
    cmd_arg *args = calloc (size, sizeof(cmd_arg));

    for (int i = 0; i < size; i++)
    {
        if (args_s[i] == 'v')
            continue;


        char *token = cmd_parser_get_token (args_s[i]);
        if (token == NULL)
            break;

        switch (args_s[i])
        {
            case 'i':
                args[i].i = atoi (token);
                break;

            case 'd':
                args[i].d = atof (token);
                break;

            case 'c':
                args[i].c = token[0];
                break;

            case 's':
                args[i].s = token;
                strtok (NULL, "\"");
                break;

            case 'v':
                continue;
                break;

            default:
                continue;
                break;
        }
    }

    return args;
}


int
cmd_execute (cmd_table *table, char *icmd) // icmd = input_command
{
    char *cmd_name = strtok (icmd, DELIM);
    cmd *command = cmd_table_get (table, cmd_name);

    if (! command)
        return -1;

    cmd_arg *args = cmd_parser_get_args (command->args);
    int ret = command->func (args);
    free(args);
    free(icmd);
    
    return ret;
}
