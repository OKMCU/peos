/**
  ******************************************************************************
  * @file    
  * @author  
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 Cloud Light Technology Ltd.</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#if (ST_ASSERT_EN > 0)
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void st_assert_failed(char *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif // (ST_ASSERT_EN > 0)

/**
  * @}
  */

/**
  * @}
  */
/********** (C) COPYRIGHT 2019 Cloud Light Technology Ltd. *****END OF FILE****/

