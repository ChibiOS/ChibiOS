[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[@pp.dropOutputFile /]
[#list conf.*.application.instances.instance as inst]
  [#if inst.@id?string == "org.chibios.spc5.components.portable.chibios_unitary_tests_engine"]
    [#assign instance = inst /]
    [#break]
  [/#if]
[/#list]
[#assign prefix_lower = instance.global_data_and_code.code_prefix.value[0]?trim?lower_case /]
[#assign prefix_upper = instance.global_data_and_code.code_prefix.value[0]?trim?upper_case /]
[#list instance.sequences.sequence as sequence]
  [@pp.changeOutputFile name=prefix_lower+"test_sequence_" + (sequence_index + 1)?string("000") + ".h" /]
[@utils.EmitIndentedCCode "" 2 instance.description.copyright.value[0] /]

/**
 * @file    ${prefix_lower}test_sequence_${(sequence_index + 1)?string("000")}.h
 * @brief   Test Sequence ${(sequence_index + 1)?string("000")} header.
 */

#ifndef ${prefix_upper}TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H
#define ${prefix_upper}TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H

extern const testcase_t * const ${prefix_lower}test_sequence_${(sequence_index + 1)?string("000")}[];

#endif /* ${prefix_upper}TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H */
[/#list]
