import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_OUTPUT, CONF_ID, CONF_VOLTAGE
from .. import axp192_ns, Axp192Component, CONF_AXP192_ID, OUTPUT_PIN

DEPENDENCIES = ["axp192", "output"]

Axp192Output = axp192_ns.class_("Axp192Output", output.FloatOutput)

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(Axp192Output),
        cv.GenerateID(CONF_AXP192_ID): cv.use_id(Axp192Component),
        cv.Required(CONF_OUTPUT): cv.enum(OUTPUT_PIN),
        cv.Optional(CONF_VOLTAGE): cv.voltage
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_AXP192_ID])
    cg.add(var.set_output(config[CONF_OUTPUT]))
    if CONF_VOLTAGE in config:
        cg.add(var.set_voltage(config[CONF_VOLTAGE]))
    await output.register_output(var, config)
    return var
