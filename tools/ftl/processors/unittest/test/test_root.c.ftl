[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[#list xml.*.application.instances.instance as inst]
  [#if inst.@id?string == "org.chibios.spc5.components.portable.chibios_unitary_tests_engine"]
    [#assign instance = inst /]
    [#break]
  [/#if]
[/#list]
[#assign conf = {"instance":instance} /]
[#assign prefix_lower = conf.instance.global_data_and_code.code_prefix.value[0]?trim?lower_case /]
[#assign prefix_upper = conf.instance.global_data_and_code.code_prefix.value[0]?trim?upper_case /]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name=prefix_lower+"test_root.c" /]
[@utils.EmitIndentedCCode "" 2 conf.instance.description.copyright.value[0] /]

/**
 * @mainpage Test Suite Specification
[#if conf.instance.description.introduction.value[0]?trim != ""]
[@utils.FormatStringAsText " * "
                           " * "
                           utils.WithDot(conf.instance.description.introduction.value[0]?trim?cap_first)
                           72 /]
[#else]
 * No introduction.
[/#if]
 *
 * <h2>Test Sequences</h2>
[#if conf.instance.sequences.sequence?size > 0]
  [#list conf.instance.sequences.sequence as sequence]
 * - @subpage ${prefix_lower}test_sequence_${(sequence_index + 1)?string("000")}
  [/#list]
 * .
[#else]
 * No test sequences defined in the test suite.
[/#if]
 */

/**
 * @file    ${prefix_lower}test_root.c
 * @brief   Test Suite root structures code.
 */

#include "hal.h"
#include "${prefix_lower}test_root.h"

#if !defined(__DOXYGEN__)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Array of test sequences.
 */
const testsequence_t * const ${prefix_lower}test_suite_array[] = {
[#list conf.instance.sequences.sequence as sequence]
  [#if sequence.condition.value[0]?trim?length > 0]
#if (${sequence.condition.value[0]}) || defined(__DOXYGEN__)
  [/#if]
  &${prefix_lower}test_sequence_${(sequence_index + 1)?string("000")},
  [#if sequence.condition.value[0]?trim?length > 0]
#endif
  [/#if]
[/#list]
  NULL
};

/**
 * @brief   Test suite root structure.
 */
const testsuite_t ${prefix_lower}test_suite = {
  "${utils.WithoutDot(conf.instance.description.brief.value[0]?trim)}",
  ${prefix_lower}test_suite_array
};

/*===========================================================================*/
/* Shared code.                                                              */
/*===========================================================================*/

[#if conf.instance.global_data_and_code.global_code.value[0]?trim?length > 0]
[@utils.EmitIndentedCCode "" 2 conf.instance.global_data_and_code.global_code.value[0] /]

[/#if]
#endif /* !defined(__DOXYGEN__) */
