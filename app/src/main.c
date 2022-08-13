#include "lfs_port.h"
#include "sfud.h"
#include "main.h"

static BOOT_ErrorStatus jump_app(void)
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

int main(void)
{
  boot_init();
  LFS_Init();
  sfud_init();
  jump_app();
  return 0;
}