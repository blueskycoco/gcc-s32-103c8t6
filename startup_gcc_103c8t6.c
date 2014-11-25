//*****************************************************************************
//
// startup_gcc.c - Startup code for use with GNU tools.
//
// Copyright (c) 2013-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.0.12573 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include "system_stm32f10x.h"
//#include "inc/hw_types.h"

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);
static void SVC_Handler(void);
static void DebugMon_Handler(void);
//*****************************************************************************
//
// External declaration for the interrupt handler used by the application.
//
//*****************************************************************************
extern void USART1_IRQHandler(void);

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
static uint32_t pui32Stack[512];
#define BootRAM ((void *)(0xF1E0F85F))
//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
	(void (*)(void))((uint32_t)pui32Stack + sizeof(pui32Stack)),
	// The initial stack pointer
	ResetISR,                               // The reset handler
	NmiSR,                                  // The NMI handler
	FaultISR,                               // The hard fault handler
	0,                      				// The MPU fault handler
	0,                      				// The bus fault handler
	0,                     					// The usage fault handler
	0,                                      // Reserved
	0,                                      // Reserved
	0,                                      // Reserved
	0,                                      // Reserved
	SVC_Handler,                      		// SVCall handler
	DebugMon_Handler,                      	// Debug monitor handler
	0,                                      // Reserved
	 IntDefaultHandler,    //	PendSV_Handler
	 IntDefaultHandler,    //	SysTick_Handler
	 IntDefaultHandler,    //	WWDG_IRQHandler
	 IntDefaultHandler,    //	PVD_IRQHandler
	 IntDefaultHandler,    //	TAMPER_IRQHandler
	 IntDefaultHandler,    //	RTC_IRQHandler
	 IntDefaultHandler,    //	FLASH_IRQHandler
	 IntDefaultHandler,    //	RCC_IRQHandler
	 IntDefaultHandler,    //	EXTI0_IRQHandler
	 IntDefaultHandler,    //	EXTI1_IRQHandler
	 IntDefaultHandler,    //	EXTI2_IRQHandler
	 IntDefaultHandler,    //	EXTI3_IRQHandler
	 IntDefaultHandler,    //	EXTI4_IRQHandler
	 IntDefaultHandler,    //	DMA1_Channel1_IRQHandler
	 IntDefaultHandler,    //	DMA1_Channel2_IRQHandler
	 IntDefaultHandler,    //	DMA1_Channel3_IRQHandler
	 IntDefaultHandler,    //	DMA1_Channel4_IRQHandler
	 IntDefaultHandler,    //	DMA1_Channel5_IRQHandler
	 IntDefaultHandler,    //	DMA1_Channel6_IRQHandler
	 IntDefaultHandler,    //	DMA1_Channel7_IRQHandler
	 IntDefaultHandler,    //	ADC1_2_IRQHandler
	 IntDefaultHandler,    //	USB_HP_CAN1_TX_IRQHandler
	 IntDefaultHandler,    //	USB_LP_CAN1_RX0_IRQHandler
	 IntDefaultHandler,    //	CAN1_RX1_IRQHandler
	 IntDefaultHandler,    //	CAN1_SCE_IRQHandler
	 IntDefaultHandler,    //	EXTI9_5_IRQHandler
	 IntDefaultHandler,    //	TIM1_BRK_IRQHandler
	 IntDefaultHandler,    //	TIM1_UP_IRQHandler
	 IntDefaultHandler,    //	TIM1_TRG_COM_IRQHandler
	 IntDefaultHandler,    //	TIM1_CC_IRQHandler
	 IntDefaultHandler,    //	TIM2_IRQHandler
	 IntDefaultHandler,    //	TIM3_IRQHandler
	 IntDefaultHandler,    //	TIM4_IRQHandler
	 IntDefaultHandler,    //	I2C1_EV_IRQHandler
	 IntDefaultHandler,    //	I2C1_ER_IRQHandler
	 IntDefaultHandler,    //	I2C2_EV_IRQHandler
	 IntDefaultHandler,    //	I2C2_ER_IRQHandler
	 IntDefaultHandler,    //	SPI1_IRQHandler
	 IntDefaultHandler,    //	SPI2_IRQHandler
	 IntDefaultHandler,    //	USART1_IRQHandler
	 IntDefaultHandler,    //	USART2_IRQHandler
	 IntDefaultHandler,    //	USART3_IRQHandler
	 IntDefaultHandler,    //	EXTI15_10_IRQHandler
	 IntDefaultHandler,    //	RTCAlarm_IRQHandler
	 IntDefaultHandler,    //	USBWakeUp_IRQHandler	
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	BootRAM
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern uint32_t _etext;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
	void
ResetISR(void)
{
	uint32_t *pui32Src, *pui32Dest;

	//
	// Copy the data segment initializers from flash to SRAM.
	//
	pui32Src = &_etext;
	for(pui32Dest = &_data; pui32Dest < &_edata; )
	{
		*pui32Dest++ = *pui32Src++;
	}

	//
	// Zero fill the bss segment.
	//
	for(pui32Dest = &_bss; pui32Dest < &_ebss; )
	{
		*pui32Dest++ = 0;
	}

	//
	// Call the application's entry point.
	//
	SystemInit();
	main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
	static void
NmiSR(void)
{
	//
	// Enter an infinite loop.
	//
	while(1)
	{
	}
}
	static void
SVC_Handler(void)
{
	//
	// Enter an infinite loop.
	//
	while(1)
	{
	}
}
	static void
DebugMon_Handler(void)
{
	//
	// Enter an infinite loop.
	//
	while(1)
	{
	}
}
//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
	static void
FaultISR(void)
{
	//
	// Enter an infinite loop.
	//
	while(1)
	{
	}
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
	static void
IntDefaultHandler(void)
{
	//
	// Go into an infinite loop.
	//
	while(1)
	{
	}
}
