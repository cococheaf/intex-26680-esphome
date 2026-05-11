import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_ID

DEPENDENCIES = ["esp32"]

CONF_ENABLED = "enabled"
CONF_LOG_RAW = "log_raw"
CONF_SCL_PIN = "scl_pin"
CONF_SDA_PIN = "sda_pin"

tm1650_sniffer_ns = cg.esphome_ns.namespace("tm1650_sniffer")
TM1650Sniffer = tm1650_sniffer_ns.class_("TM1650Sniffer", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TM1650Sniffer),
        cv.Required(CONF_SCL_PIN): pins.gpio_input_pin_schema,
        cv.Required(CONF_SDA_PIN): pins.gpio_input_pin_schema,
        cv.Optional(CONF_ENABLED, default=True): cv.boolean,
        cv.Optional(CONF_LOG_RAW, default=True): cv.boolean,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    scl_pin = await cg.gpio_pin_expression(config[CONF_SCL_PIN])
    sda_pin = await cg.gpio_pin_expression(config[CONF_SDA_PIN])

    cg.add(var.set_scl_pin(scl_pin))
    cg.add(var.set_sda_pin(sda_pin))
    cg.add(var.set_enabled(config[CONF_ENABLED]))
    cg.add(var.set_log_raw(config[CONF_LOG_RAW]))
