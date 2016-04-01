# List of all the ChibiOS/RT test files.
TESTSRC = ${CHIBIOS}/test/lib/ch_test.c \
          ${CHIBIOS}/test/rt/source/test/test_root.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_001.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_002.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_003.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_004.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_005.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_006.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_007.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_008.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_009.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_010.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_011.c \
          ${CHIBIOS}/test/rt/source/test/test_sequence_012.c

# Required include directories
TESTINC = ${CHIBIOS}/test/lib \
          ${CHIBIOS}/test/rt/source/test
