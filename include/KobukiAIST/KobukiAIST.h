// -*- C++ -*-
/*!
 * @file  KobukiAIST.h
 * @brief Kobuki RTC by AIST
 * @date  $Date$
 *
 * $Id$
 */

#ifndef KOBUKIAIST_H
#define KOBUKIAIST_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>
#include <rtm/idl/InterfaceDataTypesSkel.h>

#include <libkobuki.h>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

/*!
 * @class KobukiAIST
 * @brief Kobuki RTC by AIST
 *
 */
class KobukiAIST
  : public RTC::DataFlowComponentBase
{
 public:
  /*!
   * @brief constructor
   * @param manager Maneger Object
   */
  KobukiAIST(RTC::Manager* manager);

  /*!
   * @brief destructor
   */
  ~KobukiAIST();

  // <rtc-template block="public_attribute">
  
  // </rtc-template>

  // <rtc-template block="public_operation">
  
  // </rtc-template>

  /***
   *
   * The initialize action (on CREATED->ALIVE transition)
   * formaer rtc_init_entry() 
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onInitialize();

  /***
   *
   * The finalize action (on ALIVE->END transition)
   * formaer rtc_exiting_entry()
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onFinalize();

  /***
   *
   * The startup action when ExecutionContext startup
   * former rtc_starting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  /***
   *
   * The shutdown action when ExecutionContext stop
   * former rtc_stopping_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  /***
   *
   * The activated action (Active state entry action)
   * former rtc_active_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  /***
   *
   * The deactivated action (Active state exit action)
   * former rtc_active_exit()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  /***
   *
   * The execution action that is invoked periodically
   * former rtc_active_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
   virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  /***
   *
   * The aborting action when main logic error occurred.
   * former rtc_aborting_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  /***
   *
   * The error action in ERROR state
   * former rtc_error_do()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  /***
   *
   * The reset action that is invoked resetting
   * This is same but different the former rtc_init_entry()
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  /***
   *
   * The state update action that is invoked after onExecute() action
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  /***
   *
   * The action that is invoked when execution context's rate is changed
   * no corresponding operation exists in OpenRTm-aist-0.2.0
   *
   * @param ec_id target ExecutionContext Id
   *
   * @return RTC::ReturnCode_t
   * 
   * 
   */
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // <rtc-template block="protected_attribute">
  
  // </rtc-template>

  // <rtc-template block="protected_operation">
  
  // </rtc-template>

  // Configuration variable declaration
  // <rtc-template block="config_declare">
  /*!
   * 
   * - Name:  serialPort
   * - DefaultValue: /dev/ttyUSB0
   */
  std::string m_serialPort;
  /*!
   * 
   * - Name:  debug
   * - DefaultValue: 0
   */
  bool m_debug;
  /*!
   * 
   * - Name:  dock
   * - DefaultValue: 0
   */
  bool m_dock;
  bool m_dockPre;
  /*!
   * Status of LED1
   * OFF: off
   * RED: turn on red
   * GREEN: turn on green
   * - Name: Status of LED1 led1
   * - DefaultValue: OFF
   * - Unit: None
   * - Range: (OFF,RED,GREEN)
   * - Constraint: (OFF,RED,GREEN)
   */
  std::string m_led1;
  std::string m_led1Pre;
  /*!
   * Status of LED1
   * OFF: off
   * RED: turn on red
   * GREEN: turn on green
   * - Name: Status of LED1 led2
   * - DefaultValue: OFF
   * - Unit: None
   * - Range: (OFF,RED,GREEN)
   * - Constraint: (OFF,RED,GREEN)
   */
  std::string m_led2;
  std::string m_led2Pre;
  /*!
   * power output status
   * - Name: power status power
   * - DefaultValue: 3.3V,5.0V,12V1A,12V5A
   * - Unit: None
   * - Range: (3.3V,5.0V,12V1A,12V5A)
   * - Constraint: (3.3V,5.0V,12V1A,12V5A)
   */
  std::string m_power;
#define POWER_CH 4
  bool m_prePower[POWER_CH];
  /*!
   * GPIO output
   * - Name: GPIO output gpio
   * - DefaultValue: ,,,
   * - Unit: None
   * - Range: (CH0,CH1,CH2,CH3)
   * - Constraint: (CH0,CH1,CH2,CH3)
   */
  std::string m_gpio;
#define GPIO_CH 4
  bool m_preGPIO[GPIO_CH];

  // </rtc-template>

  // DataInPort declaration
  // <rtc-template block="inport_declare">
  RTC::TimedVelocity2D m_targetVelocity;
  /*!
   */
  InPort<RTC::TimedVelocity2D> m_targetVelocityIn;
  RTC::TimedPose2D m_poseUpdate;
  /*!
   */
  InPort<RTC::TimedPose2D> m_poseUpdateIn;
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  RTC::TimedPose2D m_currentPose;
  /*!
   */
  OutPort<RTC::TimedPose2D> m_currentPoseOut;
  RTC::TimedDouble m_battery;
  /*!
   */
  OutPort<RTC::TimedDouble> m_batteryOut;
  RTC::TimedBooleanSeq m_bumper;
  /*!
   */
  OutPort<RTC::TimedBooleanSeq> m_bumperOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
  enum KobukiSensors
	{
      RIGHT_BUMPER,
      CENTER_BUMPER,
      LEFT_BUMPER,
      RIGHT_WHEEL_DROP,
      LEFT_WHEEL_DROP,
      RIGHT_CLIFF,
      CENTER_CLIFF,
      LEFT_CLIFF,
      RIGHT_IRFAR_RIGHT,
      RIGHT_IRFAR_CENTER,
      RIGHT_IRFAR_LEFT,
      RIGHT_IRNEAR_RIGHT,
      RIGHT_IRNEAR_CENTER,
      RIGHT_IRNEAR_LEFT,
      CENTER_IRFAR_RIGHT,
      CENTER_IRFAR_CENTER,
      CENTER_IRFAR_LEFT,
      CENTER_IRNEAR_RIGHT,
      CENTER_IRNEAR_CENTER,
      CENTER_IRNEAR_LEFT,
      LEFT_IRFAR_RIGHT,
      LEFT_IRFAR_CENTER,
      LEFT_IRFAR_LEFT,
      LEFT_IRNEAR_RIGHT,
      LEFT_IRNEAR_CENTER,
      LEFT_IRNEAR_LEFT,
	  KOBUKI_DOCKED,
      KOBUKI_SENSOR_NUM
	};

  rt_net::Kobuki* m_kobuki;
  // <rtc-template block="private_attribute">
  
  // </rtc-template>

  ReturnCode_t targetVelocityRead();
  ReturnCode_t positionPoseOut();
  ReturnCode_t batteryStatusOut();
  ReturnCode_t sensorStatusOut();
  ReturnCode_t setLED1();
  ReturnCode_t setLED2();
  ReturnCode_t setPower();
  ReturnCode_t setGPIO();
  ReturnCode_t setDock();
  // <rtc-template block="private_operation">
  
  // </rtc-template>

};


extern "C"
{
  DLL_EXPORT void KobukiAISTInit(RTC::Manager* manager);
};

#endif // KOBUKIAIST_H
