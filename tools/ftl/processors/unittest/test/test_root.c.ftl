[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="test_root.c" /]
[#list conf.*.application.instances.instance as inst]
  [#if inst.@id?string == "org.chibios.spc5.components.portable.chibios_unitary_tests_engine"]
    [#assign instance = inst /]
    [#break]
  [/#if]
[/#list]
[@utils.EmitIndentedCCode "" 2 instance.description.copyright.value[0] /]

/**
 * @mainpage Test Suite Specification
[#if instance.description.introduction.value[0]?trim != ""]
[@utils.FormatStringAsText " * "
                           " * "
                           utils.WithDot(instance.description.introduction.value[0]?trim?cap_first)
                           72 /]
[#else]
 * No introduction.
[/#if]
 *
 * <h2>Test Sequences</h2>
[#if instance.sequences.sequence?size > 0]
  [#list instance.sequences.sequence as sequence]
 * - @subpage test_sequence_${(sequence_index + 1)?string("000")}
  [/#list]
 * .
[#else]
 * No test sequences defined in the test suite.
[/#if]
 */

/**
 * @file    test_root.c
 * @brief   Test Suite root structures code.
 */

#include "hal.h"
#include "ch_test.h"
#include "test_root.h"

#if !defined(__DOXYGEN__)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Array of all the test sequences.
 */
const testcase_t * const *test_suite[] = {
[#list instance.sequences.sequence as sequence]
  [#if sequence.condition.value[0]?trim?length > 0]
#if (${sequence.condition.value[0]}) || defined(__DOXYGEN__)
  [/#if]
  test_sequence_${(sequence_index + 1)?string("000")},
  [#if sequence.condition.value[0]?trim?length > 0]
#endif
  [/#if]
[/#list]
  NULL
};

/*===========================================================================*/
/* Shared code.                                                              */
/*===========================================================================*/

[#if instance.global_data_and_code.global_code.value[0]?trim?length > 0]
[@utils.EmitIndentedCCode "" 2 instance.global_data_and_code.global_code.value[0] /]

[/#if]
#endif /* !defined(__DOXYGEN__) */
