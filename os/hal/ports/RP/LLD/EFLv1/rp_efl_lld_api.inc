/**
 * @brief   Hook invoked immediately before the RP EFL driver disables XIP.
 * @details Applications may override this weak hook in order to coordinate
 *          other cores, park threads, or perform any other preparation
 *          required before flash becomes temporarily unavailable through XIP.
 * @note    This hook is called in thread context while XIP is still enabled.
 */
void rpEflBeforeXipOff(void);

/**
 * @brief   Hook invoked immediately after the RP EFL driver restores XIP.
 * @details Applications may override this weak hook in order to resume the
 *          coordination established by @p rpEflBeforeXipOff().
 * @note    This hook is called in thread context after XIP has been restored.
 */
void rpEflAfterXipOn(void);

void efl_lld_init(void);
void efl_lld_start(EFlashDriver *eflp);
void efl_lld_stop(EFlashDriver *eflp);
const flash_descriptor_t *efl_lld_get_descriptor(void *instance);
flash_error_t efl_lld_read(void *instance, flash_offset_t offset,
                           size_t n, uint8_t *rp);
flash_error_t efl_lld_program(void *instance, flash_offset_t offset,
                              size_t n, const uint8_t *pp);
flash_error_t efl_lld_start_erase_all(void *instance);
flash_error_t efl_lld_start_erase_sector(void *instance,
                                         flash_sector_t sector);
flash_error_t efl_lld_start_erase_block(void *instance,
                                        uint8_t cmd,
                                        uint32_t erase_size,
                                        uint32_t block);
flash_error_t efl_lld_query_erase(void *instance, uint32_t *wait_time);
flash_error_t efl_lld_verify_erase(void *instance, flash_sector_t sector);
void efl_lld_read_unique_id(EFlashDriver *eflp, uint8_t *uid);
