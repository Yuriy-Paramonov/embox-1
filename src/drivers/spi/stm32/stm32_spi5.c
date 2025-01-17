/**
 * @file
 * @brief
 * @author Denis Deryugin <deryugin.denis@gmail.com>
 * @author Andrew Bursian
 * @version
 * @date 25.12.2019
 */

#include <string.h>
#include <drivers/spi.h>

#include <embox/unit.h>

#include "stm32_spi.h"

#include <config/board_config.h>

EMBOX_UNIT_INIT(stm32_spi5_init);

static struct stm32_spi stm32_spi5 = {
#if defined(CONF_SPI5_PIN_CS_PORT)
	.nss_port = CONF_SPI5_PIN_CS_PORT,
	.nss_pin  = CONF_SPI5_PIN_CS_NR,
#endif
};

static int stm32_spi5_init(void) {
	GPIO_InitTypeDef  GPIO_InitStruct;

	CONF_SPI5_CLK_ENABLE_SCK();
	CONF_SPI5_CLK_ENABLE_MISO();
	CONF_SPI5_CLK_ENABLE_MOSI();
	CONF_SPI5_CLK_ENABLE_CS();
	CONF_SPI5_CLK_ENABLE_SPI();

	stm32_spi_init(&stm32_spi5, SPI5);

	memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));
	GPIO_InitStruct.Pin       = CONF_SPI5_PIN_SCK_NR;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = CONF_SPI5_PIN_SCK_AF;
	HAL_GPIO_Init(CONF_SPI5_PIN_SCK_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = CONF_SPI5_PIN_MISO_NR;
	GPIO_InitStruct.Alternate = CONF_SPI5_PIN_MISO_AF;
	HAL_GPIO_Init(CONF_SPI5_PIN_MISO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = CONF_SPI5_PIN_MOSI_NR;
	GPIO_InitStruct.Alternate = CONF_SPI5_PIN_MOSI_AF;
	HAL_GPIO_Init(CONF_SPI5_PIN_MOSI_PORT, &GPIO_InitStruct);

#if defined(CONF_SPI5_PIN_CS_PORT)
	/* Chip Select is usual GPIO pin. */
	GPIO_InitStruct.Pin       = CONF_SPI5_PIN_CS_NR;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	HAL_GPIO_Init(CONF_SPI5_PIN_CS_PORT, &GPIO_InitStruct);

	/* Chip Select is in inactive state by default. */
	HAL_GPIO_WritePin(CONF_SPI5_PIN_CS_PORT, CONF_SPI5_PIN_CS_NR, GPIO_PIN_SET);
#endif

	return 0;
}

SPI_DEV_DEF("stm32_spi_5", &stm32_spi_ops, &stm32_spi5, 5);

