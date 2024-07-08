/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "24cxx.h"
#include <stm32f0xx_hal_uart.h>
//#include <stm32f0xx_hal_dma.h>
//#include "stm32f0xx_hal.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define UART_TIMEOUT    60
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch. FILE *f)
#endif /* __GNUC__ */

#define USART_MAX_LEN  255
#define DOUBLE_END_MARK  //閸欏瞼绮ㄩ弶鐔剁秴閺冭埖澧﹂敓锟�???????????
#if SINGLE_END_MARK
#define USART_END_MARK 0XFF                //鐎规矮绠熺紒鎾存将閿燂拷???????????
#endif
#ifdef DOUBLE_END_MARK
#define USART_END_MARK1  0x0D   //鐎规矮绠熺紒鎾存将閿燂拷???????????
#define USART_END_MARK2  0x0A        //鐎规矮绠熺紒鎾存将閺嶅洤绻�
#endif
#define I2C_BUFF_SIZE 255
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	//uint8_t I2C_BUFF_SIZE=1;
	uint8_t I2C_BUFFER_WRITE[I2C_BUFF_SIZE] = {0};
	uint8_t I2C_BUFFER_READ[I2C_BUFF_SIZE] = {0};
	uint8_t WRITE_DATA_SIZE;
	uint16_t MemAddress;


	uint8_t RX_Buff[1]={0};  //鏉╂瑤閲滈惄绋跨秼娴滃骸鐣炬稊澶夌啊1byte閻ㄥ嫭甯撮弨鍓佺处鐎涙ê灏�
	uint8_t USART_RX_BUF[USART_MAX_LEN]={0}; //鐎规矮绠熼弫鐗堝祦鐎涙ɑ鏂佺粚娲？
	int8_t USART_RX_STA = 0;        //閺佺増宓佺�涙ɑ鏂佺拋鈩冩殶閺嶅洩顔�
	uint8_t n = 0;
	char rx_flg = 0;


extern	DMA_HandleTypeDef hdma_usart1_rx;
extern	DMA_HandleTypeDef hdma_usart1_tx;
extern  uint8_t WRITE_DATA_COUNT;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
int fputc(int ch, FILE *f)         //闁插秴鍟損rintf閸戣姤鏆�
{
	while(!((USART1->ISR) & (1<<7)));
	USART1->TDR = (uint8_t)ch;
	return ch;
}
*/
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
/*
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{

		HAL_UART_Transmit_IT(&huart1, WRITE_DATA, sizeof(WRITE_DATA));
		//printf("data:%d\n",WRITE_DATA);
		memset(WRITE_DATA, 0, sizeof(WRITE_DATA));
		HAL_UART_Receive_IT(&huart1, WRITE_DATA, sizeof(WRITE_DATA));
	}

}
*/

/*******************************************************************************/
/* 閸戣姤鏆熼崝鐔诲厴閿涙矮瑕嗛崣锝勮厬閺傤厼娲栫拫鍐ㄥ毐閺佸府绱濋悽銊ょ艾娑撳秴鐣鹃梹鎸庢殶閹诡喗甯撮弨璁圭礉娴ｅ棝娓剁憰浣规箒缂佹挻娼弽鍥х箶閿燂拷???????????           閿燂拷???????????閿燂拷???????????        閿燂拷???????????閿燂拷???????????*/
/*                                                                             */
/* 閸戣姤鏆熼崣鍌涙殶閿涙瓗ART_HandleTypeDef缁鐎风紒鎾寸�敓锟�???????????                                       閿燂拷???????????閿燂拷???????????*/
/*                                                                             */
/* 閸戣姤鏆熸潻鏂挎礀閸婄》绱伴敓锟�???????????                                                               閿燂拷???????????*/
/*******************************************************************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)        //娑撴彃褰涙稉顓熸焽閸ョ偠鐨熼崙鑺ユ殶
{

	UNUSED(huart);
	if(huart->Instance == USART1)
	{
		//rx_flg = 1;

#ifdef DOUBLE_END_MARK  //閸欏苯浠犲顫秴
		printf("something is done\n");

		//printf("register data:0x%x\n", USART1->RDR );
		//USART_RX_BUF[USART_RX_STA ++ ] = RX_Buff[n++];
		USART_RX_BUF[USART_RX_STA ++ ] = RX_Buff[n++];

		printf("data now:0x%x,next:0x%x\n",RX_Buff[1], RX_Buff[0]);
		//if(n==2){
		//			n = 0;
		//		}
		//printf("data now:0x%x\n",USART_RX_BUF[USART_RX_STA - 1]);
		//printf("USART_RX_BUF[USART_RX_STA-1] data:0x%x,usart_rx_sta:%d\n", USART_RX_BUF[USART_RX_STA-1],USART_RX_STA-1);
		//printf("first..%d\n",*((uint8_t*)huart->pRxBuffPtr - 1));
		//__HAL_UART_CLEAR_OREFLAG(huart);
        if(USART_RX_BUF[USART_RX_STA-1] == USART_END_MARK2)
        {
                if(USART_RX_BUF[USART_RX_STA-2] == USART_END_MARK1)
                {
                		printf("end sending %d data is sended\n", USART_RX_STA);

        		        for(uint8_t i = 0; i < USART_RX_STA; i++)
        		        {
        		        	printf("UART DataBuff[%d] = 0x%x\r\n",i, USART_RX_BUF[i]);
        		        }
                        memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
                        //USART_RX_BUF[USART_RX_STA-1] = 0x00;
                        USART_RX_STA = 0;
                        //HAL_UART_Transmit_IT(&huart1,(uint8_t *)RX_Buff,sizeof(RX_Buff));
                }

        }
        else{
        //HAL_StatusTypeDef ret;

        //do{
 ///       HAL_UART_Transmit_IT(&huart1,RX_Buff,sizeof(RX_Buff));
        HAL_UART_Receive_DMA(&huart1, USART_RX_BUF, sizeof(USART_RX_BUF));
       // HAL_UARTEx_ReceiveToIdle_DMA(&huart1, USART_RX_BUF, sizeof(USART_RX_BUF));
        //memset(RX_Buff, 0, sizeof(RX_Buff));
         RX_Buff[0] = 0;
         //HAL_UART_Receive_IT(&huart1,RX_Buff,sizeof(RX_Buff));

        //huart1.RxState = HAL_UART_STATE_READY;
        //__HAL_UNLOCK(&huart1);
        //}while(ret != HAL_OK);

        printf("second receive\n");
       // return;
       }

#endif

#ifdef SINGLE_END_MARK        //閸楁洖浠犲顫秴
        if(*((uint8_t*)huart->pRxBuffPtr - 1 )== USART_END_MARK)
        {
                        USART_RX_BUF[USART_RX_STA] = 0x00;
                        HAL_UART_Receive_IT(&huart1, &USART_RX_ADD, 1);
                        USART_RX_STA = 0;
                        return;
        }

        else
        {
                if(USART_RX_STA > sizeof(USART_RX_BUF))
                {
                        USART_RX_STA = 0;
                }
                USART_RX_BUF[USART_RX_STA ++ ] = *((uint8_t*)huart->pRxBuffPtr - 1 );
                HAL_UART_Receive_IT(huart, &USART_RX_ADD, 1);
                return;
        }
#endif

	}
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == &huart1)
	{
		rx_flg = 1;

		//HAL_UARTEx_ReceiveToIdle_DMA(&huart1, USART_RX_BUF, sizeof(USART_RX_BUF));
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);//浼犺緭杩囧崐涓柇绂佺敤
	}
}

void Write_EEPROM(void)
{
	if(EP24C_IsDeviceReady()==HAL_OK)
	{
	/*
for (uint8_t i=0; i<WRITE_DATA_SIZE; i++ ) { //婵� 鍘栫紓鎾冲暱
	  I2C_BUFFER_WRITE[i] = i;
         printf("0x%02X ", I2C_BUFFER_WRITE[i]);
         if (i%16 == 15)
             printf("\n\r");
     }
     */
	printf("%d was received\n", WRITE_DATA_COUNT);
//鐏忓捄2c_Buf_Write娑擃參銆庢惔蹇ユ嫹?閿熻棄顤冮惃鍕殶閹诡喖鍟撻崗顧婨RPOM閿燂拷????????????
EP24C_WriteLongData(0, USART_RX_BUF, WRITE_DATA_COUNT);
HAL_Delay(100);
printf("I2C writes rom end\n");
EP24C_ReadBytes(0, I2C_BUFFER_READ, WRITE_DATA_COUNT);

for (uint8_t i=0; i<WRITE_DATA_COUNT; i++) {
         if (I2C_BUFFER_READ[i] != USART_RX_BUF[i])
         {
             printf("The data write to the rom:(0x%02X)\n ", USART_RX_BUF[i]);
             printf("Error:I2C EEPROM reading is not the same as writing\n");
             //return 0;
         }

         printf("the data read from the rom:0x%02X\n ", I2C_BUFFER_READ[i]);
         if (i%16 == 15)
             printf("\n\r");
     }
     printf("\n\r I2C(AT24C02) writing ending\n");
     //HAL_UART_Transmit_DMA(&huart1, I2C_BUFFER_READ, WRITE_DATA_COUNT);
}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	//uint8_t ch;
	//uint8_t ch[20] = {0};

	//uint8_t READ_DATA = 0;
	//uint8_t READ_BUFFER;
	//HAL_StatusTypeDef read_result = HAL_ERROR;


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  HAL_Delay(100);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  //HAL_UART_Transmit(&huart1, "This is string\n", strlen("This is string\n"), UART_TIMEOUT);
  printf("this is printf test\n");
  //HAL_Delay(100);
  //HAL_UART_Receive_IT(&huart1, RX_Buff, sizeof(RX_Buff));
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart1, USART_RX_BUF, sizeof(USART_RX_BUF));
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, USART_RX_BUF, sizeof(USART_RX_BUF));
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

  //HAL_UART_ErrorCallback(&huart1);
  /*
  if (HAL_UART_Receive_IT(&huart1, &USART_RX_ADD, 1) != HAL_OK)
  {
	  __HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
      //Error_Handler();
  }
  */

  //HAL_UART_Receive_IT(&huart1, WRITE_DATA, 1);

 // Write_EEPROM();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(rx_flg > 0)
	  {
		  rx_flg = 0;
		  //HAL_UART_Transmit_DMA(&huart1, USART_RX_BUF, huart1.RxXferCount);
		  //HAL_I2C_Master_Transmit_DMA(&hi2c1, DEV_ADDR_24CXX, USART_RX_BUF, huart1.RxXferCount);
		  Write_EEPROM();

		  huart1.RxXferCount = 0;
		  HAL_UART_Receive_DMA(&huart1, USART_RX_BUF, sizeof(USART_RX_BUF));
		  //HAL_UARTEx_ReceiveToIdle_DMA(&huart1, USART_RX_BUF, sizeof(USART_RX_BUF));
	  }
/*
	  if(!HAL_OK){
	  		//read_result = EP24C_WriteOneByte(0x00, WRITE_DATA);
		  read_result = HAL_UART_Receive_IT(&huart1, &WRITE_DATA, 1);
	  		printf("start writing\n");
	  	 }
	  	 HAL_Delay(10);
	  	  printf("end write\n");
	  	 if(read_result == HAL_OK){
	  		 HAL_UART_Transmit_IT(&huart1, &WRITE_DATA, 1);
	  	 }

	  	 printf("data:0x%02x \r\n",WRITE_DATA);
	  	  memset(&WRITE_DATA, 0, 20);
*/
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
