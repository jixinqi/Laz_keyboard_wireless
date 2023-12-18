#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "laz_bluetooth/laz_bluetooth.h"

LOG_MODULE_REGISTER(Laz_keyboard_wireless, LOG_LEVEL_DBG);

int main(void)
{
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);

	laz_bluetooth_init();

	while(1)
	{
		k_sleep(K_HOURS(1));
	}
	return 0;
}
