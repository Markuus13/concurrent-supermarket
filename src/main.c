#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define DEFAULT "\x1B[0m"
#define BLUE "\x1B[34m"
#define YELLOW "\x1B[33m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define GRAY "\x1B[37m"

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

  printf(DEFAULT "Customer~%d enters in supermarket.\n", id);
  sleep(1);
  printf(DEFAULT "Customer~%d picks some items and goes to supermarket checkout to pay its shopping.\n", id);
  sleep(1);

  sem_wait(&sem_market_cashier);
  sem_post(&sem_customer_on_cashier);
  printf(GREEN "\nCustomer~%d is paying its shopping.\n", id);
  sem_wait(&sem_customer_served);
  printf(GRAY "Customer~%d leaves the market.\n\n", id);
  sleep(1);
  sem_post(&sem_market_cashier);

  pthread_exit(0);
}

void *worker_verify_customer_shopping(void *arg) {
  while (1) {
    sem_wait(&sem_customer_on_cashier);
    printf(YELLOW "Worker checks customer's shopping and emits sales receipts.\n");
    sleep(1);
    sem_post(&sem_customer_served);
  }
  pthread_exit(0);
}
