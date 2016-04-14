#include <stdlib.h>
#include <stdio.h>

#define LEN sizeof(struct mynum)

struct mynum{
  int num;
  struct mynum *next;
};

// Create the chain to trace number
struct mynum *create_num()
{
  struct mynum *head;
  
  head = (struct mynum *) malloc(LEN);
  head->num = -1;
  head->next = NULL;
  return head;  
}

// Add a number to the chain
void add_num(struct mynum *head, int num)
{
  struct mynum * new_num = create_num();
  new_num->num = num;
  if (head->next == NULL)
  {
    head->next = new_num;
  }  
  else{
    struct mynum * temp = head->next;
    head->next = new_num;
    new_num->next = temp;
  }
}

// Search a number in the chain
int search_num(struct mynum *head, int num)
{
  struct mynum * temp = head;
  while(temp->next != NULL)
  {
    if (temp->next->num == num)
    {
      return 1;
    }
    temp = temp->next;
  }
  return -1;
}

// Delete a number from the chain
void delete_num(struct mynum *head, int num)
{
  struct mynum * temp = head;
  while(temp->next != NULL)
  {
    if (temp->next->num == num)
    {
      if (temp->next->next == NULL)
      {
        struct mynum * discard = temp->next;
        temp->next = NULL;
        free(discard);
        discard = NULL;
      }
      else{
        struct mynum * discard = temp->next;
        temp->next = temp->next->next;
        free(discard);
        discard = NULL;
      }
      break;
    }
    temp = temp->next;
  }
}

// Traverse the chain
void traverse_num(struct mynum *head)
{
  struct mynum * temp = head;
  while(temp->next != NULL)
  {
    printf("The number is %d\n", temp->next->num);
    temp = temp->next;    
  }
}
