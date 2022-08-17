#include "lfs_port.h"
#include "board.h"
#include "boot_load.h"
#include "main.h"

static BOOT_ErrorStatus jump_to_app(void)
{
  BOOT_ErrorStatus e_ret_status = BOOT_SUCCESS;
  uint32_t jump_address;
  typedef void (*Function_Pointer)(void);
  Function_Pointer p_jump_to_function;

  if (e_ret_status == BOOT_SUCCESS)
  {
    /* Initialize address to jump */
    jump_address = *(__IO uint32_t *)(((uint32_t)LOADER_REGION_ROM_START + 4));
    p_jump_to_function = (Function_Pointer)jump_address;

    /* Initialize loader's Stack Pointer */
    __set_MSP(*(__IO uint32_t *)(LOADER_REGION_ROM_START));

    /* Jump into loader */
    p_jump_to_function();
  }

  /* The point below should NOT be reached */
  return e_ret_status;
}

static BOOT_ErrorStatus progarm_app(UPDATA_STATUS status)
{
  /*  lfs values */
  uint32_t NbrBlocks = 0;
  uint32_t PacketsReceived = 0;
  uint32_t packet_size = 0;
  struct lfs_info lfs_file_info = {0};
  lfs_file_t file;
  static uint8_t App_data[PACKET_1K_SIZE] __attribute__((aligned(8)));

  /* if app status is not APP_UPDATE ,Restore default program */
  if (status == APP_UPDATA)
  {
    lfs_file_open(&lfs, &file, "UserApp", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_rewind(&lfs, &file);
    lfs_stat(&lfs, "UserApp", &lfs_file_info);
  }
  else
  {
    lfs_file_open(&lfs, &file, "DefApp", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_rewind(&lfs, &file);
    lfs_stat(&lfs, "DefApp", &lfs_file_info);
  }
  BOOT_HeaderPktRxCpltCallback(lfs_file_info.size);
  NbrBlocks = (lfs_file_info.size + (PACKET_1K_SIZE - 1U)) / PACKET_1K_SIZE;
  while (NbrBlocks - PacketsReceived)
  {
    packet_size = PACKET_1K_SIZE;
    if (NbrBlocks == PacketsReceived + 1)
    {
      if (0U == (lfs_file_info.size % PACKET_1K_SIZE))
      {
        /* The last packet must be fully considered */
        packet_size = PACKET_1K_SIZE;
      }
      else
      {
        /* The last packet is not full, drop the extra bytes */
        packet_size = lfs_file_info.size - ((uint32_t)(lfs_file_info.size / PACKET_1K_SIZE) * PACKET_1K_SIZE);
      }
    }
    if (lfs_file_read(&lfs, &file, &App_data, packet_size) != BOOT_SUCCESS)
    {
      lfs_file_close(&lfs,&file);
      // BSP_LED_Off(LED_RED);
      return BOOT_ERROR;
    }
    PacketsReceived++;
    // BSP_LED_Off(LED_RED);
    if (BOOT_DataPktRxCpltCallback(App_data, packet_size) != BOOT_SUCCESS)
    {
      PacketsReceived = 0;
      NbrBlocks = 0;
      lfs_file_close(&lfs,&file);
      lfs_remove(&lfs, "UserApp");
      return BOOT_ERROR;
    }
    // BSP_LED_On(LED_RED);
  }
  return BOOT_SUCCESS;
}

static BOOT_ErrorStatus app_updata(void)
{
  lfs_file_t file;
  UPDATA_STATUS app_status;
  if (lfs_file_open(&lfs, &file, "APP_STATUS", LFS_O_RDWR | LFS_O_CREAT) == LFS_ERR_OK)
  {
    lfs_file_read(&lfs, &file, &app_status, 1);
    lfs_file_close(&lfs, &file);
  }
  if (app_status != APP_OK)
  {
    progarm_app(app_status);
    if (lfs_file_open(&lfs, &file, "APP_STATUS", LFS_O_RDWR | LFS_O_CREAT) == LFS_ERR_OK)
    {
      app_status = APP_READY;
      lfs_file_write(&lfs, &file, &app_status, 1);
      lfs_file_close(&lfs, &file);
    }
  }
  return BOOT_SUCCESS;
}
int main(void)
{
  boot_init();
  if (BOOT_SUCCESS != lfs_Init())
  {
    goto jump;
  }
  app_updata();
jump:
  jump_to_app();
  return 0;
}