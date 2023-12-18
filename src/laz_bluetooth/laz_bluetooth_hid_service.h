#ifndef _LAZ_BLUETOOTH_HID_SERVICE_H_
#define _LAZ_BLUETOOTH_HID_SERVICE_H_

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

enum {
    HIDS_REMOTE_WAKE = BIT(0),
    HIDS_NORMALLY_CONNECTABLE = BIT(1),
};

enum {
    HIDS_INPUT   = 0x01,
    HIDS_OUTPUT  = 0x02,
    HIDS_FEATURE = 0x03,
};

struct hids_info {
    uint16_t version; /* version number of base USB HID Specification */
    uint8_t  code; /* country HID Device hardware is localized for. */
    uint8_t  flags;
} __packed;

struct hids_report {
    uint8_t id; /* report id */
    uint8_t type; /* report type */
} __packed;

ssize_t laz_bluebooth_hid_service_read_hids_info(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    void *buf,
    uint16_t len,
    uint16_t offset
);

ssize_t laz_bluebooth_hid_service_read_hid_report_map(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    void *buf,
    uint16_t len,
    uint16_t offset
);

ssize_t laz_bluebooth_hid_service_read_input_report(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    void *buf,
    uint16_t len,
    uint16_t offset
);

void laz_bluebooth_hid_service_ccc_changed(
    const struct bt_gatt_attr *attr,
    uint16_t value
);

#endif // _LAZ_BLUETOOTH_HID_SERVICE_H_
