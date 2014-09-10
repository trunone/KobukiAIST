// -*- C++ -*-
/*!
 * @file  KobukiAIST.cpp
 * @brief Kobuki RTC by AIST
 * @date $Date$
 *
 * $Id$
 */

#include "KobukiAIST.h"

namespace coil
{
  template <>
  bool stringTo<bool>(bool& val, const char* str)
  {
    if (str == 0) { return false; }
    std::string boolstr(str);
    coil::normalize(boolstr);
    if (boolstr == "true" || boolstr == "1" ||
        boolstr == "yes"  || boolstr == "on")
      {
        val = true;
        return true;
      }
    else if (boolstr == "false" || boolstr == "0" ||
             boolstr == "no"    || boolstr == "off")
      {
        val = false;
        return true;
      }
    return false;
  }
};

// Module specification
// <rtc-template block="module_spec">
static const char* kobukiaist_spec[] =
  {
    "implementation_id", "KobukiAIST",
    "type_name",         "KobukiAIST",
    "description",       "Kobuki RTC by AIST",
    "version",           "1.0.0",
    "vendor",            "AIST",
    "category",          "Category",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.serialPort", "/dev/ttyUSB0",
    "conf.default.debug", "OFF",
    "conf.default.dock", "OFF",
    "conf.default.led1", "OFF",
    "conf.default.led2", "OFF",
    "conf.default.power", "3.3V,5.0V,12V1A,12V5A",
    "conf.default.gpio", "",
    // Widget
    "conf.__widget__.serialPort", "text",
    "conf.__widget__.debug", "radio",
    "conf.__widget__.dock", "radio",
    "conf.__widget__.led1", "radio",
    "conf.__widget__.led2", "radio",
    "conf.__widget__.power", "checkbox",
    "conf.__widget__.gpio", "checkbox",
    // Constraints
    "conf.__constraints__.debug", "(ON,OFF)",
    "conf.__constraints__.dock", "(ON,OFF)",
    "conf.__constraints__.led1", "(OFF,RED,GREEN)",
    "conf.__constraints__.led2", "(OFF,RED,GREEN)",
    "conf.__constraints__.power", "(3.3V,5.0V,12V1A,12V5A)",
    "conf.__constraints__.gpio", "(CH0,CH1,CH2,CH3)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
KobukiAIST::KobukiAIST(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_serialPort("/dev/ttyUSB0"), m_debug(false),
    m_dock(false), m_dockPre(false),
    m_led1("OFF"), m_led1Pre("OFF"), m_led2("OFF"), m_led2Pre("OFF"),
    m_power("3.3V,5.0V,12V1A,12V5A"), m_gpio(""),
    m_targetVelocityIn("targetVelocity", m_targetVelocity),
    m_poseUpdateIn("poseUpdate", m_poseUpdate),
    m_currentPoseOut("currentPose", m_currentPose),
    m_batteryOut("battery", m_battery),
    m_bumperOut("bumper", m_bumper)

    // </rtc-template>
{
  for (int i(0); i < POWER_CH; ++i) { m_prePower[i] = true; }
  for (int i(0); i < GPIO_CH; ++i)  { m_preGPIO[i] = true; }
}

/*!
 * @brief destructor
 */
KobukiAIST::~KobukiAIST()
{
}



RTC::ReturnCode_t KobukiAIST::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("targetVelocity", m_targetVelocityIn);
  addInPort("poseUpdate", m_poseUpdateIn);
  
  // Set OutPort buffer
  addOutPort("currentPose", m_currentPoseOut);
  addOutPort("battery", m_batteryOut);
  addOutPort("bumper", m_bumperOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>
  bool toBool(std::string str, std::string yes, std::string no,
              bool default_value = true);
  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("serialPort", m_serialPort, "/dev/ttyUSB0");
  bindParameter("debug", m_debug, "OFF");
  bindParameter("dock", m_dock, "OFF");
  bindParameter("led1", m_led1, "OFF");
  bindParameter("led2", m_led2, "OFF");
  bindParameter("power", m_power, "3.3V,5.0V,12V1A,12V5A");
  bindParameter("gpio", m_gpio, "");
  // </rtc-template>
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t KobukiAIST::onFinalize()
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t KobukiAIST::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiAIST::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t KobukiAIST::onActivated(RTC::UniqueId ec_id)
{
  rt_net::KobukiStringArgument arg(m_serialPort);
  m_kobuki = createKobuki(arg);
  return RTC::RTC_OK;
}


RTC::ReturnCode_t KobukiAIST::onDeactivated(RTC::UniqueId ec_id)
{
  delete m_kobuki;
  m_kobuki = NULL;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t KobukiAIST::onExecute(RTC::UniqueId ec_id)
{
  setLED1();
  setLED2();
  setPower();
  setGPIO();
  setDock();
  ReturnCode_t ret(RTC::RTC_OK);
  ReturnCode_t tmp(RTC::RTC_OK);
  if ((tmp = targetVelocityRead()) != RTC::RTC_OK) { ret = tmp; }
  if ((tmp = positionPoseOut())    != RTC::RTC_OK) { ret = tmp; }
  if ((tmp = batteryStatusOut())   != RTC::RTC_OK) { ret = tmp; }
  if ((tmp = sensorStatusOut())    != RTC::RTC_OK) { ret = tmp; }
  return ret;
}

/*
RTC::ReturnCode_t KobukiAIST::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiAIST::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiAIST::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiAIST::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KobukiAIST::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

RTC::ReturnCode_t KobukiAIST::targetVelocityRead()
{
  if (m_targetVelocityIn.isNew())
    {
      m_targetVelocityIn.read();
      double vx = m_targetVelocity.data.vx;
      double va = m_targetVelocity.data.va;
      m_debug ? printf("Target velocity: (vx, va) = (%f, %f)\n", vx, va) : true;
      m_kobuki->setTargetVelocity(vx, va);
    }
  return RTC::RTC_OK;
}

RTC::ReturnCode_t KobukiAIST::positionPoseOut()
{
  double x (m_kobuki->getPoseX());
  double y (m_kobuki->getPoseY());
  double th(m_kobuki->getPoseTh());
  setTimestamp(m_currentPose);

  m_currentPose.data.position.x = x;
  m_currentPose.data.position.y = y;
  m_currentPose.data.heading    = th;
  m_debug ? printf("Current Position: (x, y, th) = (%f, %f, %f)",
                   x, y, th) : true;
  m_currentPoseOut.write();

  return RTC::RTC_OK;
}

RTC::ReturnCode_t KobukiAIST::batteryStatusOut()
{
  double battery(m_kobuki->getBatteryVoltage());
  setTimestamp(m_battery);

  m_battery.data = battery;
  m_debug ? printf("Battery Voltage: %f\n", battery) : true;
  m_batteryOut.write();
  
  return RTC::RTC_OK;
}

RTC::ReturnCode_t KobukiAIST::sensorStatusOut()
{
//  if (m_bumperOut.getStatusList().size() != 0 || m_debug != 0)
//    {
      m_bumper.data.length(KOBUKI_SENSOR_NUM);
      m_bumper.data[RIGHT_BUMPER]         = m_kobuki->isRightBump();
      m_bumper.data[CENTER_BUMPER]        = m_kobuki->isCenterBump();
      m_bumper.data[LEFT_BUMPER]          = m_kobuki->isLeftBump();
      m_bumper.data[RIGHT_WHEEL_DROP]     = m_kobuki->isRightWheelDrop();
      m_bumper.data[LEFT_WHEEL_DROP]      = m_kobuki->isLeftWheelDrop();
      m_bumper.data[RIGHT_CLIFF]          = m_kobuki->isRightCliff();
      m_bumper.data[CENTER_CLIFF]         = m_kobuki->isCenterCliff();
      m_bumper.data[LEFT_CLIFF]           = m_kobuki->isLeftCliff();
      m_bumper.data[RIGHT_IRFAR_RIGHT]    = m_kobuki->isRightIRFarRight();
      m_bumper.data[RIGHT_IRFAR_CENTER]   = m_kobuki->isRightIRFarCenter();
      m_bumper.data[RIGHT_IRFAR_LEFT]     = m_kobuki->isRightIRFarLeft();
      m_bumper.data[RIGHT_IRNEAR_RIGHT]   = m_kobuki->isRightIRNearRight();
      m_bumper.data[RIGHT_IRNEAR_CENTER]  = m_kobuki->isRightIRNearCenter();
      m_bumper.data[RIGHT_IRNEAR_LEFT]    = m_kobuki->isRightIRNearLeft();
      m_bumper.data[CENTER_IRFAR_RIGHT]   = m_kobuki->isCenterIRFarRight();
      m_bumper.data[CENTER_IRFAR_CENTER]  = m_kobuki->isCenterIRFarCenter();
      m_bumper.data[CENTER_IRFAR_LEFT]    = m_kobuki->isCenterIRFarLeft();
      m_bumper.data[CENTER_IRNEAR_RIGHT]  = m_kobuki->isCenterIRNearRight();
      m_bumper.data[CENTER_IRNEAR_CENTER] = m_kobuki->isCenterIRNearCenter();
      m_bumper.data[CENTER_IRNEAR_LEFT]   = m_kobuki->isCenterIRNearLeft();
      m_bumper.data[LEFT_IRFAR_RIGHT]     = m_kobuki->isLeftIRFarRight();
      m_bumper.data[LEFT_IRFAR_CENTER]    = m_kobuki->isLeftIRFarCenter();
      m_bumper.data[LEFT_IRFAR_LEFT]      = m_kobuki->isLeftIRFarLeft();
      m_bumper.data[LEFT_IRNEAR_RIGHT]    = m_kobuki->isLeftIRNearRight();
      m_bumper.data[LEFT_IRNEAR_CENTER]   = m_kobuki->isLeftIRNearCenter();
      m_bumper.data[LEFT_IRNEAR_LEFT]     = m_kobuki->isLeftIRNearLeft();
      m_bumper.data[KOBUKI_DOCKED]        = m_kobuki->isDocked();
      setTimestamp(m_bumper);
      m_debug ? printf("Bumper: %d %d %d\n",
                       m_bumper.data[RIGHT_BUMPER],
                       m_bumper.data[CENTER_BUMPER],
                       m_bumper.data[LEFT_BUMPER]) : true;
      m_debug ? printf("Wheel: %d %d\n",
                       m_bumper.data[RIGHT_WHEEL_DROP],
                       m_bumper.data[LEFT_WHEEL_DROP]) : true;

      m_debug ? printf("Cliff: %d %d %d\n",
                       m_bumper.data[RIGHT_CLIFF],
                       m_bumper.data[CENTER_CLIFF],
                       m_bumper.data[LEFT_CLIFF]) : true;
      m_bumperOut.write();
//    }
  return RTC::RTC_OK;
}

RTC::ReturnCode_t KobukiAIST::setLED1()
{
  m_led1 = coil::normalize(m_led1);
  if (m_led1 == m_led1Pre) { return RTC::RTC_OK; }
  m_led1Pre = m_led1;

  if (m_led1 == "off")
    {
      RTC_INFO(("LED1 status changed: OFF"));
      m_debug ? printf("LED1 status changed: OFF\n") : true;
      m_kobuki->setLED1(RED, false);
    }
  else if (m_led1 == "red")
    {
      RTC_INFO(("LED1 status changed: RED"));
      m_debug ? printf("LED1 status changed: RED\n") : true;
      m_kobuki->setLED1(RED, true);
    }
  else if (m_led1 == "green")
    {
      RTC_INFO(("LED1 status changed: GREEN"));
      m_debug ? printf("LED1 status changed: GREEN\n") : true;
      m_kobuki->setLED1(GREEN, true);
    }
  else
    {
      RTC_WARN(("LED1 status changed: Unknown"));
      m_debug ? printf("LED1 status changed: Unknown\n") : true;
      m_kobuki->setLED1(RED, false);
    }
  return RTC::RTC_OK;
}

RTC::ReturnCode_t KobukiAIST::setLED2()
{
  m_led2 = coil::normalize(m_led2);
  if (m_led2 == m_led2Pre) { return RTC::RTC_OK; }
  m_led2Pre = m_led2;

  if (m_led2 == "off")
    {
      RTC_INFO(("LED2 status changed: OFF"));
      m_debug ? printf("LED2 status changed: OFF\n") : true;
      m_kobuki->setLED2(RED, false);
    }
  else if (m_led2 == "red")
    {
      RTC_INFO(("LED2 status changed: RED"));
      m_debug ? printf("LED2 status changed: RED\n") : true;
      m_kobuki->setLED2(RED, true);
    }
  else if (m_led2 == "green")
    {
      RTC_INFO(("LED2 status changed: GREEN"));
      m_debug ? printf("LED2 status changed: GREEN\n") : true;
      m_kobuki->setLED2(GREEN, true);
    }
  else
    {
      RTC_WARN(("LED2 status changed: Unknown"));
      m_debug ? printf("LED2 status changed: Unknown\n") : true;
      m_kobuki->setLED2(RED, false);
    }
  return RTC::RTC_OK;
}

RTC::ReturnCode_t KobukiAIST::setPower()
{
  POWER powerEnum[] = {POWER_33V, POWER_50V, POWER_12V1A, POWER_12V5A};
  const char* power_str[] = {"3.3v", "5.0v", "12v1a", "12v5a"};
  bool currPower[] = {false, false, false, false};

  m_power = coil::normalize(m_power);
  coil::vstring active_ch = coil::split(m_power, ",");

  // It creates power[4] = {t/f, t/f, t/f, t/f} from config param
  for (size_t i(0); i < active_ch.size(); ++i)
    {
      if      (active_ch[i] == power_str[0]) { currPower[0] = true; }
      else if (active_ch[i] == power_str[1]) { currPower[1] = true; }
      else if (active_ch[i] == power_str[2]) { currPower[2] = true; }
      else if (active_ch[i] == power_str[3]) { currPower[3] = true; }
      else { RTC_WARN(("Unknown GPIO channel: %s", active_ch[i].c_str())); }
    }
  // m_prePower[i]: previous val, currPower[i]: current val 
  for (int i(0); i < POWER_CH; ++i)
    {
      if (m_prePower[i] != currPower[i])
        {
          RTC_INFO(("Power channel %d turned %s",
                    i, currPower[i] ? "ON" : "OFF"));
          m_debug ? printf("Power channel %d turned %s\n",
                           i, currPower[i] ? "ON" : "OFF") : true;
          m_kobuki->setExternalPower(powerEnum[i], currPower[i]);
          m_prePower[i] = currPower[i];
        }
    }
  return RTC::RTC_OK;
}

RTC::ReturnCode_t KobukiAIST::setGPIO()
{
  GPIO gpioEnum[] = {GPIO_CH0, GPIO_CH1, GPIO_CH2, GPIO_CH3};
  const char* gpioStr[] = {"ch0", "ch1", "ch2", "ch3"};
  bool currGPIO[] = {false, false, false, false};

  m_gpio = coil::normalize(m_gpio);
  coil::vstring gpioConfig = coil::split(m_gpio, ",");

  // It creates gpio[4] = {t/f, t/f, t/f, t/f} from config param
  for (size_t i(0); i < gpioConfig.size(); ++i)
    {
      if      (gpioConfig[i] == gpioStr[0]) { currGPIO[0] = true; }
      else if (gpioConfig[i] == gpioStr[1]) { currGPIO[1] = true; }
      else if (gpioConfig[i] == gpioStr[2]) { currGPIO[2] = true; }
      else if (gpioConfig[i] == gpioStr[3]) { currGPIO[3] = true; }
      else { RTC_WARN(("Unknown GPIO channel: %s", gpioConfig[i].c_str())); }
    }
  // m_preGPIO[i]: previous val, gpio[i]: current val 
  for (int i(0); i < GPIO_CH; ++i)
    {
      if (m_preGPIO[i] != currGPIO[i])
        {
          RTC_INFO(("GPIO channel %d turned %s",
                    i, currGPIO[i] ? "ON" : "OFF"));
          m_debug ? printf("GPIO channel %d turned %s\n",
                           i, currGPIO[i] ? "ON" : "OFF") : true;
          m_kobuki->setDigitalOut(gpioEnum[i], currGPIO[i]);
          m_preGPIO[i] = currGPIO[i];
        }
    }
  return RTC::RTC_OK;
}

RTC::ReturnCode_t KobukiAIST::setDock()
{
  if (m_dock == m_dockPre) { return RTC::RTC_OK; }
  // DOCKSTATE { IDLE = 0, DOCKED = 1, DOCKING = 2, BUMPSTOP = -1, 
  //             CLIFFSTOP = -2, DROPSTOP = -3, LOSTSTOP = -4 }
  const char* state[] = {"LOSTSTOP", "DROPSTOP", "CLIFFSTOP",
                         "BUMPSTOP", "IDLE", "DOCKED", "DOCKING"};
  m_debug ? printf("Current dock state: %s\n",
                   state[m_kobuki->getDockState() - LOSTSTOP]) : true;
  if (m_dock && !m_kobuki->isDocked())
    {
      m_debug ? printf("Docking...\n") : true;
      m_kobuki->dock(false);
    }
  m_dockPre = m_dock;
  return RTC::RTC_OK;
}

extern "C"
{

  void KobukiAISTInit(RTC::Manager* manager)
  {
    coil::Properties profile(kobukiaist_spec);
    manager->registerFactory(profile,
                             RTC::Create<KobukiAIST>,
                             RTC::Delete<KobukiAIST>);
  }

};


