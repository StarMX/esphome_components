import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    ENTITY_CATEGORY_CONFIG,
    ICON_POWER,
    DEVICE_CLASS_OUTLET,
    CONF_OUTPUT
)

from .. import axp192_ns, Axp192Component, CONF_AXP192_ID, OUTPUT_PIN

DEPENDENCIES = ["axp192", "switch"]

Axp192Switch = axp192_ns.class_("Axp192Switch", switch.Switch, cg.Component)

CONFIG_SCHEMA = switch.switch_schema(
    Axp192Switch,
    icon=ICON_POWER,
    device_class=DEVICE_CLASS_OUTLET,
    entity_category=ENTITY_CATEGORY_CONFIG,
).extend(cv.COMPONENT_SCHEMA).extend({
    cv.GenerateID(CONF_AXP192_ID): cv.use_id(Axp192Component),
    cv.Required(CONF_OUTPUT): cv.enum(OUTPUT_PIN),
})


async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_parented(var, config[CONF_AXP192_ID])
    cg.add(var.set_output(config[CONF_OUTPUT]))
    await cg.register_component(var, config)
    cg.add_define("USE_AXP92_SWITCH")