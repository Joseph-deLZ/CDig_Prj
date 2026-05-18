/*
 * ESP32_connection_test_types.h
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

#ifndef ESP32_connection_test_types_h_
#define ESP32_connection_test_types_h_
#include "MW_SVD.h"
#include "rtwtypes.h"
#ifndef struct_tag_THwWBY9YFNkNDxiODvU89D
#define struct_tag_THwWBY9YFNkNDxiODvU89D

struct tag_THwWBY9YFNkNDxiODvU89D
{
  MW_Handle_Type MW_PWM_HANDLE;
};

#endif                                 /* struct_tag_THwWBY9YFNkNDxiODvU89D */

#ifndef typedef_e_matlabshared_ioclient_perip_T
#define typedef_e_matlabshared_ioclient_perip_T

typedef struct tag_THwWBY9YFNkNDxiODvU89D e_matlabshared_ioclient_perip_T;

#endif                             /* typedef_e_matlabshared_ioclient_perip_T */

#ifndef struct_tag_c1LrmxoqxwjoXkZhzKTz4G
#define struct_tag_c1LrmxoqxwjoXkZhzKTz4G

struct tag_c1LrmxoqxwjoXkZhzKTz4G
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  e_matlabshared_ioclient_perip_T PWMDriverObj;
};

#endif                                 /* struct_tag_c1LrmxoqxwjoXkZhzKTz4G */

#ifndef typedef_codertarget_arduinobase_inter_T
#define typedef_codertarget_arduinobase_inter_T

typedef struct tag_c1LrmxoqxwjoXkZhzKTz4G codertarget_arduinobase_inter_T;

#endif                             /* typedef_codertarget_arduinobase_inter_T */

#ifndef struct_tag_nAWbnnOgZuxKWuTGXwrCG
#define struct_tag_nAWbnnOgZuxKWuTGXwrCG

struct tag_nAWbnnOgZuxKWuTGXwrCG
{
  MW_Handle_Type MW_ANALOGIN_HANDLE;
};

#endif                                 /* struct_tag_nAWbnnOgZuxKWuTGXwrCG */

#ifndef typedef_e_arduinodriver_ArduinoAnalog_T
#define typedef_e_arduinodriver_ArduinoAnalog_T

typedef struct tag_nAWbnnOgZuxKWuTGXwrCG e_arduinodriver_ArduinoAnalog_T;

#endif                             /* typedef_e_arduinodriver_ArduinoAnalog_T */

#ifndef struct_tag_gXF6Q3iquGlwRgoZrA70wG
#define struct_tag_gXF6Q3iquGlwRgoZrA70wG

struct tag_gXF6Q3iquGlwRgoZrA70wG
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  e_arduinodriver_ArduinoAnalog_T AnalogInDriverObj;
};

#endif                                 /* struct_tag_gXF6Q3iquGlwRgoZrA70wG */

#ifndef typedef_codertarget_arduinobase_int_i_T
#define typedef_codertarget_arduinobase_int_i_T

typedef struct tag_gXF6Q3iquGlwRgoZrA70wG codertarget_arduinobase_int_i_T;

#endif                             /* typedef_codertarget_arduinobase_int_i_T */

/* Parameters (default storage) */
typedef struct P_ESP32_connection_test_T_ P_ESP32_connection_test_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_ESP32_connection_test_T RT_MODEL_ESP32_connection_tes_T;

#endif                                 /* ESP32_connection_test_types_h_ */
