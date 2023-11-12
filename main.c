#include <project.h>
#define INIT_STATE (0X40)
#define PASS_STATE (0X20)
#define FAIL_STATE (0X10)

#define POT_CHAN (0)
#define WRITABLE (3)

#define Buzz_C (3824)
#define Buzz_G (2552)
#define Buzz_A (2272)
#define Buzz_D (3408)
#define Buzz_F (2865)
#define Buzz_E (3034)

#define Buzz_Delay (200)



void Buzz_Play_SuccesMelody(void) // Перемога
{
    Buzz_PWM_Wakeup();
    //C
    Buzz_PWM_WritePeriod(Buzz_C);
    Buzz_PWM_WriteCompare(Buzz_C/2);
    CyDelay(Buzz_Delay);
    
    //G
    Buzz_PWM_WritePeriod(Buzz_G);
    Buzz_PWM_WriteCompare(Buzz_G/2);
    CyDelay(Buzz_Delay);
    
    //A
    Buzz_PWM_WritePeriod(Buzz_A);
    Buzz_PWM_WriteCompare(Buzz_A/2);
    CyDelay(Buzz_Delay);
    
    //G
    Buzz_PWM_WritePeriod(Buzz_G);
    Buzz_PWM_WriteCompare(Buzz_G/2);
    CyDelay(Buzz_Delay);
    
    //C
    Buzz_PWM_WritePeriod(Buzz_C);
    Buzz_PWM_WriteCompare(Buzz_C/2);
    CyDelay(Buzz_Delay);
    
    //G
    Buzz_PWM_WritePeriod(Buzz_G);
    Buzz_PWM_WriteCompare(Buzz_G/2);
    CyDelay(Buzz_Delay);
    
    //A
    Buzz_PWM_WritePeriod(Buzz_A);
    Buzz_PWM_WriteCompare(Buzz_A/2);
    CyDelay(Buzz_Delay);
    
    //G
    Buzz_PWM_WritePeriod(Buzz_G);
    Buzz_PWM_WriteCompare(Buzz_G/2);
    CyDelay(Buzz_Delay);
    
    Buzz_PWM_Sleep();
}

void Buzz_Play_DefMelody(void) //Програш
{
    Buzz_PWM_Wakeup();
    
    //D
    Buzz_PWM_WritePeriod(Buzz_D);
    Buzz_PWM_WriteCompare(Buzz_D/2);
    CyDelay(Buzz_Delay + 300);
    
    //OffBuzz
    Buzz_PWM_Sleep();
    CyDelay(Buzz_Delay / 2);
    Buzz_PWM_Wakeup();    
    
    //D
    Buzz_PWM_WritePeriod(Buzz_D);
    Buzz_PWM_WriteCompare(Buzz_D/2);
    CyDelay(Buzz_Delay + 300);
    
    //OffBuzz
    Buzz_PWM_Sleep();
    CyDelay(Buzz_Delay / 2);
    Buzz_PWM_Wakeup();
    
    //D
    Buzz_PWM_WritePeriod(Buzz_D);
    Buzz_PWM_WriteCompare(Buzz_D/2);
    CyDelay(Buzz_Delay + 1000);
    
    Buzz_PWM_Sleep();
}

void Buzz_Play_ButtMelody(int RegButt) //Програш
{
    Buzz_PWM_Wakeup();
    
    switch(RegButt)
    {        
        case(0x1):
            //D
            Buzz_PWM_WritePeriod(Buzz_D);
            Buzz_PWM_WriteCompare(Buzz_D/2);
            CyDelay(Buzz_Delay / 4);
            break;
        
        case(0x2):
            //F
            Buzz_PWM_WritePeriod(Buzz_F);
            Buzz_PWM_WriteCompare(Buzz_F/2);
            CyDelay(Buzz_Delay / 4);
        
            break;
        
        case(0x4):
            //A
            Buzz_PWM_WritePeriod(Buzz_A);
            Buzz_PWM_WriteCompare(Buzz_A/2);
            CyDelay(Buzz_Delay / 4);        
            break;
        
        case(0x8):        
            //C
            Buzz_PWM_WritePeriod(Buzz_C);
            Buzz_PWM_WriteCompare(Buzz_C/2);
            CyDelay(Buzz_Delay / 4);
            break;   
        
        default:
        break;
    }
        
    Buzz_PWM_Sleep();
}


int main()
{
    int16 counts; 
    int16 mVolts; 

    uint8 i2cReg[3]; 
    EZI2C_Start();
    EZI2C_EzI2CSetBuffer1(sizeof(i2cReg), WRITABLE, i2cReg);
    ADC_Start();
    ADC_StartConvert();
    LCD_Start();
    Buzz_PWM_Start();
    Buzz_PWM_Sleep();

    for(;;)
    {
        counts = ADC_GetResult16(POT_CHAN);
        mVolts = ADC_CountsTo_mVolts(POT_CHAN,counts);
        
        int pot_state =  (mVolts >> 10) +1;
        i2cReg[2] = pot_state;
        if (i2cReg[0] == INIT_STATE ){
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("INIT");
            LCD_Position(1,0);
            LCD_PrintString("SPEED");
            LCD_Position(1,7);
            LCD_PrintNumber(pot_state);
            
        }
        else  if (i2cReg[0] == PASS_STATE ){
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("PASS");
            Buzz_Play_SuccesMelody();
        
        }
        else  if (i2cReg[0] == FAIL_STATE ){
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("FAIL");
            Buzz_Play_DefMelody();
        }
        else{
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("RUN");
            LCD_Position(1,0);
            LCD_PrintString("SPEED");
            LCD_Position(1,7);
            LCD_PrintNumber(pot_state);
        }
        i2cReg[1] = Status_Reg_Read();
        Buzz_Play_ButtMelody(i2cReg[1]);
        if (i2cReg[0] ==0x1 || i2cReg[0] ==0x2 || i2cReg[0] ==0x4 || i2cReg[0] ==0x8){
            Buzz_Play_ButtMelody(i2cReg[0]);
        }
        
        CyDelay(100);
    }
}

