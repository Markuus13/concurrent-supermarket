#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CUSTOMERS_NUMBER 10

void *customer_go_shopping_in_the_market(void *arg);

pthread_t customers[CUSTOMERS_NUMBER];

int main() {
  for (int i = 0; i < CUSTOMERS_NUMBER; i++) {
    int *id = (int *) malloc(sizeof (int));
    *id = i;
    pthread_create(customers, NULL, customer_go_shopping_in_the_market, (void *) (id));
  }

  for (int i = 0; i < CUSTOMERS_NUMBER; i++) {
    pthread_join(customers[i], NULL);
  }

  return 0;
}

void *customer_go_shopping_in_the_market(void *arg) {
  int id = *((int *) arg);

  printf("Customer[%d] enters in supermarket.\n", id);

  pthread_exit(0);
}
