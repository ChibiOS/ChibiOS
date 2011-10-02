#ifndef TMP75_H_
#define TMP75_H_



/* buffers depth */
#define TMP75_RX_DEPTH 2
#define TMP75_TX_DEPTH 2

void init_tmp75(void);
void request_temperature(void);

#endif /* TMP75_H_ */
