import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation, pins
from esphome.const import CONF_ID, CONF_TRIGGER_ID, CONF_CS_PIN, CONF_RESET_PIN, CONF_BUSY_PIN 

CODEOWNERS = ["@starz"]

template_ns = cg.esphome_ns.namespace('template')

CONF_MESSAGE = "message"
CONF_ON_DATA_RECEIVED = "on_data_received"
TemplateComponent = template_ns.class_('TemplateComponent', cg.PollingComponent)
TemplateSendAction = template_ns.class_("TemplateSendAction", automation.Action)
TemplateDataReceivedMessageTrigger = template_ns.class_(
    "TemplateDataReceivedMessageTrigger",
    automation.Trigger.template(cg.std_string),
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TemplateComponent),
    cv.Optional(CONF_ON_DATA_RECEIVED): automation.validate_automation(
        {
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                TemplateDataReceivedMessageTrigger
            ),
        }
    ),
    cv.Required(CONF_CS_PIN): pins.internal_gpio_output_pin_number,
    cv.Optional(CONF_RESET_PIN): pins.internal_gpio_output_pin_number,
}).extend(cv.polling_component_schema('10s'))


async def to_code(config):
    cg.add_global(template_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    if CONF_CS_PIN in config:
        cg.add(var.set_reset_pin(config[CONF_CS_PIN]))
    if CONF_RESET_PIN in config:
        cg.add(var.set_reset_pin(config[CONF_RESET_PIN]))  
        
    await cg.register_component(var, config)
    # cg.add_library("sandeepmistry/Template", "0.8.0")

    for conf in config.get(CONF_ON_DATA_RECEIVED, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(
            trigger, [(cg.std_string, "message")], conf
        )


TEMPLATE_SEND_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(TemplateComponent),
        cv.Required(CONF_MESSAGE): cv.templatable(cv.string),
    }
)

@automation.register_action(
    "template.send", TemplateSendAction, TEMPLATE_SEND_SCHEMA
)

async def template_send_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(config[CONF_MESSAGE], args, cg.std_string)
    cg.add(var.set_message(template_))
    return var