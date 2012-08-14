#!/bin/bash
FILES=`find -name 'mcuconf.h'`
for i in $FILES
do
	cp $i ${i}_backup
	cat ${i}_backup | \
		sed -e 's/\(#define STM32_I2C_I2C[0-9]_IRQ_PRIORITY\)\([ ]*\)\([0-9]*\)/\1\25/'     | \
		sed -e 's/\(#define STM32_I2C_I2C[0-9]_DMA_PRIORITY\)\([ ]*\)\([0-9]*\)/\1\23/'     | \
		sed -e 's/\(#define STM32_ADC_IRQ_PRIORITY\)\([ ]*\)\([0-9]*\)/\1\26/'				| \
		sed -e 's/\(#define STM32_ADC_ADC[0-9]_IRQ_PRIORITY\)\([ ]*\)\([0-9]*\)/\1\26/'     | \
		sed -e 's/\(#define STM32_ADC_ADC[0-9]_DMA_IRQ_PRIORITY\)\([ ]*\)\([0-9]*\)/\1\26/' > $i
	rm ${i}_backup
done
