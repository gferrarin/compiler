/*
 * stack.h
 */

typedef struct stack_elt_tag {
  int top;
  int elseSeen;
  int trueIf;
  struct stack_elt_tag *under;
} stack_elt;

typedef struct {
  stack_elt *first;
} stack;

void init_stack (stack *s);
void push (stack *s, int top);
void pop (stack *s);
void delete_all_entries (stack *s);
void print_stack (stack *s);
void print_top (stack *s);
stack_elt* peek_first (stack *s);
stack_elt* peek_sec (stack *s);
