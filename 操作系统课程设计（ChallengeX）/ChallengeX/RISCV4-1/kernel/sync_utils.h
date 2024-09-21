#ifndef _SYNC_UTILS_H_
#define _SYNC_UTILS_H_

static inline void sync_barrier(volatile int *counter, int all) {

  int local;

  asm volatile("amoadd.w %0, %2, (%1)\n"
               : "=r"(local)
               : "r"(counter), "r"(1)
               : "memory");

  if (local + 1 < all) {
    do {
      asm volatile("lw %0, (%1)\n" : "=r"(local) : "r"(counter) : "memory");
    } while (local < all);
  }
}

static inline void spin_lock(volatile int *lock) {
  int expected = 0;
  int value = 1;

  do {
    asm volatile("amoswap.w %0, %2, (%1)\n"
                 : "=r"(expected)
                 : "r"(lock), "r"(value)
                 : "memory");
  } while (expected != 0);
}

static inline void spin_unlock(volatile int *lock) {
  *lock = 0;
}

#endif
