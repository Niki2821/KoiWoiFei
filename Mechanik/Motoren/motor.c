/************************************************************************/
/* Author: Marc Spieler                                                 */
/* Team: noOS                                                           */
/* Created: 22.03.2019                                                  */
/************************************************************************/

#include "motor.h"
#include "asf.h"
#include "string.h"
#include "math.h"

#define CM_PER_TICK             ((2 * M_PI * 3) / 464.64)
#define ENCODER_UPDATE_RATE     (0.008) //in seconds

#define MOTOR_LEFT_SIN          (0.5f)
#define MOTOR_RIGHT_SIN         (0.5f)
#define MOTOR_REAR_SIN          (-1.0f)
#define MOTOR_LEFT_COSIN        (-0.866025404f)
#define MOTOR_RIGHT_COSIN       (0.866025404f)
#define MOTOR_REAR_COSIN        (0.0f)

pwm_channel_t pwm_motor_left;
pwm_channel_t pwm_motor_right;
pwm_channel_t pwm_motor_rear;
pwm_channel_t pwm_encoder;    // do not move - weird thing happen

pidReg_t pid_motor_left;
pidReg_t pid_motor_right;
pidReg_t pid_motor_rear;

float speed_mleft;
float speed_mright;
float speed_mrear;

int16_t mleft_out;
int16_t mright_out;
int16_t mrear_out;

float act_motor_speed_left;
float act_motor_speed_right;
float act_motor_speed_rear;
float act_motor_speed_right_raw;
float act_motor_speed_left_raw;
float act_motor_speed_rear_raw;

uint16_t log_cnt = 0;
int8_t eleft_counts_log[400];
int8_t eright_counts_log[400];
int8_t erear_counts_log[400];
int8_t speed_mleft_log[400];
int8_t speed_mright_log[400];
int8_t speed_mrear_log[400];
int16_t mleft_out_log[400];
int16_t mright_out_log[400];
int16_t mrear_out_log[400];

void motor_init(void)
{
    /* Initialize PWM channel for left motor */
    pwm_motor_left.alignment = PWM_ALIGN_LEFT;
    pwm_motor_left.polarity = PWM_LOW;
    pwm_motor_left.ul_prescaler = PWM_CMR_CPRE_CLKA;
    pwm_motor_left.ul_period = PERIOD_VALUE;
    pwm_motor_left.ul_duty = INIT_DUTY_VALUE;
    pwm_motor_left.channel = MOTOR_LEFT;
    pwm_channel_init(PWM, &pwm_motor_left);

    /* Initialize PWM channel for right motor */
    pwm_motor_right.alignment = PWM_ALIGN_LEFT;
    pwm_motor_right.polarity = PWM_LOW;
    pwm_motor_right.ul_prescaler = PWM_CMR_CPRE_CLKA;
    pwm_motor_right.ul_period = PERIOD_VALUE;
    pwm_motor_right.ul_duty = INIT_DUTY_VALUE;
    pwm_motor_right.channel = MOTOR_RIGHT;
    pwm_channel_init(PWM, &pwm_motor_right);

    /* Initialize PWM channel for rear motor */
    pwm_motor_rear.alignment = PWM_ALIGN_LEFT;
    pwm_motor_rear.polarity = PWM_LOW;
    pwm_motor_rear.ul_prescaler = PWM_CMR_CPRE_CLKA;
    pwm_motor_rear.ul_period = PERIOD_VALUE;
    pwm_motor_rear.ul_duty = INIT_DUTY_VALUE;
    pwm_motor_rear.channel = MOTOR_REAR;
    pwm_channel_init(PWM, &pwm_motor_rear);

    pwm_channel_disable(PWM, MOTOR_LEFT);
    pwm_channel_disable(PWM, MOTOR_RIGHT);
    pwm_channel_disable(PWM, MOTOR_REAR);

    /* Initialize PWM channel for encoders */
    pwm_encoder.alignment = PWM_ALIGN_LEFT;
    pwm_encoder.polarity = PWM_LOW;
    pwm_encoder.ul_prescaler = PWM_CMR_CPRE_CLKA;
    pwm_encoder.ul_period = 330;
    pwm_encoder.ul_duty = 165;
    pwm_encoder.channel = ENC_CLK;
    pwm_channel_init(PWM, &pwm_encoder);
    pwm_channel_enable(PWM, ENC_CLK);
 
    sysclk_enable_peripheral_clock(ID_TC1);
    tc_init(TC0, 1, TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_CPCTRG);
    tc_write_rc(TC0, 1, 5249);  //MCLK / 128 * 0,008
    NVIC_DisableIRQ(TC1_IRQn);
    NVIC_ClearPendingIRQ(TC1_IRQn);
    NVIC_SetPriority(TC1_IRQn, 0);
    NVIC_EnableIRQ(TC1_IRQn);
    tc_enable_interrupt(TC0, 1, TC_IER_CPCS);
    tc_start(TC0, 1);

    pid_motor_left.kp = 30.0f; //10
    pid_motor_left.ki = 5.0f; //0.7
    pid_motor_left.kc = 1.0f; //1
    pid_motor_left.kd = 10.0f; //0
    pid_motor_left.outMin = -500.0f;
    pid_motor_left.outMax = 500.0f;

    pid_motor_right = pid_motor_left;
    pid_motor_rear = pid_motor_left;
}

void enable_motor(void)
{
    pid_motor_left.intg = 0.0f;
    pid_motor_left.prevErr = 0.0f;
    pid_motor_left.satErr = 0.0f;
    
    pid_motor_right.intg = 0.0f;
    pid_motor_right.prevErr = 0.0f;
    pid_motor_right.satErr = 0.0f;
    
    pid_motor_rear.intg = 0.0f;
    pid_motor_rear.prevErr = 0.0f;
    pid_motor_rear.satErr = 0.0f;
    
    pwm_channel_enable(PWM, MOTOR_LEFT);
    pwm_channel_enable(PWM, MOTOR_RIGHT);
    pwm_channel_enable(PWM, MOTOR_REAR);

    ioport_set_pin_level(ENC_LOAD, 0);
    ioport_set_pin_level(ENC_LOAD, 1);

    tc_enable_interrupt(TC0, 1, TC_IER_CPCS);
}

void disable_motor(void)
{
    pwm_channel_disable(PWM, MOTOR_LEFT);
    pwm_channel_disable(PWM, MOTOR_RIGHT);
    pwm_channel_disable(PWM, MOTOR_REAR);

    tc_disable_interrupt(TC0, 1, TC_IER_CPCS);
}

void set_motor(float speed, float dir, float trn)
{
    float left;
    float right;
    float rear;
    
    dir *= (3.14159265359f / 180.0f);
    
    left = speed * (cos(dir) * MOTOR_LEFT_COSIN - sin(dir) * MOTOR_LEFT_SIN);
    right = speed * (cos(dir) * MOTOR_RIGHT_COSIN - sin(dir) * MOTOR_RIGHT_SIN);
    rear = speed * (cos(dir) * MOTOR_REAR_COSIN - sin(dir) * MOTOR_REAR_SIN);
    
    left += trn;
    right += trn;
    rear += trn;
    
    set_motor_individual(left, right, rear);
}

void set_motor_extended(float speed, float dir, float trn, float left, float right, float rear)
{
    dir *= (3.14159265359f / 180.0f);
    
    left += speed * (cos(dir) * MOTOR_LEFT_COSIN - sin(dir) * MOTOR_LEFT_SIN);
    right += speed * (cos(dir) * MOTOR_RIGHT_COSIN - sin(dir) * MOTOR_RIGHT_SIN);
    rear += speed * (cos(dir) * MOTOR_REAR_COSIN - sin(dir) * MOTOR_REAR_SIN);
    
    left += trn;
    right += trn;
    rear += trn;
    
    set_motor_individual(left, right, rear);
}

void set_motor_individual(float left, float right, float rear)
{
    /* compensate motor output */
    float motor[3] = {left, right, rear};

    if(motor[0] > MAX_MOTOR_SPEED || motor[1] > MAX_MOTOR_SPEED || motor[2] > MAX_MOTOR_SPEED)
    {
        uint8_t highest_value = 0;
        uint8_t highest_motor = 0;

        for(int i = 0; i < 3; i++)
        {
            if(motor[i] > highest_value)
            {
                highest_value = motor[i];
                highest_motor = i;
            }
        }

        float factor = motor[highest_motor] / MAX_MOTOR_SPEED;
        left /= factor;
        right /= factor;
        rear /= factor;
    }

    /* Convert cm/s in ticks per loop */
    left /= (CM_PER_TICK / ENCODER_UPDATE_RATE);
    right /= (CM_PER_TICK / ENCODER_UPDATE_RATE);
    rear /= (CM_PER_TICK / ENCODER_UPDATE_RATE);

    /* update PID input values */
    tc_disable_interrupt(TC0, 1, TC_IER_CPCS);
    speed_mleft = (float)left;
    speed_mright = (float)right;
    speed_mrear = (float)rear;
    tc_enable_interrupt(TC0, 1, TC_IER_CPCS);
}

void update_motor_pwm(uint8_t motor, int16_t ispeed)
{
    if (ispeed > 500)
    {
        ispeed = 500;
    }

    if (ispeed < -500)
    {
        ispeed = -500;
    }

    uint16_t duty_cycle = (uint16_t)(ispeed + (int16_t)INIT_DUTY_VALUE);

    if (duty_cycle < 10)
    {
        duty_cycle = 10;
    }

    if (duty_cycle > 990)
    {
        duty_cycle = 990;
    }

    switch(motor)
    {
        case MOTOR_LEFT:
            pwm_channel_update_duty(PWM, &pwm_motor_left, duty_cycle);
            break;
        case MOTOR_RIGHT:
            pwm_channel_update_duty(PWM, &pwm_motor_right, duty_cycle);
            break;
        case MOTOR_REAR:
            pwm_channel_update_duty(PWM, &pwm_motor_rear, duty_cycle);
            break;
        default:
            break;
    }
}

void TC1_Handler(void)
{
    uint32_t PIOC_value;
    int8_t left_enc_counts;
    int8_t right_enc_counts;
    int8_t rear_enc_counts;

    //ioport_set_pin_level(LED_M3, 1);

    if ((tc_get_status(TC0, 1) & TC_SR_CPCS) == TC_SR_CPCS)
    {
        pwm_channel_disable(PWM, ENC_CLK);
        PIOC_value = ioport_get_port_level(IOPORT_PIOC, 0xFFFFFFFF);
        ioport_set_pin_level(ENC_LOAD, 0);
        ioport_set_pin_level(ENC_LOAD, 1);
        pwm_channel_enable(PWM, ENC_CLK);

        left_enc_counts = (PIOC_value & 0x7F000000) >> 24;
        left_enc_counts = (left_enc_counts & 0x00000040) ? left_enc_counts - 128 : left_enc_counts;
        right_enc_counts = ((PIOC_value & 0x00C00000) >> 17) | ((PIOC_value & 0x001F0000) >> 16);
        right_enc_counts = (right_enc_counts & 0x00000040) ? right_enc_counts - 128 : right_enc_counts;
        rear_enc_counts = ((PIOC_value & 0x0000FC00) >> 9) | ((PIOC_value & 0x00000002) >> 1);
        rear_enc_counts = (rear_enc_counts & 0x00000040) ? rear_enc_counts - 128 : rear_enc_counts;

        mleft_out = pidReg(&pid_motor_left, speed_mleft, (float)left_enc_counts);
        mright_out = pidReg(&pid_motor_right, speed_mright, (float)right_enc_counts);
        mrear_out = pidReg(&pid_motor_rear, speed_mrear, (float)rear_enc_counts);
        update_motor_pwm(MOTOR_LEFT, mleft_out);
        update_motor_pwm(MOTOR_RIGHT, mright_out);
        update_motor_pwm(MOTOR_REAR, mrear_out);

        act_motor_speed_left_raw = (float)left_enc_counts * (CM_PER_TICK / ENCODER_UPDATE_RATE);
        act_motor_speed_right_raw = (float)right_enc_counts * (CM_PER_TICK / ENCODER_UPDATE_RATE);
        act_motor_speed_rear_raw = (float)rear_enc_counts * (CM_PER_TICK / ENCODER_UPDATE_RATE);
        act_motor_speed_left = act_motor_speed_left_raw * 0.1 + act_motor_speed_left * 0.9;
        act_motor_speed_right = act_motor_speed_right_raw * 0.1 + act_motor_speed_right * 0.9;
        act_motor_speed_rear = act_motor_speed_rear_raw * 0.1 + act_motor_speed_rear * 0.9;

        /*if(log_cnt < 400)
        {
            eleft_counts_log[log_cnt] = eleft_counts;
            eright_counts_log[log_cnt] = eright_counts;
            erear_counts_log[log_cnt] = erear_counts;
            speed_mleft_log[log_cnt] = (int8_t)speed_mleft;
            speed_mright_log[log_cnt] = (int8_t)speed_mright;
            speed_mrear_log[log_cnt] = (int8_t)speed_mrear;
            mleft_out_log[log_cnt] = mleft_out;
            mright_out_log[log_cnt] = mright_out;
            mrear_out_log[log_cnt] = mrear_out;
            log_cnt++;
        }
        else
        {
            tc_disable_interrupt(TC0, 1, TC_IER_CPCS);
            disable_motor();
            UINT bw;
            FIL file_object;
            char test_file_name[] = "motor_logging_p10_i0.7_c1_d0_+-5_teppich.txt";
            char sprintf_buf[11];
            f_open(&file_object, (char const *)test_file_name, FA_CREATE_ALWAYS | FA_WRITE);
            
            for(int i = 0; i < log_cnt; i++)
            {
                sprintf(sprintf_buf, "%5d;", i);
                f_write(&file_object, sprintf_buf, strlen(sprintf_buf), &bw);
                sprintf(sprintf_buf, "%3d;", eleft_counts_log[i]);
                f_write(&file_object, sprintf_buf, strlen(sprintf_buf), &bw);
                sprintf(sprintf_buf, "%3d;", eright_counts_log[i]);
                f_write(&file_object, sprintf_buf, strlen(sprintf_buf), &bw);
                sprintf(sprintf_buf, "%3d;", erear_counts_log[i]);
                f_write(&file_object, sprintf_buf, strlen(sprintf_buf), &bw);
                sprintf(sprintf_buf, "%3d;", speed_mleft_log[i]);
                f_write(&file_object, sprintf_buf, strlen(sprintf_buf), &bw);
                sprintf(sprintf_buf, "%3d;", speed_mright_log[i]);
                f_write(&file_object, sprintf_buf, strlen(sprintf_buf), &bw);
                sprintf(sprintf_buf, "%3d;", speed_mrear_log[i]);
                f_write(&file_object, sprintf_buf, strlen(sprintf_buf), &bw);
                sprintf(sprintf_buf, "%4d;", mleft_out_log[i]);
                f_write(&file_object, sprintf_buf, strlen(sprintf_buf), &bw);
                sprintf(sprintf_buf, "%4d;", mright_out_log[i]);
                f_write(&file_object, sprintf_buf, strlen(sprintf_buf), &bw);
                sprintf(sprintf_buf, "%4d\r\n", mrear_out_log[i]);
                f_write(&file_object, sprintf_buf, strlen(sprintf_buf), &bw);
            }
            f_close(&file_object);
            ioport_set_pin_level(LED_ONBOARD, 1);
            while(1);
        }*/
        //pwm_channel_enable(PWM, ENC_CLK);
    }

    //ioport_set_pin_level(LED_M3, 0);
}