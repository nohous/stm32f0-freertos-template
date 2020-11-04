# STM32F0 native FreeRTOS template (GCC)

Some notes: 
- STMCubeMX generator wraps FreeRTOS into CMSIS-RTOS API, which we do not want to use.
- Linker script in CMSIS_v5 is not compatible with startup files provided by ST (renamed sections etc).
- FreeRTOS requires config header
- HAL requires config header
- How to pass SysTicks to FreeRTOS?
- Dont forget to use newlib-nano (--specs=nano.specs)
- Tips on native FreeRTOS at https://jaywang.info/using-native-freertos-in-stm32-microcontrollers/
- CMake support for STM32: https://github.com/ObKo/stm32-cmake

This has been created from upstream FreeRTOS 10.4.1:
```bash
git clone https://github.com/FreeRTOS/FreeRTOS.git
git checkout V10.4.1 --recurse-submodules
git submodule init
git submodule update
```
Files in external/FreeRTOS/ come from pruned upstream FreeRTOS/FreeRTOS/Source. Dir tree diff:
```text
Only in .: CONTRIBUTING.md
Only in .: GitHub-FreeRTOS-Kernel-Home.url
Only in .: History.txt
Only in .: LICENSE.md
Only in .: Quick_Start_Guide.url
Only in .: README.md
Only in .: SECURITY.md
Only in .: lexicon.txt
Only in ./portable: ARMClang
Only in ./portable: ARMv8M
Only in ./portable: BCC
Only in ./portable: CCS
Only in ./portable: CodeWarrior
Only in ./portable: Common
Only in ./portable/GCC: ARM7_AT91FR40008
Only in ./portable/GCC: ARM7_AT91SAM7S
Only in ./portable/GCC: ARM7_LPC2000
Only in ./portable/GCC: ARM7_LPC23xx
Only in ./portable/GCC: ARM_CA53_64_BIT
Only in ./portable/GCC: ARM_CA9
Only in ./portable/GCC: ARM_CM23
Only in ./portable/GCC: ARM_CM23_NTZ
Only in ./portable/GCC: ARM_CM3
Only in ./portable/GCC: ARM_CM33
Only in ./portable/GCC: ARM_CM33_NTZ
Only in ./portable/GCC: ARM_CM3_MPU
Only in ./portable/GCC: ARM_CM4F
Only in ./portable/GCC: ARM_CM4_MPU
Only in ./portable/GCC: ARM_CM7
Only in ./portable/GCC: ARM_CR5
Only in ./portable/GCC: ARM_CRx_No_GIC
Only in ./portable/GCC: ATMega323
Only in ./portable/GCC: AVR32_UC3
Only in ./portable/GCC: AVR_AVRDx
Only in ./portable/GCC: AVR_Mega0
Only in ./portable/GCC: CORTUS_APS3
Only in ./portable/GCC: ColdFire_V2
Only in ./portable/GCC: H8S2329
Only in ./portable/GCC: HCS12
Only in ./portable/GCC: IA32_flat
Only in ./portable/GCC: MCF5235
Only in ./portable/GCC: MSP430F449
Only in ./portable/GCC: MicroBlaze
Only in ./portable/GCC: MicroBlazeV8
Only in ./portable/GCC: MicroBlazeV9
Only in ./portable/GCC: NiosII
Only in ./portable/GCC: PPC405_Xilinx
Only in ./portable/GCC: PPC440_Xilinx
Only in ./portable/GCC: RISC-V
Only in ./portable/GCC: RL78
Only in ./portable/GCC: RX100
Only in ./portable/GCC: RX200
Only in ./portable/GCC: RX600
Only in ./portable/GCC: RX600v2
Only in ./portable/GCC: RX700v3_DPFPU
Only in ./portable/GCC: STR75x
Only in ./portable/GCC: TriCore_1782
Only in ./portable: IAR
Only in ./portable: Keil
Only in ./portable: MPLAB
Only in ./portable: MSVC-MingW
Only in ./portable: MikroC
Only in ./portable: Paradigm
Only in ./portable: RVDS
Only in ./portable: Renesas
Only in ./portable: Rowley
Only in ./portable: SDCC
Only in ./portable: Softune
Only in ./portable: Tasking
Only in ./portable: ThirdParty
Only in ./portable: WizC
Only in ./portable: oWatcom
Only in ./portable: readme.txt
```

Upstream STM32CubeF0 is a base for STM files:
```bash
git clone https://github.com/STMicroelectronics/STM32CubeF0.git
git checkout v1.11.1
```
Files in external/STM32CubeF0 come frome pruned upstream STM32CubeF0, dir tree diff:
```text
Only in .: CODE_OF_CONDUCT.md
Only in .: CONTRIBUTING.md
Only in .: Documentation
Only in ./Drivers: BSP

Only in ./Drivers/CMSIS: docs
Only in ./Drivers/STM32F0xx_HAL_Driver: Release_Notes.html
Only in ./Drivers/STM32F0xx_HAL_Driver: _htmresc
Only in .: License.md
Only in .: Middlewares
Only in .: Projects
Only in .: README.md
Only in .: Release_Notes.html
Only in .: Utilities
Only in .: _htmresc
Only in .: package.xml
```

CMSIS_v5 upstream:
```bash
git clone https://github.com/ARM-software/CMSIS_5.git
git checkout 5.7.0
```
Files are located in external/CMSIS_5.

## What to Be Careful About

Notes:
- When using SysTick_Config() CMSIS function, SysTick clock is apparently by taken from SYSCLK, not from /8 divider https://developer.arm.com/documentation/dui0497/a/cortex-m0-peripherals/optional-system-timer--systick/systick-control-and-status-register?lang=en
- FreeRTOS tick handler (and system service entry etc.) is installed by macro-overriding in FreeRTOSConfig.h (#define xPortSysTickHandler SysTick_Handler) by default, of course this can be extended via custom ISR
- Be careful about proper clock init sequence. Setting clock source doesn't mean it's set at the next instruction; typically one has to wait for clock source status; otherwise you end up with system that will not boot predictably
- Don't forget to enable clock for peripherals
