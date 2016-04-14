#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LENG sizeof(struct mystring)

struct mystring{
  char str[20];
  struct mystring *next;
};

// Create the head of the chain
struct mystring *create_string()
{
  struct mystring *head;
  
  head = (struct mystring *) malloc(LENG);
  strcpy(head->str, "null");
  head->next = NULL;
  return head;  
}

// Add a string to the chain 
// To make a queue
void add_string(struct mystring *head, char str[])
{
  struct mystring * new_str = create_string();
  strcpy(new_str->str, str);

  struct mystring * temp = head;
  while (temp->next != NULL)
  {
    temp = temp->next;
  }
  temp->next = new_str;
}

// Traverse the chain
void traverse_string(struct mystring *head)
{
  struct mystring * temp = head;
  while(temp->next != NULL)
  {
    printf("The string is %s\n", temp->next->str);
    temp = temp->next;    
  }
}
