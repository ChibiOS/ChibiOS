[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[#list conf.*.application.instances.instance as inst]
  [#if inst.@id?string == "org.chibios.spc5.components.portable.chibios_unitary_tests_engine"]
    [#assign instance = inst /]
    [#break]
  [/#if]
[/#list]
[#assign prefix_lower = instance.global_data_and_code.code_prefix.value[0]?trim?lower_case /]
[#assign prefix_upper = instance.global_data_and_code.code_prefix.value[0]?trim?upper_case /]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name=prefix_lower+"test_root.h" /]
[@utils.EmitIndentedCCode "" 2 instance.description.copyright.value[0] /]

/**
 * @file    ${prefix_lower}test_root.h
 * @brief   Test Suite root structures header.
 */

#ifndef ${prefix_upper}TEST_ROOT_H
#define ${prefix_upper}TEST_ROOT_H

[#list instance.sequences.sequence as sequence]
#include "${prefix_lower}test_sequence_${(sequence_index + 1)?string("000")}.h"
[/#list]

#if !defined(__DOXYGEN__)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern const testcase_t * const *${prefix_lower}test_suite[];

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Shared definitions.                                                       */
/*===========================================================================*/

[#if instance.global_data_and_code.global_definitions.value[0]?trim?length > 0]
[@utils.EmitIndentedCCode "" 2 instance.global_data_and_code.global_definitions.value[0] /]

[/#if]
#endif /* !defined(__DOXYGEN__) */

#endif /* ${prefix_upper}TEST_ROOT_H */
