[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="test_root.h" /]
[#list conf.*.application.instances.instance as inst]
  [#if inst.@id?string == "org.chibios.spc5.components.portable.chibios_unitary_tests_engine"]
    [#assign instance = inst /]
    [#break]
  [/#if]
[/#list]
[@utils.EmitIndentedCCode "" 2 instance.description.copyright.value[0] /]

/**
 * @file    test_root.h
 * @brief   Test Suite root structures header.
 */

#ifndef TEST_ROOT_H
#define TEST_ROOT_H

[#list instance.sequences.sequence as sequence]
#include "test_sequence_${(sequence_index + 1)?string("000")}.h"
[/#list]

#if !defined(__DOXYGEN__)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern const testcase_t * const *test_suite[];

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

#endif /* TEST_ROOT_H */
