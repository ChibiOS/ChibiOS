# List of all the NASA OSAL over ChibiOS/RT test files.
TESTSRC = ${CHIBIOS}/test/lib/ch_test.c \
          ${CHIBIOS}/test/nasa_osal/source/test/test_root.c \
          ${CHIBIOS}/test/nasa_osal/source/test/test_sequence_001.c

# Required include directories
TESTINC = ${CHIBIOS}/test/lib \
          ${CHIBIOS}/test/nasa_osal/source/test/
