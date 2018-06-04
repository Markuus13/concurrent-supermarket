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

#define CUSTOMERS_NUMBER 12
#define WORKERS_NUMBER 3

void *customer_goes_to_the_market(void *arg);
void customer_enters_in_supermarket(int id);
void customer_picks_items_in_shelves(int id);
void customer_enters_in_one_queue(int id);
void customer_pays_for_his_shopping_and_leaves(int id);

void *worker_verify_customer_shopping(void *arg);
void worker_waits_for_customer(int id);
void worker_checks_shopping_and_emits_receipts(int id);

sem_t sem_market_cashier;
sem_t sem_customer_on_cashier;
sem_t sem_customer_served;

int main() {
  pthread_t customers[CUSTOMERS_NUMBER];
  pthread_t workers[WORKERS_NUMBER];

  int customers_id[CUSTOMERS_NUMBER];
  int workers_id[WORKERS_NUMBER];

  sem_init(&sem_market_cashier, 0, 1);
  sem_init(&sem_customer_on_cashier, 0, 0);
  sem_init(&sem_customer_served, 0, 0);

  for (int i = 0; i < WORKERS_NUMBER; i++) {
    workers_id[i] = i;
    pthread_create(&workers[i], NULL, worker_verify_customer_shopping, (void *) &workers_id[i]);
  }

  for (int i = 0; i < CUSTOMERS_NUMBER; i++) {
    customers_id[i] = i;
    pthread_create(&customers[i], NULL, customer_goes_to_the_market, (void *) &customers_id[i]);
  }

  for (int i = 0; i < WORKERS_NUMBER; i++) {
    pthread_join(workers[i], NULL);
  }

  for (int i = 0; i < CUSTOMERS_NUMBER; i++) {
    pthread_join(customers[i], NULL);
  }

  return 0;
}

void *customer_goes_to_the_market(void *arg) {
  int id = *((int *) arg);

  customer_enters_in_supermarket(id);
  customer_picks_items_in_shelves(id);
  customer_enters_in_one_queue(id);
  customer_pays_for_his_shopping_and_leaves(id);

  pthread_exit(0);
}

void customer_enters_in_supermarket(int id) {
  printf(DEFAULT "Customer~%d enters in supermarket.\n", id);
  sleep(2);
}

void customer_picks_items_in_shelves(int id) {
  printf(DEFAULT "Customer~%d picks some items and goes to supermarket checkout to pay its shopping.\n", id);
  sleep(2);
}

void customer_enters_in_one_queue(int id) {

}

void customer_pays_for_his_shopping_and_leaves(int id) {
  sem_wait(&sem_market_cashier);
  sem_post(&sem_customer_on_cashier);
  printf(GREEN "\nCustomer~%d goes to the cashier and pays his shopping.\n", id);
  sem_wait(&sem_customer_served);
  printf(GRAY "Customer~%d leaves the market.\n\n", id);
  sleep(1);
  sem_post(&sem_market_cashier);
}

void worker_waits_for_customer(int id) {
  printf(YELLOW "Worker~%d waits for customers on the cashier.\n", id);
  sem_wait(&sem_customer_on_cashier);
  sleep(1);
}

void worker_checks_shopping_and_emits_receipts(int id) {
  printf(YELLOW "Worker~%d checks customer's shopping and emits sales receipts.\n", id);
  sem_post(&sem_customer_served);
  sleep(1);
}

void *worker_verify_customer_shopping(void *arg) {
  int id = *((int *) arg);

  while (1) {
    worker_waits_for_customer(id);
    worker_checks_shopping_and_emits_receipts(id);
  }

  pthread_exit(0);
}
