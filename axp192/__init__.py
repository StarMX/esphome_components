import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c, sensor, binary_sensor
from esphome.const import CONF_ID, CONF_BRIGHTNESS, ICON_POWER, ENTITY_CATEGORY_DIAGNOSTIC

MULTI_CONF = True

CONF_AXP192_ID = 'axp192_id'
DEPENDENCIES = ['i2c']

CONF_POWER_BUTTON = 'power_button_sensor'
CONF_DISABLE_LDOIO0 = 'disable_ldoio0'
CONF_DISABLE_RTC = 'disable_ldo1'
CONF_DISABLE_LDO2 = 'disable_ldo2'
CONF_DISABLE_LDO3 = 'disable_ldo3'
CONF_DISABLE_DCDC1 = 'disable_dcdc1'
CONF_DISABLE_DCDC3 = 'disable_dcdc3'
CONF_LDO2_VOLTAGE = 'ldo2_voltage'
CONF_LDO3_VOLTAGE = 'ldo3_voltage'
CONF_DCDC1_VOLTAGE = 'dcdc1_voltage'
CONF_DCDC3_VOLTAGE = 'dcdc3_voltage'
CONF_LDOIO0_VOLTAGE = 'ldoio0_voltage'
axp192_ns = cg.esphome_ns.namespace('axp192')


CONF_OUTPUT_LDO2 = 'ldo2'
CONF_OUTPUT_LDO3 = 'ldo3'
CONF_OUTPUT_DCDC1 = 'dcdc1'
CONF_OUTPUT_DCDC3 = 'dcdc3'
CONF_OUTPUT_LDOIO0 = 'ldoio0'

output_pin = axp192_ns.enum('OutputPin', is_class=True)
OUTPUT_PIN = {
    CONF_OUTPUT_LDO2: output_pin.OUTPUT_LDO2,
    CONF_OUTPUT_LDO3: output_pin.OUTPUT_LDO3,
    CONF_OUTPUT_DCDC1: output_pin.OUTPUT_DCDC1,
    CONF_OUTPUT_DCDC3: output_pin.OUTPUT_DCDC3,
    CONF_OUTPUT_LDOIO0: output_pin.OUTPUT_LDOIO0,
}


Axp192Component = axp192_ns.class_(
    'Axp192Component', i2c.I2CDevice, cg.PollingComponent)

VoffVoltage = axp192_ns.enum('VoffVoltage', is_class=True)
VOFFVOLTAGE = {
    '2600mV': VoffVoltage.VOFF_2600MV,
    '2700mV': VoffVoltage.VOFF_2700MV,
    '2800mV': VoffVoltage.VOFF_2800MV,
    '2900mV': VoffVoltage.VOFF_2900MV,
    '3000mV': VoffVoltage.VOFF_3000MV,
    '3100mV': VoffVoltage.VOFF_3100MV,
    '3200mV': VoffVoltage.VOFF_3200MV,
    '3300mV': VoffVoltage.VOFF_3300MV,
}
CONF_VOFF_VOLTAGE = 'voff_voltage'

ChargeCurrent = axp192_ns.enum('ChargeCurrent', is_class=True)
CHARGECURRENT = {
    '100mA': ChargeCurrent.CHARGE_100MA,
    '190mA': ChargeCurrent.CHARGE_190MA,
    '280mA': ChargeCurrent.CHARGE_280MA,
    '360mA': ChargeCurrent.CHARGE_360MA,
    '450mA': ChargeCurrent.CHARGE_450MA,
    '550mA': ChargeCurrent.CHARGE_550MA,
    '630mA': ChargeCurrent.CHARGE_630MA,
    '700mA': ChargeCurrent.CHARGE_700MA,
}
CONF_CHARGE_CURRENT = 'charge_current'

ChargeVoltage = axp192_ns.enum('ChargeVoltage', is_class=True)
CHARGEVOLTAGE = {
    '4100mV': ChargeVoltage.CHARGE_4100MV,
    '4150mV': ChargeVoltage.CHARGE_4150MV,
    '4200mV': ChargeVoltage.CHARGE_4200MV,
    '4360mV': ChargeVoltage.CHARGE_4360MV,
}
CONF_CHARGE_VOLTAGE = 'charge_voltage'

VBusHoldVoltageLimit = axp192_ns.enum('VBusHoldVoltageLimit', is_class=True)
VBUS_HOLD_VOLTAGE_LIMIT = {
    '4000mV': VBusHoldVoltageLimit.HOLD_4000MV,
    '4100mV': VBusHoldVoltageLimit.HOLD_4100MV,
    '4200mV': VBusHoldVoltageLimit.HOLD_4200MV,
    '4300mV': VBusHoldVoltageLimit.HOLD_4300MV,
    '4400mV': VBusHoldVoltageLimit.HOLD_4400MV,
    '4500mV': VBusHoldVoltageLimit.HOLD_4500MV,
    '4600mV': VBusHoldVoltageLimit.HOLD_4600MV,
    '4700mV': VBusHoldVoltageLimit.HOLD_4700MV,
}
CONF_VBUS_HOLD_VOLTAGE_LIMIT = 'vbus_hold_voltage_limit'

VBusHoldVoltageLimited = axp192_ns.enum('VBusHoldVoltageLimited', is_class=True)
VBUS_HOLD_VOLTAGE_LIMITED = {
    'NO': VBusHoldVoltageLimited.VOLTAGE_NOT_LIMITED,
    'YES': VBusHoldVoltageLimited.VOLTAGE_LIMITED,
}
CONF_VBUS_HOLD_VOLTAGE_LIMITED = 'vbus_hold_voltage_limited'

VBusHoldCurrentLimit = axp192_ns.enum('VBusHoldCurrentLimit', is_class=True)
VBUS_HOLD_CURRENT_LIMIT = {
    '500mA': VBusHoldCurrentLimit.CURRENT_LIMIT_500MA,
    '100mA': VBusHoldCurrentLimit.CURRENT_LIMIT_100MA,
}
CONF_VBUS_HOLD_CURRENT_LIMIT = 'vbus_hold_current_limit'

VBusHoldCurrentLimited = axp192_ns.enum('VBusHoldCurrentLimited', is_class=True)
VBUS_HOLD_CURRENT_LIMITED = {
    'NO': VBusHoldCurrentLimited.CURRENT_NOT_LIMITED,
    'YES': VBusHoldCurrentLimited.CURRENT_LIMITED,
}
CONF_VBUS_HOLD_CURRENT_LIMITED = 'vbus_hold_current_limited'

VBusIpsout = axp192_ns.enum('VBusIpsout', is_class=True)
VBUS_IPSOUT = {
    'NO': VBusIpsout.IPSOUT_NOT_MANAGED,
    'YES': VBusIpsout.IPSOUT_MANAGED,
}
CONF_VBUS_IPSOUT = 'vbus_ipsout'

LDOio0Control = axp192_ns.enum('LDOio0Control', is_class=True)
LDOIO0_MODE = {
    'NMOS_OPENDRAIN': LDOio0Control.NMOS_OPENDRAIN,
    'GENERAL_INPUT': LDOio0Control.GENERAL_INPUT,
    'LOWNOISE_LDO': LDOio0Control.LOWNOISE_LDO,
    'ADC_IN': LDOio0Control.ADC_IN,
    'LOW_OUTPUT': LDOio0Control.LOW_OUTPUT,
    'FLOATING': LDOio0Control.FLOATING,

}
CONF_LDOIO0_MODE = 'ldoio0_mode'


PowerOffAction = axp192_ns.class_('PowerOffAction', automation.Action)
PrepareSleepAction = axp192_ns.class_('PrepareSleepAction', automation.Action)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Axp192Component),
    cv.Optional(CONF_POWER_BUTTON): binary_sensor.binary_sensor_schema(
        icon=ICON_POWER,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    cv.Optional(CONF_VOFF_VOLTAGE, default='3000mV'): cv.enum(VOFFVOLTAGE),
    cv.Optional(CONF_CHARGE_CURRENT, default='100mA'): cv.enum(CHARGECURRENT),
    cv.Optional(CONF_CHARGE_VOLTAGE, default='4200mV'): cv.enum(CHARGEVOLTAGE),
    cv.Optional(CONF_VBUS_HOLD_VOLTAGE_LIMIT, default='4400mV'): cv.enum(VBUS_HOLD_VOLTAGE_LIMIT),
    cv.Optional(CONF_VBUS_HOLD_VOLTAGE_LIMITED, default='YES'): cv.enum(VBUS_HOLD_VOLTAGE_LIMITED),
    cv.Optional(CONF_VBUS_HOLD_CURRENT_LIMIT, default='500mA'): cv.enum(VBUS_HOLD_CURRENT_LIMIT),
    cv.Optional(CONF_VBUS_HOLD_CURRENT_LIMITED, default='YES'): cv.enum(VBUS_HOLD_CURRENT_LIMITED),
    cv.Optional(CONF_VBUS_IPSOUT, default='NO'): cv.enum(VBUS_IPSOUT),
    cv.Optional(CONF_LDOIO0_MODE, default='FLOATING'): cv.enum(LDOIO0_MODE),
    cv.Optional(CONF_DISABLE_LDO2, default=False): cv.boolean,
    cv.Optional(CONF_DISABLE_LDO3, default=False): cv.boolean,
    cv.Optional(CONF_DISABLE_RTC, default=False): cv.boolean,
    cv.Optional(CONF_DISABLE_DCDC1, default=False): cv.boolean,
    cv.Optional(CONF_DISABLE_DCDC3, default=False): cv.boolean,
    cv.Optional(CONF_LDO2_VOLTAGE, default=3): cv.All(cv.voltage, cv.float_range(1.8, 3.3)),
    cv.Optional(CONF_LDO3_VOLTAGE, default=3.3): cv.All(cv.voltage, cv.float_range(1.8, 3.3)),
    cv.Optional(CONF_DCDC1_VOLTAGE, default=3.3): cv.All(cv.voltage, cv.float_range(0.7, 3.5)),
    cv.Optional(CONF_DCDC3_VOLTAGE, default=2.5): cv.All(cv.voltage, cv.float_range(0.7, 3.5)),
    cv.Optional(CONF_LDOIO0_VOLTAGE, default=2.8): cv.All(cv.voltage, cv.float_range(1.8, 3.3)),
}).extend(i2c.i2c_device_schema(0x77)).extend(cv.polling_component_schema('60s'))


async def to_code(config):
    cg.add_global(axp192_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    cg.add(var.set_voff(config[CONF_VOFF_VOLTAGE]))
    cg.add(var.set_charge_voltage(config[CONF_CHARGE_VOLTAGE]))
    cg.add(var.set_charge_current(config[CONF_CHARGE_CURRENT]))
    cg.add(var.set_vbus_ipsout(config[CONF_VBUS_IPSOUT]))
    cg.add(var.set_vbus_hold_current_limited(config[CONF_VBUS_HOLD_CURRENT_LIMITED]))
    cg.add(var.set_vbus_hold_current_limit(config[CONF_VBUS_HOLD_CURRENT_LIMIT]))
    cg.add(var.set_vbus_hold_voltage_limited(config[CONF_VBUS_HOLD_VOLTAGE_LIMITED]))
    cg.add(var.set_vbus_hold_voltage_limit(config[CONF_VBUS_HOLD_VOLTAGE_LIMIT]))
    cg.add(var.set_disable_rtc(config[CONF_DISABLE_RTC]))
    cg.add(var.set_disable_ldo2(config[CONF_DISABLE_LDO2]))
    cg.add(var.set_disable_ldo3(config[CONF_DISABLE_LDO3]))
    cg.add(var.set_disable_dcdc1(config[CONF_DISABLE_DCDC1]))
    cg.add(var.set_disable_dcdc3(config[CONF_DISABLE_DCDC3]))
    cg.add(var.set_dcdc1_voltage(config[CONF_DCDC1_VOLTAGE] * 1000))
    cg.add(var.set_dcdc3_voltage(config[CONF_DCDC3_VOLTAGE] * 1000))
    cg.add(var.set_ldo2_voltage(config[CONF_LDO2_VOLTAGE] * 1000))
    cg.add(var.set_ldo3_voltage(config[CONF_LDO3_VOLTAGE] * 1000))
    cg.add(var.set_ldoio0_voltage(config[CONF_LDOIO0_VOLTAGE] * 1000))
    cg.add(var.set_ldoio0_mode(config[CONF_LDOIO0_MODE]))

    if (CONF_POWER_BUTTON in config):
        button = await binary_sensor.new_binary_sensor(config[CONF_POWER_BUTTON])
        cg.add(var.monitor_power_button(button))


@automation.register_action(
    'axp192.power_off',
    PowerOffAction,
    cv.Schema({cv.GenerateID(): cv.use_id(Axp192Component)}),
)
async def axp192_power_off_to_code(config, action_id, args):
    var = cg.new_Pvariable(action_id)
    await cg.register_parented(var, config[CONF_ID])
    return var


@automation.register_action(
    'axp192.prepare_sleep',
    PrepareSleepAction,
    cv.Schema({cv.GenerateID(): cv.use_id(Axp192Component)}),
)
async def axp192_prepare_sleep_to_code(config, action_id, args):
    var = cg.new_Pvariable(action_id)
    await cg.register_parented(var, config[CONF_ID])
    return var
