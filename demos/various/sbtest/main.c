#include <stdbool.h>

int bssvar;
int datavar = 5;
const int rodatavar = 18;

static void foo(void);

static void xxx(void) {

}

int main(void) {

  foo();

  while (true) {
    xxx();
  }
}

static void foo(void) {

}
