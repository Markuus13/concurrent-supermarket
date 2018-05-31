#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define CUSTOMERS_NUMBER 10

void *customer_go_shopping_in_the_market(void *arg);
void *worker_verify_customer_shopping(void *arg);

sem_t sem_market_cashier;
sem_t sem_customer_on_cashier;
sem_t sem_customer_served;

int main() {
  pthread_t customers[CUSTOMERS_NUMBER];
  pthread_t worker;
  int customers_id[CUSTOMERS_NUMBER];

  sem_init(&sem_market_cashier, 0, 1);
  sem_init(&sem_customer_on_cashier, 0, 0);
  sem_init(&sem_customer_served, 0, 0);

  pthread_create(&worker, NULL, worker_verify_customer_shopping, NULL);

  for (int i = 0; i < CUSTOMERS_NUMBER; i++) {
    customers_id[i] = i;
    pthread_create(&customers[i], NULL, customer_go_shopping_in_the_market, (void *) &customers_id[i]);
  }

  for (int i = 0; i < CUSTOMERS_NUMBER; i++) {
    pthread_join(customers[i], NULL);
  }

  return 0;
}

void *customer_go_shopping_in_the_market(void *arg) {
  int id = *((int *) arg);

  printf("Customer~%d enters in supermarket.\n", id);
  sleep(1);
  printf("Customer~%d picks some items and goes to supermarket checkout to pay its shopping.\n", id);
  sleep(1);

  sem_wait(&sem_market_cashier);
  sem_post(&sem_customer_on_cashier);
  printf("\nCustomer~%d is paying its shopping.\n", id);
  sem_wait(&sem_customer_served);
  printf("Customer~%d leaves the market.\n\n", id);
  sleep(1);
  sem_post(&sem_market_cashier);

  pthread_exit(0);
}

void *worker_verify_customer_shopping(void *arg) {
  while (1) {
    sem_wait(&sem_customer_on_cashier);
    printf("Worker checks customer's shopping and emits sales receipts.\n");
    sleep(1);
    sem_post(&sem_customer_served);
  }
  pthread_exit(0);
}
