/*
 * ESP32_connection_test.c
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

#include "ESP32_connection_test.h"
#include "MW_PWM.h"
#include "MW_AnalogIn.h"
#include "ESP32_connection_test_types.h"
#include "rtwtypes.h"
#include "ESP32_connection_test_private.h"
#include <string.h>
#include "rt_nonfinite.h"

/* Block states (default storage) */
DW_ESP32_connection_test_T ESP32_connection_test_DW;

/* Real-time model */
static RT_MODEL_ESP32_connection_tes_T ESP32_connection_test_M_;
RT_MODEL_ESP32_connection_tes_T *const ESP32_connection_test_M =
  &ESP32_connection_test_M_;

/* Forward declaration for local functions */
static void ESP32_connec_SystemCore_release(codertarget_arduinobase_inter_T *obj);
static void ESP32_connec_SystemCore_release(codertarget_arduinobase_inter_T *obj)
{
  /* Start for MATLABSystem: '<Root>/PWM' */
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    obj->PWMDriverObj.MW_PWM_HANDLE = MW_PWM_GetHandle(16U);
    MW_PWM_SetDutyCycle(obj->PWMDriverObj.MW_PWM_HANDLE, -0.0);
    obj->PWMDriverObj.MW_PWM_HANDLE = MW_PWM_GetHandle(16U);
    MW_PWM_Close(obj->PWMDriverObj.MW_PWM_HANDLE);
  }

  /* End of Start for MATLABSystem: '<Root>/PWM' */
}

/* Model step function */
void ESP32_connection_test_step(void)
{
  uint16_T rtb_ManualSwitchTamb;
  uint16_T rtb_ManualSwitchu;

  /* MATLABSystem: '<Root>/PWM' incorporates:
   *  Constant: '<Root>/Constant'
   */
  ESP32_connection_test_DW.obj.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_GetHandle(16U);
  MW_PWM_SetDutyCycle(ESP32_connection_test_DW.obj.PWMDriverObj.MW_PWM_HANDLE,
                      -((real_T)ESP32_connection_test_P.Constant_Value * 255.0 /
                        255.0));

  /* MATLABSystem: '<Root>/u' */
  ESP32_connection_test_DW.obj_p.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
    MW_AnalogIn_GetHandle(15U);
  MW_AnalogInSingle_ReadResult
    (ESP32_connection_test_DW.obj_p.AnalogInDriverObj.MW_ANALOGIN_HANDLE,
     &rtb_ManualSwitchu, MW_ANALOGIN_UINT16);

  /* ManualSwitch: '<Root>/Manual Switch - u' incorporates:
   *  Gain: '<Root>/Gain'
   *  MATLABSystem: '<Root>/u'
   * */
  if (ESP32_connection_test_P.ManualSwitchu_CurrentSetting == 1) {
    rtb_ManualSwitchu = (uint16_T)(((uint32_T)ESP32_connection_test_P.Gain_Gain *
      rtb_ManualSwitchu) >> 16);
  }

  /* End of ManualSwitch: '<Root>/Manual Switch - u' */

  /* MATLABSystem: '<Root>/Tamb' */
  ESP32_connection_test_DW.obj_h.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
    MW_AnalogIn_GetHandle(32U);
  MW_AnalogInSingle_ReadResult
    (ESP32_connection_test_DW.obj_h.AnalogInDriverObj.MW_ANALOGIN_HANDLE,
     &rtb_ManualSwitchTamb, MW_ANALOGIN_UINT16);

  /* ManualSwitch: '<Root>/Manual Switch - Tamb' incorporates:
   *  Gain: '<Root>/Gain1'
   *  MATLABSystem: '<Root>/Tamb'
   * */
  if (ESP32_connection_test_P.ManualSwitchTamb_CurrentSetting == 1) {
    rtb_ManualSwitchTamb = (uint16_T)(((uint32_T)
      ESP32_connection_test_P.Gain1_Gain * rtb_ManualSwitchTamb) >> 16);
  }

  /* End of ManualSwitch: '<Root>/Manual Switch - Tamb' */
}

/* Model initialize function */
void ESP32_connection_test_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize error status */
  rtmSetErrorStatus(ESP32_connection_test_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&ESP32_connection_test_DW, 0,
                sizeof(DW_ESP32_connection_test_T));

  /* Start for MATLABSystem: '<Root>/PWM' */
  ESP32_connection_test_DW.obj.matlabCodegenIsDeleted = false;
  ESP32_connection_test_DW.objisempty_j = true;
  ESP32_connection_test_DW.obj.isInitialized = 1;
  ESP32_connection_test_DW.obj.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_Open(16U,
    5000.0, 255.0);
  ESP32_connection_test_DW.obj.isSetupComplete = true;

  /* Start for MATLABSystem: '<Root>/u' */
  ESP32_connection_test_DW.obj_p.matlabCodegenIsDeleted = false;
  ESP32_connection_test_DW.objisempty = true;
  ESP32_connection_test_DW.obj_p.isInitialized = 1;
  ESP32_connection_test_DW.obj_p.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
    MW_AnalogInSingle_Open(15U);
  ESP32_connection_test_DW.obj_p.isSetupComplete = true;

  /* Start for MATLABSystem: '<Root>/Tamb' */
  ESP32_connection_test_DW.obj_h.matlabCodegenIsDeleted = false;
  ESP32_connection_test_DW.objisempty_g = true;
  ESP32_connection_test_DW.obj_h.isInitialized = 1;
  ESP32_connection_test_DW.obj_h.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
    MW_AnalogInSingle_Open(32U);
  ESP32_connection_test_DW.obj_h.isSetupComplete = true;
}

/* Model terminate function */
void ESP32_connection_test_terminate(void)
{
  /* Terminate for MATLABSystem: '<Root>/PWM' */
  if (!ESP32_connection_test_DW.obj.matlabCodegenIsDeleted) {
    ESP32_connection_test_DW.obj.matlabCodegenIsDeleted = true;
    ESP32_connec_SystemCore_release(&ESP32_connection_test_DW.obj);
  }

  /* End of Terminate for MATLABSystem: '<Root>/PWM' */

  /* Terminate for MATLABSystem: '<Root>/u' */
  if (!ESP32_connection_test_DW.obj_p.matlabCodegenIsDeleted) {
    ESP32_connection_test_DW.obj_p.matlabCodegenIsDeleted = true;
    if ((ESP32_connection_test_DW.obj_p.isInitialized == 1) &&
        ESP32_connection_test_DW.obj_p.isSetupComplete) {
      ESP32_connection_test_DW.obj_p.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
        MW_AnalogIn_GetHandle(15U);
      MW_AnalogIn_Close
        (ESP32_connection_test_DW.obj_p.AnalogInDriverObj.MW_ANALOGIN_HANDLE);
    }
  }

  /* End of Terminate for MATLABSystem: '<Root>/u' */

  /* Terminate for MATLABSystem: '<Root>/Tamb' */
  if (!ESP32_connection_test_DW.obj_h.matlabCodegenIsDeleted) {
    ESP32_connection_test_DW.obj_h.matlabCodegenIsDeleted = true;
    if ((ESP32_connection_test_DW.obj_h.isInitialized == 1) &&
        ESP32_connection_test_DW.obj_h.isSetupComplete) {
      ESP32_connection_test_DW.obj_h.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
        MW_AnalogIn_GetHandle(32U);
      MW_AnalogIn_Close
        (ESP32_connection_test_DW.obj_h.AnalogInDriverObj.MW_ANALOGIN_HANDLE);
    }
  }

  /* End of Terminate for MATLABSystem: '<Root>/Tamb' */
}
