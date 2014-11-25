#******************************************************************************
#
# Makefile - Rules for building the driver library.
#
# Copyright (c) 2005-2014 Texas Instruments Incorporated.  All rights reserved.
# Software License Agreement
# 
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
# 
#   Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# 
#   Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the  
#   distribution.
# 
#   Neither the name of Texas Instruments Incorporated nor the names of
#   its contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# This is part of revision 2.1.0.12573 of the Tiva Peripheral Driver Library.
#
#******************************************************************************

#
# The base directory for TivaWare.
#
ROOT=..

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find header files that do not live in the source directory.
#
IPATH=../inc

#
# The default rule, which causes the driver library to be built.
#
all: ${COMPILER}
all: ${COMPILER}/libdriver.a

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the driver library.
#
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_gpio.o
${COMPILER}/libdriver.a: ${COMPILER}/misc.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_adc.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_bkp.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_can.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_cec.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_crc.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_dac.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_dbgmcu.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_dma.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_exti.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_flash.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_fsmc.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_i2c.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_iwdg.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_pwr.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_rcc.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_rtc.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_sdio.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_spi.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_tim.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_usart.o
${COMPILER}/libdriver.a: ${COMPILER}/stm32f10x_wwdg.o
CFLAGSgcc=-DUSE_STDPERIPH_DRIVER  -DSTM32F10X_MD
#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
