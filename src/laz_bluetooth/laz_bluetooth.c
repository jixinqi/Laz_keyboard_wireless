#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "laz_bluetooth.h"

LOG_MODULE_REGISTER(laz_bluetooth, LOG_LEVEL_DBG);

#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)

static const struct bt_data ad[] = 
{
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(
        BT_DATA_NAME_COMPLETE,
        DEVICE_NAME,
        DEVICE_NAME_LEN
    ),
};

static const struct bt_data sd[] = 
{
    BT_DATA_BYTES(
        BT_DATA_UUID16_ALL,
        BT_UUID_16_ENCODE(BT_UUID_HIDS_VAL),
        BT_UUID_16_ENCODE(BT_UUID_BAS_VAL)
    )
};

struct bt_conn* laz_bt_conn;

static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(
    (BT_LE_ADV_OPT_CONNECTABLE|BT_LE_ADV_OPT_USE_IDENTITY),
    800,
    801,
    NULL
); 

struct bt_conn_cb laz_bluetooth_connection_callbacks = {
	.connected    = on_laz_bluetooth_connected,
	.disconnected = on_laz_bluetooth_disconnected
};

int laz_bluetooth_init()
{
    int ec;

    bt_addr_le_t addr;
    ec = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
    if (ec)
    {
        LOG_ERR("bt_addr_le_from_str() failed! (ec %d)", ec);
    }

    ec = bt_id_create(&addr, NULL);
    if (ec < 0)
    {
        LOG_ERR("bt_id_create() failed! (ec %d)", ec);
    }

    bt_conn_cb_register(&laz_bluetooth_connection_callbacks);

    ec = bt_enable(NULL);
    if (ec)
    {
        LOG_ERR("bt_enable() failed! (ec %d)", ec);
    }

    ec = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (ec)
    {
        LOG_ERR("bt_le_adv_start() failed! (ec %d)", ec);
    }

    return ec;
}

void on_laz_bluetooth_connected(struct bt_conn *conn, uint8_t ec)
{
    LOG_INF("Bluetooth connected!");
}

void on_laz_bluetooth_disconnected(struct bt_conn *conn, uint8_t ec)
{
    LOG_INF("Bluetooth disconnected!");
}

