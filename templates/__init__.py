import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation, pins
from esphome.const import CONF_ID, CONF_TRIGGER_ID 

CODEOWNERS = ["@starz"]

templates_ns = cg.esphome_ns.namespace('templates')

CONF_MESSAGE = "message"
CONF_ON_DATA_RECEIVED = "on_data_received"
TemplatesComponent = templates_ns.class_('TemplatesComponent', cg.PollingComponent)
TemplatesSendAction = templates_ns.class_("TemplatesSendAction", automation.Action)
TemplatesDataReceivedMessageTrigger = templates_ns.class_(
    "TemplatesDataReceivedMessageTrigger",
    automation.Trigger.template(cg.std_string),
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TemplatesComponent),
    cv.Optional(CONF_ON_DATA_RECEIVED): automation.validate_automation(
        {
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                TemplatesDataReceivedMessageTrigger
            ),
        }
    )
}).extend(cv.polling_component_schema('10s'))


async def to_code(config):
    cg.add_global(templates_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
        
    await cg.register_component(var, config)
    # cg.add_library("sandeepmistry/Templates", "0.8.0")

    for conf in config.get(CONF_ON_DATA_RECEIVED, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(
            trigger, [(cg.std_string, "message")], conf
        )


TEMPLATES_SEND_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(TemplatesComponent),
        cv.Required(CONF_MESSAGE): cv.templatable(cv.string),
    }
)

@automation.register_action(
    "templates.send", TemplatesSendAction, TEMPLATES_SEND_SCHEMA
)

async def templates_send_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    data = await cg.templatable(config[CONF_MESSAGE], args, cg.std_string)
    cg.add(var.set_message(data))
    return var