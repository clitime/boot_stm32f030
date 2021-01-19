#ifndef FULL_ASSERT_H_
#define FULL_ASSERT_H_

#ifdef __cplusplus
extern "C" {
#endif


#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif
