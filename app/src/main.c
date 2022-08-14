#include "lfs_port.h"
#include "main.h"

static BOOT_ErrorStatus jump_to_app(void)
{
  BOOT_ErrorStatus e_ret_status = BOOT_ERROR;
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

static BOOT_ErrorStatus app_updata(void)
{
  lfs_file_t file;
  uint8_t app_status;
  if (lfs_file_open(&lfs, &file, "APP_STATUS", LFS_O_RDWR | LFS_O_CREAT) == LFS_ERR_OK)
  {
    lfs_file_read(&lfs, &file, &app_status, 1);
    lfs_file_close(&lfs, &file);
  }
  if (app_status != 1)
  {
    if (lfs_file_open(&lfs, &file, "APP_STATUS", LFS_O_RDWR | LFS_O_CREAT) == LFS_ERR_OK)
    {
      app_status = 1;
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