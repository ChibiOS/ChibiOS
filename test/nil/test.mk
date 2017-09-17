# List of all the ChibiOS/NIL test files.
TESTSRC = ${CHIBIOS}/test/lib/ch_test.c \
          ${CHIBIOS}/test/nil/source/test/test_root.c \
          ${CHIBIOS}/test/nil/source/test/test_sequence_001.c \
          ${CHIBIOS}/test/nil/source/test/test_sequence_002.c \
          ${CHIBIOS}/test/nil/source/test/test_sequence_003.c \
          ${CHIBIOS}/test/nil/source/test/test_sequence_004.c \
          ${CHIBIOS}/test/nil/source/test/test_sequence_005.c \
          ${CHIBIOS}/test/nil/source/test/test_sequence_006.c\
          ${CHIBIOS}/test/nil/source/test/test_sequence_007.c

# Required include directories
TESTINC = ${CHIBIOS}/test/lib \
          ${CHIBIOS}/test/nil/source/test
