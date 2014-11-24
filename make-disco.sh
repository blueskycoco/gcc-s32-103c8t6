#! /bin/bash
export RTT_EXEC_PATH="d:/Program Files/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI/bin"
make
openocd -f openocd.cfg -c "flash_image"
#openocd -f openocd.cfg -c "flash_param"
