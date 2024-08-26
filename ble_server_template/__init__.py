import esphome.codegen as cg
from esphome.components import esp32_ble_server
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@StarZ"]

DEPENDENCIES = ["esp32"]
CONFLICTS_WITH = ["esp32_ble_tracker", "esp32_ble_beacon"]
AUTO_LOAD = ["esp32_ble_server"]

CONF_BLE_SERVER_ID = "ble_server_id"

ble_server_template_ns = cg.esphome_ns.namespace("ble_server_template")
TemplateBleServer = ble_server_template_ns.class_(
    "TemplateBleServer", cg.Component, esp32_ble_server.BLEServiceComponent
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TemplateBleServer),
        cv.GenerateID(CONF_BLE_SERVER_ID): cv.use_id(esp32_ble_server.BLEServer),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    ble_server = await cg.get_variable(config[CONF_BLE_SERVER_ID])
    cg.add(ble_server.register_service_component(var))
