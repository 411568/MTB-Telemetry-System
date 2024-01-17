#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_conf.h"
#include "main.h"
#include "ST7565.h"
#include "stm32f4xx_hal_rtc.h"
#include "rtc.h"
#include <stdio.h>
#include <string.h>


// Enter settings menu
void enterMenu();

// Time setup
void setupTime();
