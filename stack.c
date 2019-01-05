/*
 * stack.c
 */

#include "stack.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void init_stack (stack *s)
{
  s->first = NULL;
}

void push (stack *s, int top)
{
  stack_elt *elt;
  
  // Allocate a new list element with malloc
  elt = malloc (sizeof(*elt));

  // If malloc fails end the program
  if (elt == NULL) {
    printf ("Couldn't allocate a new stack element\n");
    exit (1);
  }

  // Allocate space to store the top entry
  //elt->under = malloc(sizeof(stack_elt));

  // If malloc fails end the program
  // if (elt == NULL) {
  //   printf ("Couldn't allocate a new entry\n");
  //   exit (1);
  // }

  elt->top = top;

  // Check if stack is empty
  if (s->first) {
    // add elt to top
    elt->under = s->first;
    s->first = elt;
  } else {
    s->first = elt;
    elt->under = NULL;
  }
}

void pop (stack *s)
{
  stack_elt *temp;
  
  if (s->first != NULL) {
    temp = s->first;

    if ((s->first)->under != NULL) {
     s->first = (s->first)->under;

    } else {
      // there is only one element in the stack
      s->first = NULL;
    }

    // free the element
    free(temp);
  }
  
}


void delete_all_entries (stack *s)
{
  while (s->first) {
    pop(s);
  }
}

void print_stack (stack *s)
{
  stack_elt *elt = s->first;

  printf ("\nSTACK CONTENTS\n");

  while (elt) {
    printf ("%d\n", (elt->top));
    elt = elt->under;
  }
}
 
void print_top (stack *s)
{
  if (s->first) {
    printf ("%d\n", ((s->first)->top));
  }
}

stack_elt* peek_first (stack *s)
{
  return s->first;
}

stack_elt* peek_sec (stack *s) {
  if (s->first) {
    return (s->first)->under;
  }
  return NULL;
}
