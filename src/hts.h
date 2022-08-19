// the configured options and settings
#define HTS_CORE_VERSION_MAJOR 0
#define HTS_CORE_VERSION_MINOR 1

/**
 * @brief Status codes to support the HTS system
 * 
 */

typedef enum
{
    HTS_STATUS_OK=0,    /*!< Success / OK Results */
    HTS_STATUS_FAILURE  /*!< General failure */
}hts_status_t;

/**
 * @brief Register HAL tests suite to the system
 * 
 * @return int 
 */
extern hts_status_t hts_register_test_suite( void );
