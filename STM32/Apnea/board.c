#include "board.h"
#include <softtimer.h>
SPI_InitTypeDef SPI1_InitStructure;
SPI_InitTypeDef SPI2_InitStructure;
DMA_InitTypeDef DMA1_InitStructure;
static uint16_t ADCdata=0;

/*PROTOTYPES*/
void MCOInit(void);

void GpioInit(void) {
    EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/*pressure sensor chip select */
	RCC_AHBPeriphClockCmd(SPI_CS_PRESS_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =SPI_CS_PRESS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SPI_CS_PRESS_PORT, &GPIO_InitStructure);
    
    	/*accelerometer chip select */
	RCC_AHBPeriphClockCmd(SPI_CS_ACCEL_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =SPI_CS_ACCEL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SPI_CS_ACCEL_PORT, &GPIO_InitStructure);
    
    SPI_CS_ACCEL_OFF;
    SPI_CS_PRESS_OFF;

    /*accel int2*/
	RCC_AHBPeriphClockCmd(ACCEL_INT_2_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ACCEL_INT_2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
	GPIO_Init(ACCEL_INT_2_PORT, &GPIO_InitStructure);
    /*exti config*/
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    /* Connect EXTI Line */
    SYSCFG_EXTILineConfig(ACCEL_INT_2_EXTI_PORT, ACCEL_INT_2_EXTI_PIN);
    /* Configure EXTI line */
    EXTI_InitStructure.EXTI_Line = ACCEL_INT_2_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    

    /*leds*/
    /*led1*/
    RCC_AHBPeriphClockCmd(LED1_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =LED1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);


    /*led2*/
    RCC_AHBPeriphClockCmd(LED2_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =LED2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
	LEDS_OFF;
}


/*
*   @brief Check buttery voltage
*/
uint32_t Check_voltage(void)
{
  	//uint16_t ADCdata;
    	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConv(ADC1 );
    	/* Wait until ADC  end of conversion */
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ) == RESET) {

	}
    ADCdata = ADC_GetConversionValue(ADC1 );
    if (ADCdata<LOW_VOLTAGE_LVL){
      return 1;
    }
    else {
      return 0;
    }
}


/*
*  @brief Onchip and onboard periph init
*/
void PeriphInit(void) {
    MCOInit();
	NVIC_Configuration();
	Tim6Init();
	SPI1Init();
    SPI2Init();
	GpioInit();
    ADC13_Init();
    AFE44x0LowLavelInit();
}
/**
*
*/
void MCOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	// Тактуем порт
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);			// MCO output init
 
  RCC_MCOConfig(RCC_MCOSource_HSE, RCC_MCODiv_1);			// Set clk
}
void SPI1Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	// spi 1 on PORTA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* SPI pin mappings */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable the SPI peripheral */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(SPI1 );
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;
#if F16_MHZ
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;/*SPI_BaudRatePrescaler_4;*/
#else 
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
#endif
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI1_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI1_InitStructure);
	/* Enable the SPI peripheral */
	SPI_Cmd(SPI1, ENABLE);
}

void SPI2Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	// spi 2 on PORTB
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	/* SPI pin mappings */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable the SPI peripheral */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(SPI2 );
	SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI2_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI2_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI2_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI2_InitStructure);
	/* Enable the SPI peripheral */
	SPI_Cmd(SPI2, ENABLE);
}
void NVIC_Configuration(void) {

    NVIC_SetPriority(SDIO_IRQn, 3);
    NVIC_EnableIRQ(SDIO_IRQn);

    NVIC_SetPriority(SD_SDIO_DMA_IRQn, 2);
    NVIC_EnableIRQ(SD_SDIO_DMA_IRQn);

    NVIC_SetPriority(TIM6_IRQn, 0);
    NVIC_EnableIRQ(TIM6_IRQn);
}

void AFE44x0LowLavelInit(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
#if 0
    /*afe reset*/
	RCC_AHBPeriphClockCmd(AFE_RESET_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =AFE_RESET_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(AFE_RESET_PORT, &GPIO_InitStructure);
#endif
    /*afe ADC_RDY*/
	RCC_AHBPeriphClockCmd(AFE_ADC_RDY_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =AFE_ADC_RDY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(AFE_ADC_RDY_PORT, &GPIO_InitStructure);
    /*exti config*/
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    /* Connect EXTI Line */
    SYSCFG_EXTILineConfig(AFE_ADC_RDY_EXTI_PORT, AFE_ADC_RDY_EXTI_PIN);
    /* Configure EXTI line */
    EXTI_InitStructure.EXTI_Line = AFE_ADC_RDY_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    
    /*afe DIAG_END*/
	RCC_AHBPeriphClockCmd(AFE_DIAG_END_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =AFE_DIAG_END_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(AFE_DIAG_END_PORT, &GPIO_InitStructure);
    /*afe PWDN*/
	RCC_AHBPeriphClockCmd(AFE_PWDN_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =AFE_PWDN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(AFE_PWDN_PORT, &GPIO_InitStructure);
    AFE_PWDN_ON;

    /*afe SPI CS*/
	RCC_AHBPeriphClockCmd(SPI_CS_AFE_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =SPI_CS_AFE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SPI_CS_AFE_PORT, &GPIO_InitStructure);
	SPI_CS_AFE_OFF;

    NVIC_SetPriority(AFE_ADC_RDY_EXTI_IRQN, AFE_ADC_RDY_EXTI_IRQ_PRIO);
    NVIC_DisableIRQ(AFE_ADC_RDY_EXTI_IRQN);
}

void Tim6Init(void) {
  RCC_ClocksTypeDef Clocks;
  uint32_t timers_mul;
  uint32_t timers_clk;
	RCC ->APB1ENR |= RCC_APB1ENR_TIM6EN; // enable RCC
    /*if APB1 devider not zero, then multiply by 2*/
    if(((RCC->CFGR>>8)&0x07)){
      timers_mul = 2;
    }
    else{
      timers_mul = 1;
    }
    RCC_GetClocksFreq(&Clocks);
    timers_clk = Clocks.PCLK1_Frequency*timers_mul;
#if 1
	TIM6 ->PSC = timers_clk/10000 - 1; // want 10kHz
    /*to debug only*/
    timers_clk = TIM6 ->PSC;
	TIM6 ->ARR = 10 - 1; // 10kHz/10 = 1kHz
#else
			TIM6->PSC =2000-1; // PLCK1=4MHz/2000 = 2kHz
			TIM6->ARR = 2-1;// 2kHz/2 = 1kHz
#endif 
	TIM6 ->DIER = TIM_DIER_UIE; //update interrupt enable
	TIM6 ->CR1 = /*TIM_CR1_CEN |*/TIM_CR1_ARPE; // Auto-reload preload enable
}


/**
 * @brief  Initializes the ADC channel 13.
 * @param  None
 * @retval None
 */
void ADC13_Init(void){
  	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable The HSI (16Mhz) */
	RCC_HSICmd(ENABLE);
	/* Enable the GPIOC  Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	/* Configure PC.3 (ADC Channel13)  in analog mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ADC1 Configuration ------------------------------------------------------*/
	ADC_DeInit(ADC1 );
	ADC_BankSelection(ADC1, ADC_Bank_A );
	/* Enable ADC1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel5 or channel1 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1,
			ADC_SampleTime_384Cycles );

	/* Define delay between ADC1 conversions */
	ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze );

	/* Enable ADC1 Power Down during Delay */
	ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* Wait until ADC1 ON status */

	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS ) == RESET) {
	}
}

/**
 * @brief  DeInitializes the SDIO interface.
 * @param  None
 * @retval None
 */
void SD_LowLevel_DeInit(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/*!< Disable SDIO Clock */
	SDIO_ClockCmd(DISABLE);

	/*!< Set Power State to OFF */
	SDIO_SetPowerState(SDIO_PowerState_OFF);

	/*!< DeInitializes the SDIO peripheral */
	SDIO_DeInit();

	/* Disable the SDIO APB2 Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, DISABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_SDIO);

	/* Configure PC.08, PC.09, PC.10, PC.11 pins: D0, D1, D2, D3 pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10
			| GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PD.02 CMD line */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure PC.12 pin: CLK pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
 * @brief  Initializes the SD Card and put it into StandBy State (Ready for
 *         data transfer).
 * @param  None
 * @retval None
 */
void SD_LowLevel_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC and GPIOD Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_SDIO);

	/* Configure PC.08, PC.09, PC.10, PC.11 pins: D0, D1, D2, D3 pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10
			| GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PD.02 CMD line */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure PC.12 pin: CLK pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#if SD_DETECT_ENABLE  
	/*!< Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
	GPIO_InitStructure.GPIO_Pin = SD_DETECT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_InitStructure);
#endif
	/* Enable the SDIO APB2 Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDIO, ENABLE);

	/* Enable the DMA2 Clock */
	RCC_AHBPeriphClockCmd(SD_SDIO_DMA_CLK, ENABLE);

	//TODO//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, DISABLE);
}

/**
 * @brief  Configures the DMA2 Channel4 for SDIO Tx request.
 * @param  BufferSRC: pointer to the source buffer
 * @param  BufferSize: buffer size
 * @retval None
 */
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize) {
	DMA_InitTypeDef DMA_InitStructure;

	DMA_ClearFlag(
			SD_SDIO_DMA_FLAG_TC | SD_SDIO_DMA_FLAG_TE | SD_SDIO_DMA_FLAG_HT
					| SD_SDIO_DMA_FLAG_GL);

	/*!< SDIO DMA CHANNEL disable */
	DMA_Cmd(SD_SDIO_DMA_CHANNEL, DISABLE);

	/*!< SDIO DMA CHANNEL Config */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) BufferSRC;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(SD_SDIO_DMA_CHANNEL, &DMA_InitStructure);
	DMA_ITConfig(SD_SDIO_DMA_CHANNEL, DMA_IT_TC, ENABLE);
	/*!< SDIO DMA CHANNEL enable */
	DMA_Cmd(SD_SDIO_DMA_CHANNEL, ENABLE);
}

/**
 * @brief  Configures the DMA2 Channel4 for SDIO Rx request.
 * @param  BufferDST: pointer to the destination buffer
 * @param  BufferSize: buffer size
 * @retval None
 */
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize) {
	DMA_InitTypeDef DMA_InitStructure;

	DMA_ClearFlag(
			SD_SDIO_DMA_FLAG_TC | SD_SDIO_DMA_FLAG_TE | SD_SDIO_DMA_FLAG_HT
					| SD_SDIO_DMA_FLAG_GL);

	/*!< SDIO DMA CHANNEL disable */
	DMA_Cmd(SD_SDIO_DMA_CHANNEL, DISABLE);

	/*!< SDIO DMA CHANNEL Config */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) BufferDST;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(SD_SDIO_DMA_CHANNEL, &DMA_InitStructure);
	DMA_ITConfig(SD_SDIO_DMA_CHANNEL, DMA_IT_TC, ENABLE);
	/*!< SDIO DMA CHANNEL enable */
	DMA_Cmd(SD_SDIO_DMA_CHANNEL, ENABLE);
}
