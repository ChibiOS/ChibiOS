extern void main(void);

void (*mainptr)(void) = main;

void _start(void) {
  main();
}
