/*
 * ESP32_connection_test.h
 *
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * Code generation for model "ESP32_connection_test".
 *
 * Model version              : 1.12
 * Simulink Coder version : 26.1 (R2026a) 20-Nov-2025
 * C source code generated on : Wed May 13 16:41:30 2026
 *
 * Target selection: ert.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef ESP32_connection_test_h_
#define ESP32_connection_test_h_
#ifndef ESP32_connection_test_COMMON_INCLUDES_
#define ESP32_connection_test_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                              /* ESP32_connection_test_COMMON_INCLUDES_ */

#include "ESP32_connection_test_types.h"
#include <stddef.h>
#include <string.h>
#include "rt_nonfinite.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  codertarget_arduinobase_inter_T obj; /* '<Root>/PWM' */
  codertarget_arduinobase_int_i_T obj_p;/* '<Root>/u' */
  codertarget_arduinobase_int_i_T obj_h;/* '<Root>/Tamb' */
  boolean_T objisempty;                /* '<Root>/u' */
  boolean_T objisempty_g;              /* '<Root>/Tamb' */
  boolean_T objisempty_j;              /* '<Root>/PWM' */
} DW_ESP32_connection_test_T;

/* Parameters (default storage) */
struct P_ESP32_connection_test_T_ {
  uint16_T Gain1_Gain;                 /* Computed Parameter: Gain1_Gain
                                        * Referenced by: '<Root>/Gain1'
                                        */
  uint16_T Gain_Gain;                  /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<Root>/Gain'
                                        */
  uint8_T Constant_Value;              /* Computed Parameter: Constant_Value
                                        * Referenced by: '<Root>/Constant'
                                        */
  uint8_T ManualSwitchu_CurrentSetting;
                             /* Computed Parameter: ManualSwitchu_CurrentSetting
                              * Referenced by: '<Root>/Manual Switch - u'
                              */
  uint8_T ManualSwitchTamb_CurrentSetting;
                          /* Computed Parameter: ManualSwitchTamb_CurrentSetting
                           * Referenced by: '<Root>/Manual Switch - Tamb'
                           */
};

/* Real-time Model Data Structure */
struct tag_RTM_ESP32_connection_test_T {
  const char_T *errorStatus;
};

/* Block parameters (default storage) */
extern P_ESP32_connection_test_T ESP32_connection_test_P;

/* Block states (default storage) */
extern DW_ESP32_connection_test_T ESP32_connection_test_DW;

/* Model entry point functions */
extern void ESP32_connection_test_initialize(void);
extern void ESP32_connection_test_step(void);
extern void ESP32_connection_test_terminate(void);

/* Real-time Model object */
extern RT_MODEL_ESP32_connection_tes_T *const ESP32_connection_test_M;
extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'ESP32_connection_test'
 */
#endif                                 /* ESP32_connection_test_h_ */
