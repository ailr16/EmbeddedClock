CasioCAN {#mainpage}
============

High Level Description
-------------

Serial State Machine  
![SerialStateMachine](https://bytebucket.org/ailr16-workspace/casiocan/raw/7ffb446fceb3dedd58d51f29de89cc7a6335fcdc/Documentation/img/1d6d06b4-6c54-4ce1-825c-c76a44f2dce7.png?token=a83efe5ad2bdd32dd1d12f1f1ee84a91e866e89a)

Clock State Machine  
![ClocklStateMachine](https://bytebucket.org/ailr16-workspace/casiocan/raw/7ffb446fceb3dedd58d51f29de89cc7a6335fcdc/Documentation/img/clockStateMachine.png?token=a6eff26a9e4387116bdaa2663850ef32f8680424)

Available functions
-------------
- **Serial**  
  Serial_Init()  
  Serial_Task()  
    
- **Clock**  
  Clock_Init()  
  Clock_Task()

- **Heartbeat**  
  Heart_Init()  
  Heart_Beat()  

- **Watchdog Timer**  
  Dog_Init()  
  Peth_TheDog()  

- **LCD**  
  uint8_t HEL_LCD_Init( LCD_HandleTypeDef *hlcd )  
  void HEL_LCD_MspInit( LCD_HandleTypeDef *hlcd )  
  uint8_t HEL_LCD_Command( LCD_HandleTypeDef *hlcd, uint8_t cmd )  
  uint8_t HEL_LCD_Data( LCD_HandleTypeDef *hlcd, uint8_t data )  
  uint8_t HEL_LCD_String( LCD_HandleTypeDef *hlcd, char *str )  
  uint8_t HEL_LCD_SetCursor( LCD_HandleTypeDef *hlcd, uint8_t row, uint8_t col )  
  void HEL_LCD_Backlight( LCD_HandleTypeDef *hlcd, uint8_t state )  
  uint8_t HEL_LCD_Contrast( LCD_HandleTypeDef *hlcd, uint8_t contrast )  
