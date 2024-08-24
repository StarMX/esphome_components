import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_BATTERY_LEVEL, CONF_TYPE, UNIT_PERCENT, ICON_BATTERY, DEVICE_CLASS_TEMPERATURE, DEVICE_CLASS_BATTERY, DEVICE_CLASS_BATTERY_CHARGING, STATE_CLASS_MEASUREMENT, CONF_BATTERY_VOLTAGE, UNIT_WATT, DEVICE_CLASS_POWER, ENTITY_CATEGORY_DIAGNOSTIC, DEVICE_CLASS_CURRENT, UNIT_AMPERE, UNIT_VOLT, DEVICE_CLASS_VOLTAGE, UNIT_CELSIUS
from .. import axp192_ns, Axp192Component, CONF_AXP192_ID


DEPENDENCIES = ['axp192', 'sensor']
Axp192Sensor = axp192_ns.class_('Axp192Sensor', cg.Component)

CONF_ACIN_VOLTAGE = "acin_voltage"
CONF_ACIN_CURRENT = "acin_current"
CONF_VBUS_VOLTAGE = "vbus_voltage"
CONF_VBUS_CURRENT = "vbus_current"
CONF_AXP_TEMP = "axp_temp"
CONF_BATTERY_TEMP = "battery_temp"
CONF_BATTERY_POWER = "battery_power"
CONF_BATTERY_CHARGE_CURRENT = "battery_charge_current"
CONF_BATTERY_DISCHARGE_CURRENT = "battery_discharge_current"
CONF_APS_VOLTAGE = "aps_voltage"
CONF_GPIO0_VOLTAGE = "gpio0_voltage"
CONF_GPIO1_VOLTAGE = "gpio1_voltage"
CONF_GPIO2_VOLTAGE = "gpio2_voltage"
CONF_GPIO3_VOLTAGE = "gpio3_voltage"

sensor_type = axp192_ns.enum('SensorType', is_class=True)
SENSOR_TYPE = {
    CONF_ACIN_VOLTAGE: sensor_type.ACIN_VOLTAGE,
    CONF_ACIN_CURRENT: sensor_type.ACIN_CURRENT,
    CONF_VBUS_VOLTAGE: sensor_type.VBUS_VOLTAGE,
    CONF_VBUS_CURRENT: sensor_type.VBUS_CURRENT,
    CONF_AXP_TEMP: sensor_type.AXP_TEMP,
    CONF_BATTERY_TEMP: sensor_type.BATTERY_TEMP,
    CONF_BATTERY_POWER: sensor_type.BATTERY_POWER,
    CONF_BATTERY_VOLTAGE: sensor_type.BATTERY_VOLTAGE,
    CONF_BATTERY_CHARGE_CURRENT: sensor_type.BATTERY_CHARGE_CURRENT,
    CONF_BATTERY_DISCHARGE_CURRENT: sensor_type.BATTERY_DISCHARGE_CURRENT,
    CONF_APS_VOLTAGE: sensor_type.APS_VOLTAGE,
    CONF_GPIO0_VOLTAGE: sensor_type.GPIO0_VOLTAGE,
    CONF_GPIO1_VOLTAGE: sensor_type.GPIO1_VOLTAGE,
    CONF_GPIO2_VOLTAGE: sensor_type.GPIO2_VOLTAGE,
    CONF_GPIO3_VOLTAGE: sensor_type.GPIO3_VOLTAGE,
}

TYPES = {
    CONF_ACIN_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_ACIN_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=4,
        device_class=DEVICE_CLASS_CURRENT,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_VBUS_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_VBUS_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=4,
        device_class=DEVICE_CLASS_CURRENT,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_AXP_TEMP: sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_BATTERY_TEMP: sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_BATTERY_POWER: sensor.sensor_schema(
        unit_of_measurement=UNIT_WATT,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_POWER,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_BATTERY_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_BATTERY_CHARGE_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=4,
        device_class=DEVICE_CLASS_CURRENT,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_BATTERY_DISCHARGE_CURRENT: sensor.sensor_schema(
        unit_of_measurement=UNIT_AMPERE,
        accuracy_decimals=4,
        device_class=DEVICE_CLASS_CURRENT,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_APS_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=4,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_GPIO0_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=4,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_GPIO1_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=4,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_GPIO2_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=4,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
    CONF_GPIO3_VOLTAGE: sensor.sensor_schema(
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=4,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ).extend({cv.GenerateID(): cv.declare_id(Axp192Sensor), cv.Required(CONF_AXP192_ID): cv.use_id(Axp192Component)}),
}

CONFIG_SCHEMA = cv.typed_schema(
    TYPES,
    lower=True,
    key=CONF_TYPE
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await sensor.register_sensor(var, config)
    await cg.register_parented(var, config[CONF_AXP192_ID])
    cg.add(var.set_sensor(SENSOR_TYPE[config[CONF_TYPE]]))
    await cg.register_component(var, config)
