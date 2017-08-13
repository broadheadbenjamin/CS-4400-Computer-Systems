/* This is the main file for the `whoosh` interpreter and the part
   that you modify. */

#include <stdlib.h>
#include <stdio.h>
#include "csapp.h"
#include "ast.h"
#include "fail.h"

static void run_script(script *scr);
static void run_group(script_group *group);
static void run_command(script_command *command);
static void set_var(script_var *var, int new_value);
static void print_script_info(script *scr);
static void print_script_group_info(script_group *group);

/* You probably shouldn't change main at all. */

int main(int argc, char **argv) {
  script *scr;
  
  if ((argc != 1) && (argc != 2)) {
    fprintf(stderr, "usage: %s [<script-file>]\n", argv[0]);
    exit(1);
  }

  scr = parse_script_file((argc > 1) ? argv[1] : NULL);

  run_script(scr);

  return 0;
}

static void run_script(script *scr) {
  printf("Made it here 0.");
  if (scr->num_groups == 1) {
    run_group(&scr->groups[0]);
  } 
  else {
    /* You'll have to make run_script do better than this */;
    for (int i = 0; i < scr->num_groups; i++){
      run_group(&scr->groups[i]);
    }
    //fail("only 1 group supported");
  }
}

static void run_group(script_group *group){
  printf("Made it here 1.");
  // Check for an invalid number of repeats
  if (group->repeats > 0){
    
    // Now loop through each repeat
    int i;
    for (i = 0; i < group->repeats; i++){
      if (group->result_to != NULL){
        
      }
      if (group->num_commands == 1){
        run_command(&group->commands[0]);
      }
      else {
        int j;
        for (j = 0; j < group->num_commands; i++){
          run_command(&group->commands[j]);
        }
      }
    }
  }
  else{
    fail("Invalid script. Repeats <= 0");
  }
}

/* This run_command function is a good start, but note that it runs
   the command as a replacement for the `whoosh` script, instead of
   creating a new process. */

static void run_command(script_command *command) {
  const char **argv;
  int i;

  argv = malloc(sizeof(char *) * (command->num_arguments + 2));
  argv[0] = command->program;
  
  for (i = 0; i < command->num_arguments; i++) {
    if (command->arguments[i].kind == ARGUMENT_LITERAL)
      argv[i+1] = command->arguments[i].u.literal;
    else
      argv[i+1] = command->arguments[i].u.var->value;
  }
  
  argv[command->num_arguments + 1] = NULL;

  Execve(argv[0], (char * const *)argv, environ);

  free(argv);
}

/* You'll likely want to use this set_var function for converting a
   numeric value to a string and installing it as a variable's
   value: */

static void set_var(script_var *var, int new_value) {
  char buffer[32];
  free((void *)var->value);
  snprintf(buffer, sizeof(buffer), "%d", new_value);
  var->value = strdup(buffer);
}


/*
  Helper functions the print debugging information.
*/
static void print_script_info(script *scr){
	printf("script info: \n");
	printf("  num_groups = %d\n", scr->num_groups);
	printf("\n");
}

static void print_script_group_info(script_group *group){
  printf("script_group info: \n");
  switch(group->mode){
    case 0 : printf("  mode = %d (Single)\n", group->mode); break;
    case 1 : printf("  mode = %d (And)\n", group->mode); break;
    case 2 : printf("  mode = %d (Or)\n", group->mode); break;
    default: printf("  Not a valid mode.\n"); break;
  }
  printf("  repeats = %d\n", group->repeats);
  printf("  num_commands = %d\n", group->num_commands);
  if(group->result_to == NULL)
    printf("  result_to = NULL\n");
  else{
    printf("  result_to = NOT NULL\n");
    printf("  name = %s\n",(group->result_to->name));
    printf("  value = %s\n",(group->result_to->value));
  }
  printf("\n");
}
