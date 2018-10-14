MCU_SERIES = l4
CMSIS_MCU = STM32L433xx
AF_FILE = boards/stm32l433_af.csv
LD_FILES = boards/stm32l433rc.ld boards/common_ifs.ld

# Don't include default frozen modules because MCU is tight on flash space
FROZEN_MPY_DIR ?=
