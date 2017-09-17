[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[@pp.dropOutputFile /]
[#list conf.*.application.instances.instance as inst]
  [#if inst.@id?string == "org.chibios.spc5.components.portable.chibios_unitary_tests_engine"]
    [#assign instance = inst /]
    [#break]
  [/#if]
[/#list]
[#list instance.sequences.sequence as sequence]
  [@pp.changeOutputFile name="test_sequence_" + (sequence_index + 1)?string("000") + ".h" /]
[@utils.EmitIndentedCCode "" 2 instance.description.copyright.value[0] /]

/**
 * @file    test_sequence_${(sequence_index + 1)?string("000")}.h
 * @brief   Test Sequence ${(sequence_index + 1)?string("000")} header.
 */

#ifndef TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H
#define TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H

extern const testcase_t * const test_sequence_${(sequence_index + 1)?string("000")}[];

#endif /* TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H */
[/#list]
