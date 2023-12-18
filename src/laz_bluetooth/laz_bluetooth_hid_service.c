#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "laz_bluetooth_hid_service.h"

LOG_MODULE_REGISTER(laz_bluetooth_hid_service, LOG_LEVEL_DBG);

static struct hids_info hids_info_data = {
    .version = 0x0000,
    .code = 0x00,
    .flags = HIDS_NORMALLY_CONNECTABLE,
};

static uint8_t hid_descriptor_report_map[] = {
    0x05, 0x01, /* Usage Page (Generic Desktop Ctrls) */
    0x09, 0x02, /* Usage (Mouse) */
    0xA1, 0x01, /* Collection (Application) */
    0x85, 0x01, /*	 Report Id (1) */
    0x09, 0x01, /*   Usage (Pointer) */
    0xA1, 0x00, /*   Collection (Physical) */
    0x05, 0x09, /*     Usage Page (Button) */
    0x19, 0x01, /*     Usage Minimum (0x01) */
    0x29, 0x03, /*     Usage Maximum (0x03) */
    0x15, 0x00, /*     Logical Minimum (0) */
    0x25, 0x01, /*     Logical Maximum (1) */
    0x95, 0x03, /*     Report Count (3) */
    0x75, 0x01, /*     Report Size (1) */
    0x81, 0x02, /*     Input (Data,Var,Abs,No Wrap,Linear,...) */
    0x95, 0x01, /*     Report Count (1) */
    0x75, 0x05, /*     Report Size (5) */
    0x81, 0x03, /*     Input (Const,Var,Abs,No Wrap,Linear,...) */
    0x05, 0x01, /*     Usage Page (Generic Desktop Ctrls) */
    0x09, 0x30, /*     Usage (X) */
    0x09, 0x31, /*     Usage (Y) */
    0x15, 0x81, /*     Logical Minimum (129) */
    0x25, 0x7F, /*     Logical Maximum (127) */
    0x75, 0x08, /*     Report Size (8) */
    0x95, 0x02, /*     Report Count (2) */
    0x81, 0x06, /*     Input (Data,Var,Rel,No Wrap,Linear,...) */
    0xC0,       /*   End Collection */
    0xC0,       /* End Collection */
};

static uint8_t simulate_input;

#if CONFIG_SAMPLE_BT_USE_AUTHENTICATION
/* Require encryption using authenticated link-key. */
#define SAMPLE_BT_PERM_READ BT_GATT_PERM_READ_AUTHEN
#define SAMPLE_BT_PERM_WRITE BT_GATT_PERM_WRITE_AUTHEN
#else
/* Require encryption. */
#define SAMPLE_BT_PERM_READ BT_GATT_PERM_READ_ENCRYPT
#define SAMPLE_BT_PERM_WRITE BT_GATT_PERM_WRITE_ENCRYPT
#endif

BT_GATT_SERVICE_DEFINE(Laz_keyboard,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_HIDS),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_HIDS_INFO,
        BT_GATT_CHRC_READ,
        BT_GATT_PERM_READ,
        laz_bluebooth_hid_service_read_hids_info,
        NULL,
        &hids_info_data
    ),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_HIDS_REPORT_MAP,
        BT_GATT_CHRC_READ,
        BT_GATT_PERM_READ,
        laz_bluebooth_hid_service_read_hid_report_map,
        NULL,
        NULL
    ),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_HIDS_REPORT,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        SAMPLE_BT_PERM_READ,
        laz_bluebooth_hid_service_read_input_report,
        NULL,
        NULL
    ),
    BT_GATT_CCC(
        laz_bluebooth_hid_service_ccc_changed,
		SAMPLE_BT_PERM_READ | SAMPLE_BT_PERM_WRITE
    ),
    ////////////////////////////////////////////////////////////////////////////////

    BT_GATT_PRIMARY_SERVICE(BT_UUID_BAS),
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DIS),
);

ssize_t laz_bluebooth_hid_service_read_hids_info(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    void *buf,
    uint16_t len,
    uint16_t offset
)
{
    LOG_DBG("Into Function: %s", __FUNCTION__);
    return bt_gatt_attr_read(
        conn,
        attr,
        buf,
        len,
        offset,
        attr->user_data,
        sizeof(struct hids_info)
    );
}

ssize_t laz_bluebooth_hid_service_read_hid_report_map(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    void *buf,
    uint16_t len,
    uint16_t offset
)
{
    LOG_DBG("Into Function: %s", __FUNCTION__);
    return bt_gatt_attr_read(
        conn,
        attr,
        buf,
        len,
        offset,
        hid_descriptor_report_map,
        sizeof(hid_descriptor_report_map)
    );
}

ssize_t laz_bluebooth_hid_service_read_input_report(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    void *buf,
    uint16_t len,
    uint16_t offset
)
{
    LOG_DBG("Into Function: %s", __FUNCTION__);
    return bt_gatt_attr_read(
        conn,
        attr,
        buf,
        len,
        offset,
        NULL,
        0
    );
}

void laz_bluebooth_hid_service_ccc_changed(
    const struct bt_gatt_attr *attr,
    uint16_t value
)
{
    LOG_DBG("Into Function: %s", __FUNCTION__);
	simulate_input = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}
