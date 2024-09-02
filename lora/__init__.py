import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import spi
from esphome.const import CONF_ID,CONF_CS_PIN,CONF_RESET_PIN 


DEPENDENCIES = ["spi"]

CONF_DIO0_PIN = 'dio0_pin'
CONF_FREQUENCY = "frequency" #通信频率
CONF_TX_POWER = 'tx_power' #发射功率
CONF_PREAMBLE_LENGTH='preamble_length' #前导码长度
CONF_BANDWIDTH = "bandwidth" #带宽
CONF_SPREADING_FACTOR="spreading_factor" #扩频因子，决定了数据传输的速率和抗干扰能力
CONF_CODING_RATE="coding_rate" #编码率

lora_ns = cg.esphome_ns.namespace("lora")
Lora = lora_ns.class_("SX127x", cg.PollingComponent, spi.SPIDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Lora),
            cv.Required(CONF_RESET_PIN): pins.internal_gpio_output_pin_schema,
            cv.Required(CONF_DIO0_PIN): pins.internal_gpio_output_pin_schema,
            cv.Required(CONF_FREQUENCY): cv.int_range(min=410000000, max=525000000),
            cv.Optional(CONF_BANDWIDTH, default=7): cv.int_range(min=0, max=9),
            cv.Optional(CONF_TX_POWER, default=17): cv.int_range(min=2, max=17),
            cv.Optional(CONF_PREAMBLE_LENGTH, default=8): cv.int_range(min=0, max=65535),
            cv.Optional(CONF_SPREADING_FACTOR, default=7): cv.int_range(min=6, max=12),
            cv.Optional(CONF_CODING_RATE, default=5): cv.int_range(min=5, max=8),

        }
    ).extend(spi.spi_device_schema(True, 8e6, 'mode0')).extend(cv.polling_component_schema('5s'))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await spi.register_spi_device(var, config)
    if CONF_RESET_PIN in config:
        cg.add(var.set_reset_pin(await cg.gpio_pin_expression(config[CONF_RESET_PIN])))
    cg.add(var.set_dio0_pin(await cg.gpio_pin_expression(config[CONF_DIO0_PIN])))
    cg.add(var.set_frequency(config[CONF_FREQUENCY]))
    cg.add(var.set_bandwidth(config[CONF_BANDWIDTH]))
    cg.add(var.set_tx_power(config[CONF_TX_POWER]))
    cg.add(var.set_preamble_length(config[CONF_PREAMBLE_LENGTH]))
    cg.add(var.set_spreading_factor(config[CONF_SPREADING_FACTOR]))
    cg.add(var.set_coding_rate(config[CONF_CODING_RATE]))
    