#include <log.h>
#include <motor.h>
#include <uart.h>
#include <ultrasonic.h>
#include <util.h>
#include "level.h"

#include "Ifx_Types.h"


#define FILTER_SIZE 7

static float g_Kp = 0.001;
static float g_Ki = 0.0;
static float g_Kd = 0.0;

static float g_error = 0;
static float g_lastError = 0;
static float g_integral;
static float g_derivative;

static int g_mv = 0;

static uint32 g_filter[FILTER_SIZE] = {0};
static int g_filterIdx= 0;
static uint32 g_total = 0;

static uint32 g_previousFilteredDistance = 0;
static uint32 g_currentFilteredDistance = 0;

void Lvl_SetGain(int n, float i){
    if(n == 0){
        g_Kp += i;
        Log_Info("cur g_Kp: %f\n", g_Kp);
    }
    if(n == 1){
            g_Ki += i;
            Log_Info("cur Ki: %f\n", g_Ki);
        }

    if(n == 2){
            g_Kd += i;
            Log_Info("cur Kd: %f\n", g_Kd);
        }
}


static uint32 getFilteredDistance (Ult_DirType dir)
{
    g_total = g_total - g_filter[g_filterIdx];
    int dis = Ult_GetDistance(dir);
    while (dis < 0)
    {
        dis = Ult_GetDistance(dir);
    }
    g_filter[g_filterIdx] = dis;
    g_total += g_filter[g_filterIdx];
    g_filterIdx = (g_filterIdx + 1) % FILTER_SIZE;
    return g_total / FILTER_SIZE;
}

void Lvl_Init (LevelDir dir)
{
    Ult_DirType ultDir = (dir == LEVEL_LEFT) ? ULT_LEFT : ULT_RIGHT;
    g_integral = 0;
    for (int i = 0; i < FILTER_SIZE; i++)
    {
        g_previousFilteredDistance = getFilteredDistance(ultDir);
        Util_DelayMs(50);
    }
}

int Lvl_Leveling (LevelDir dir)
{

    Ult_DirType ultDir = ((dir == LEVEL_LEFT) ? ULT_LEFT : ULT_RIGHT);
    g_currentFilteredDistance = getFilteredDistance(ultDir);
    g_error = (float) g_previousFilteredDistance - (float) g_currentFilteredDistance;
    g_integral = g_integral + g_error;
    g_derivative = g_error - g_lastError;
    g_mv = (int) (g_Kp * g_error + g_Ki * g_integral + g_Kd * g_derivative);
    g_lastError = g_error;
    g_previousFilteredDistance = g_currentFilteredDistance;

    Log_Debug("error: %f\tintegral: %f\tde: %f\tmv: %d\n", g_error, g_integral, g_derivative, g_mv);

    if(g_mv < -200) g_mv = -200;
    if(g_mv > 200) g_mv = 200;

    if(dir == LEVEL_RIGHT) g_mv *= -1;

    Motor_Control(MOTOR_WHEEL_LEFT, MOTOR_DIR_FORWARD, 30000 + g_mv);
    Motor_Control(MOTOR_WHEEL_RIGHT, MOTOR_DIR_FORWARD, 30000 + g_mv);

    return g_mv;
}
